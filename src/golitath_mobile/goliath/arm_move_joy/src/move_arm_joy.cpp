#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/joy.hpp>
#include <geometry_msgs/msg/pose_stamped.hpp>
#include <moveit/move_group_interface/move_group_interface.h>
#include <moveit_msgs/msg/move_it_error_codes.hpp>
#include <tf2/LinearMath/Quaternion.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.hpp>
#include <cmath>

using MoveGroupInterface = moveit::planning_interface::MoveGroupInterface;
using Joy = sensor_msgs::msg::Joy;

class MoveArmJoy : public rclcpp::Node
{
public:
    MoveArmJoy() : Node("move_arm_joy")
    {
        auto node_ptr = std::shared_ptr<rclcpp::Node>(this, [](rclcpp::Node*) {});
        arm_ = std::make_shared<MoveGroupInterface>(node_ptr, "position");
        arm_->setMaxVelocityScalingFactor(0.15);
        arm_->setMaxAccelerationScalingFactor(0.15);

        cb_group_ = this->create_callback_group(rclcpp::CallbackGroupType::Reentrant);
        auto sub_options = rclcpp::SubscriptionOptions();
        sub_options.callback_group = cb_group_;

        joy_sub_ = this->create_subscription<Joy>(
            "joy", 10, std::bind(&MoveArmJoy::joyCallback, this, std::placeholders::_1), sub_options);

        pose_pub_ = this->create_publisher<geometry_msgs::msg::PoseStamped>("current_pose", 10);

        // System States
        is_woken_up_ = false;
        deadman_active_ = false;
        last_deadman_state_ = false;
        is_moving_ = false; // LOCK om dubbele/overlappende MoveIt plans te voorkomen

        // Button States
        last_triangle_state_ = false;
        last_circle_state_ = false;
        last_square_state_ = false;
        last_cross_state_ = false;

        // D-Pad & Stick Edge States
        last_dpad_up_ = false; last_dpad_down_ = false;
        last_dpad_left_ = false; last_dpad_right_ = false;
        
        last_stick_lx_ = 0; last_stick_ly_ = 0;
        last_stick_rx_ = 0; last_stick_ry_ = 0;
        last_trigger_l2_ = false; last_trigger_r2_ = false;

        // Instellingen
        step_size_ = 0.015;           // 1.5 cm per stap
        rotation_step_deg_ = 3.0;     // 3 graden per stap
        STICK_DEADZONE_ = 0.6;        // 60% Uitslag vereist voor sturen

        RCLCPP_INFO(this->get_logger(), "MoveArmJoy Node gestart!");
        RCLCPP_WARN(this->get_logger(), "--> DRUK OP DRIEHOEK OM DE ROBOT WAKKER TE MAKEN (wake_up) <--");
    }

private:
    void joyCallback(const Joy &msg)
    {
        // === 1. DEADMAN CHECK (L1) ===
        bool deadman_pressed = false;
        if (msg.buttons.size() > 4) {
            deadman_pressed = (msg.buttons[4] == 1);
        }

        if (deadman_pressed != last_deadman_state_) {
            if (deadman_pressed) {
                deadman_active_ = true;
                RCLCPP_INFO(this->get_logger(), "Deadman (L1) ACTIEF.");
            } else {
                deadman_active_ = false;
                arm_->stop();
                resetStates();
                is_moving_ = false;
                RCLCPP_WARN(this->get_logger(), "Deadman (L1) LOSGELATEN - Gestopt.");
            }
            last_deadman_state_ = deadman_pressed;
        }

        if (!deadman_active_) return;

        // === 2. WAKE UP CHECK (Driehoek) ===
        if (msg.buttons.size() > 2) {
            bool triangle = (msg.buttons[2] == 1);
            if (triangle && !last_triangle_state_ && !is_moving_) {
                RCLCPP_INFO(this->get_logger(), "Naar wake_up pose bewegen...");
                is_moving_ = true;
                if (goToNamedTarget("wake_up")) {
                    is_woken_up_ = true;
                    RCLCPP_INFO(this->get_logger(), ">>> ROBOT WAKKER! Cartesische besturing vrijgegeven. <<<");
                }
                is_moving_ = false;
            }
            last_triangle_state_ = triangle;
        }

        if (!is_woken_up_) {
            RCLCPP_WARN_THROTTLE(this->get_logger(), *this->get_clock(), 3000, 
                "Eerst wake_up uitvoeren (Driehoek)!");
            return;
        }

        // Als MoveIt momenteel nog een stap uitvoert: NEGEER nieuwe stick inputs!
        if (is_moving_) return;

        // === 3. OVERIGE NAMED POSES ===
        if (msg.buttons.size() > 0) {
            bool cross = (msg.buttons[0] == 1);
            if (cross && !last_cross_state_) {
                is_moving_ = true;
                goToNamedTarget("calibrated_up");
                is_moving_ = false;
            }
            last_cross_state_ = cross;
        }

        // === 4. YAW ROTATIE (Square / Circle) ===
        if (msg.buttons.size() > 3) {
            bool square = (msg.buttons[3] == 1);
            if (square && !last_square_state_) adjustYaw(rotation_step_deg_);
            last_square_state_ = square;
        }

        if (msg.buttons.size() > 1) {
            bool circle = (msg.buttons[1] == 1);
            if (circle && !last_circle_state_) adjustYaw(-rotation_step_deg_);
            last_circle_state_ = circle;
        }

        // === 5. D-PAD (Discreet) ===
        if (msg.axes.size() > 7) {
            if (msg.axes[7] > 0.6 && !last_dpad_up_) { moveCartesian(0, step_size_, 0); last_dpad_up_ = true; }
            else if (msg.axes[7] < -0.6 && !last_dpad_down_) { moveCartesian(0, -step_size_, 0); last_dpad_down_ = true; }
            else if (std::abs(msg.axes[7]) < 0.2) { last_dpad_up_ = false; last_dpad_down_ = false; }
        }

        if (msg.axes.size() > 6) {
            if (msg.axes[6] > 0.6 && !last_dpad_right_) { moveCartesian(step_size_, 0, 0); last_dpad_right_ = true; }
            else if (msg.axes[6] < -0.6 && !last_dpad_left_) { moveCartesian(-step_size_, 0, 0); last_dpad_left_ = true; }
            else if (std::abs(msg.axes[6]) < 0.2) { last_dpad_left_ = false; last_dpad_right_ = false; }
        }

        // === 6. ANALOGE STICKS BEDIENING (Met Edge Triggers) ===
        // Linker Stick X (As 0) & Y (As 1)
        if (msg.axes.size() > 1) {
            // X-as
            if (msg.axes[0] < -STICK_DEADZONE_ && last_stick_lx_ != 1) {
                moveCartesian(step_size_, 0, 0); last_stick_lx_ = 1;
            } else if (msg.axes[0] > STICK_DEADZONE_ && last_stick_lx_ != -1) {
                moveCartesian(-step_size_, 0, 0); last_stick_lx_ = -1;
            } else if (std::abs(msg.axes[0]) < 0.3) { last_stick_lx_ = 0; }

            // Y-as
            if (msg.axes[1] > STICK_DEADZONE_ && last_stick_ly_ != 1) {
                moveCartesian(0, step_size_, 0); last_stick_ly_ = 1;
            } else if (msg.axes[1] < -STICK_DEADZONE_ && last_stick_ly_ != -1) {
                moveCartesian(0, -step_size_, 0); last_stick_ly_ = -1;
            } else if (std::abs(msg.axes[1]) < 0.3) { last_stick_ly_ = 0; }
        }

        // Triggers Z-as (L2 = As 2, R2 = As 5)
        if (msg.axes.size() > 5) {
            if (msg.axes[5] < -0.6 && !last_trigger_r2_) { // R2 = Z omhoog
                moveCartesian(0, 0, step_size_); last_trigger_r2_ = true;
            } else if (msg.axes[5] > -0.2) { last_trigger_r2_ = false; }

            if (msg.axes[2] < -0.6 && !last_trigger_l2_) { // L2 = Z omlaag
                moveCartesian(0, 0, -step_size_); last_trigger_l2_ = true;
            } else if (msg.axes[2] > -0.2) { last_trigger_l2_ = false; }
        }

        // Rechter Stick Pitch (As 4) & Roll (As 3)
        if (msg.axes.size() > 4) {
            if (msg.axes[4] > STICK_DEADZONE_ && last_stick_ry_ != 1) {
                adjustPitchRoll(0, rotation_step_deg_); last_stick_ry_ = 1;
            } else if (msg.axes[4] < -STICK_DEADZONE_ && last_stick_ry_ != -1) {
                adjustPitchRoll(0, -rotation_step_deg_); last_stick_ry_ = -1;
            } else if (std::abs(msg.axes[4]) < 0.3) { last_stick_ry_ = 0; }

            if (msg.axes[3] > STICK_DEADZONE_ && last_stick_rx_ != 1) {
                adjustPitchRoll(-rotation_step_deg_, 0); last_stick_rx_ = 1;
            } else if (msg.axes[3] < -STICK_DEADZONE_ && last_stick_rx_ != -1) {
                adjustPitchRoll(rotation_step_deg_, 0); last_stick_rx_ = -1;
            } else if (std::abs(msg.axes[3]) < 0.3) { last_stick_rx_ = 0; }
        }
    }

    void resetStates()
    {
        last_triangle_state_ = false; last_circle_state_ = false;
        last_square_state_ = false; last_cross_state_ = false;
        last_dpad_up_ = false; last_dpad_down_ = false;
        last_dpad_left_ = false; last_dpad_right_ = false;
        last_stick_lx_ = 0; last_stick_ly_ = 0;
        last_stick_rx_ = 0; last_stick_ry_ = 0;
        last_trigger_l2_ = false; last_trigger_r2_ = false;
    }

    bool goToNamedTarget(const std::string &name)
    {
        arm_->setStartStateToCurrentState();
        arm_->setNamedTarget(name);
        MoveGroupInterface::Plan plan;
        if (arm_->plan(plan) == moveit::core::MoveItErrorCode::SUCCESS) {
            return (arm_->execute(plan) == moveit::core::MoveItErrorCode::SUCCESS);
        }
        return false;
    }

    void moveCartesian(double dx, double dy, double dz)
    {
        is_moving_ = true;
        geometry_msgs::msg::Pose target_pose;
        try { target_pose = arm_->getCurrentPose().pose; }
        catch (...) { is_moving_ = false; return; }

        target_pose.position.x += dx;
        target_pose.position.y += dy;
        target_pose.position.z += dz;

        executeCartesianMove(target_pose);
        is_moving_ = false;
    }

    void adjustYaw(double degrees)
    {
        is_moving_ = true;
        geometry_msgs::msg::Pose target_pose;
        try { target_pose = arm_->getCurrentPose().pose; }
        catch (...) { is_moving_ = false; return; }

        tf2::Quaternion q_current, q_rot, q_new;
        tf2::fromMsg(target_pose.orientation, q_current);
        q_rot.setRPY(0, 0, degrees * M_PI / 180.0);
        q_new = q_rot * q_current;
        q_new.normalize();
        target_pose.orientation = tf2::toMsg(q_new);

        executeCartesianMove(target_pose);
        is_moving_ = false;
    }

    void adjustPitchRoll(double roll_deg, double pitch_deg)
    {
        is_moving_ = true;
        geometry_msgs::msg::Pose target_pose;
        try { target_pose = arm_->getCurrentPose().pose; }
        catch (...) { is_moving_ = false; return; }

        tf2::Quaternion q_current, q_rot, q_new;
        tf2::fromMsg(target_pose.orientation, q_current);
        q_rot.setRPY(roll_deg * M_PI / 180.0, pitch_deg * M_PI / 180.0, 0);
        q_new = q_rot * q_current;
        q_new.normalize();
        target_pose.orientation = tf2::toMsg(q_new);

        executeCartesianMove(target_pose);
        is_moving_ = false;
    }

    void executeCartesianMove(const geometry_msgs::msg::Pose &target_pose)
    {
        std::vector<geometry_msgs::msg::Pose> waypoints{target_pose};
        moveit_msgs::msg::RobotTrajectory trajectory;
        
        double fraction = arm_->computeCartesianPath(waypoints, 0.005, 0.0, trajectory);

        if (fraction >= 0.85) {
            // SYNC execution: Wacht netjes tot de stap klaar is voor een volgende input mag komen
            arm_->execute(trajectory);

            geometry_msgs::msg::PoseStamped st_pose;
            st_pose.header.stamp = this->now();
            st_pose.header.frame_id = arm_->getPlanningFrame();
            st_pose.pose = target_pose;
            pose_pub_->publish(st_pose);
        } else {
            RCLCPP_WARN(this->get_logger(), "Cartesisch pad niet haalbaar (fraction: %.2f)", fraction);
        }
    }

    std::shared_ptr<MoveGroupInterface> arm_;
    rclcpp::Subscription<Joy>::SharedPtr joy_sub_;
    rclcpp::Publisher<geometry_msgs::msg::PoseStamped>::SharedPtr pose_pub_;
    rclcpp::CallbackGroup::SharedPtr cb_group_;

    bool is_woken_up_;
    bool deadman_active_;
    bool last_deadman_state_;
    bool is_moving_;

    bool last_triangle_state_, last_circle_state_, last_square_state_, last_cross_state_;
    bool last_dpad_up_, last_dpad_down_, last_dpad_left_, last_dpad_right_;
    
    int last_stick_lx_, last_stick_ly_;
    int last_stick_rx_, last_stick_ry_;
    bool last_trigger_l2_, last_trigger_r2_;

    double step_size_;
    double rotation_step_deg_;
    double STICK_DEADZONE_;
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<MoveArmJoy>();

    rclcpp::executors::MultiThreadedExecutor executor;
    executor.add_node(node);
    executor.spin();

    rclcpp::shutdown();
    return 0;
}
