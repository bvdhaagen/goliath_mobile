#include <rclcpp/rclcpp.hpp>
#include <moveit/move_group_interface/move_group_interface.h>
#include <example_interfaces/msg/bool.hpp>
#include <example_interfaces/msg/float64_multi_array.hpp>
#include <goliath_interfaces/msg/pose_command.hpp>
#include <tf2/LinearMath/Quaternion.h>
#include <moveit_msgs/msg/move_it_error_codes.hpp>
#include <std_msgs/msg/string.hpp>
#include <sensor_msgs/msg/joy.hpp>

using MoveGroupInterface = moveit::planning_interface::MoveGroupInterface;
using Bool = example_interfaces::msg::Bool;
using FloatArray = example_interfaces::msg::Float64MultiArray;
using PoseCmd = goliath_interfaces::msg::PoseCommand;
using String = std_msgs::msg::String;
using Joy = sensor_msgs::msg::Joy;
using namespace std::placeholders;

class Commander
{
public:
    Commander(std::shared_ptr<rclcpp::Node> node)
    {
        node_ = node;
        arm_ = std::make_shared<MoveGroupInterface>(node_, "position");
        arm_->setMaxVelocityScalingFactor(1.0);
        arm_->setMaxAccelerationScalingFactor(1.0);

        joint_cmd_sub_ = node_->create_subscription<FloatArray>(
            "joint_command", 10, std::bind(&Commander::jointCmdCallback, this, _1));

        pose_cmd_sub_ = node_->create_subscription<PoseCmd>(
            "pose_command", 10, std::bind(&Commander::poseCmdCallback, this, _1));
        
        named_pose_sub_ = node_->create_subscription<String>(
            "named_pose_command", 10, std::bind(&Commander::namedPoseCallback, this, _1));
        
        // Joy subscriber voor PS5 controller
        joy_sub_ = node_->create_subscription<Joy>(
            "joy", 10, std::bind(&Commander::joyCallback, this, _1));
        
        last_wake_up_button_state_ = false;
    }

    void goToNamedTarget(const std::string &name)
    {
        RCLCPP_INFO(node_->get_logger(), "Going to named target: %s", name.c_str());
        arm_->setStartStateToCurrentState();
        arm_->setNamedTarget(name);
        planAndExecute(arm_);
    }

    void goToJointTarget(const std::vector<double> &joints)
    {
        arm_->setStartStateToCurrentState();
        arm_->setJointValueTarget(joints);
        planAndExecute(arm_);
    }

    void goToPoseTarget(double x, double y, double z, 
                        double roll, double pitch, double yaw, bool cartesian_path=false)
    {
        tf2::Quaternion q;
        q.setRPY(roll, pitch, yaw);
        q = q.normalize();

        geometry_msgs::msg::PoseStamped target_pose;
        target_pose.header.frame_id = "base_link";
        target_pose.pose.position.x = x;
        target_pose.pose.position.y = y;
        target_pose.pose.position.z = z;
        target_pose.pose.orientation.x = q.getX();
        target_pose.pose.orientation.y = q.getY();
        target_pose.pose.orientation.z = q.getZ();
        target_pose.pose.orientation.w = q.getW();

        arm_->setStartStateToCurrentState();

        if (!cartesian_path) {
            arm_->setPoseTarget(target_pose);
            planAndExecute(arm_);
        }
        else {
            std::vector<geometry_msgs::msg::Pose> waypoints;
            waypoints.push_back(target_pose.pose);
            moveit_msgs::msg::RobotTrajectory trajectory;
            
            double jump_threshold = 0.0;
            double eef_step = 0.01;
            bool avoid_collisions = true;
            moveit_msgs::msg::MoveItErrorCodes error_code;

            double fraction = arm_->computeCartesianPath(
                waypoints, 
                eef_step, 
                jump_threshold, 
                trajectory,
                avoid_collisions,
                &error_code
            );

            if (fraction >= 0.9) {
                arm_->execute(trajectory);
                RCLCPP_INFO(node_->get_logger(), "Cartesian path executed successfully (fraction: %.2f%%)", fraction * 100.0);
            } else {
                RCLCPP_WARN(node_->get_logger(), "Cartesian path failed with fraction: %.2f%%", fraction * 100.0);
                RCLCPP_WARN(node_->get_logger(), "Error code: %d", error_code.val);
            }
        }
    }

private:

    void planAndExecute(const std::shared_ptr<MoveGroupInterface> &interface)
    {
        MoveGroupInterface::Plan plan;
        bool success = (interface->plan(plan) == moveit::core::MoveItErrorCode::SUCCESS);

        if (success) {
            interface->execute(plan);
            RCLCPP_INFO(node_->get_logger(), "Plan executed successfully");
        } else {
            RCLCPP_WARN(node_->get_logger(), "Planning failed");
        }
    }

    void jointCmdCallback(const FloatArray &msg)
    {
        auto joints = msg.data;

        if (joints.size() == 6) {
            goToJointTarget(joints);
        } else {
            RCLCPP_WARN(node_->get_logger(), "Expected 6 joints, got %zu", joints.size());
        }
    }

    void poseCmdCallback(const PoseCmd &msg)
    {
        goToPoseTarget(msg.x, msg.y, msg.z, msg.roll, msg.pitch, msg.yaw, msg.cartesian_path);   
    }

    void namedPoseCallback(const String &msg)
    {
        RCLCPP_INFO(node_->get_logger(), "Going to named pose: %s", msg.data.c_str());
        goToNamedTarget(msg.data);
    }

    // Joy callback voor PS5 controller
    void joyCallback(const Joy &msg)
    {
        // Check of de Triangle knop (index 2) is ingedrukt
        // Alleen triggeren als de knop NET is ingedrukt (niet vastgehouden)
        if (msg.buttons.size() > 2) {
            bool triangle_pressed = (msg.buttons[2] == 1);
            
            if (triangle_pressed && !last_wake_up_button_state_) {
                RCLCPP_INFO(node_->get_logger(), "Triangle button pressed! Going to wake_up pose");
                goToNamedTarget("wake_up");
            }
            
            last_wake_up_button_state_ = triangle_pressed;
        }
    }

    std::shared_ptr<rclcpp::Node> node_;
    std::shared_ptr<MoveGroupInterface> arm_;
    std::shared_ptr<MoveGroupInterface> gripper_;

    rclcpp::Subscription<Bool>::SharedPtr open_gripper_sub_;
    rclcpp::Subscription<FloatArray>::SharedPtr joint_cmd_sub_;
    rclcpp::Subscription<PoseCmd>::SharedPtr pose_cmd_sub_;
    rclcpp::Subscription<String>::SharedPtr named_pose_sub_;
    rclcpp::Subscription<Joy>::SharedPtr joy_sub_;
    
    bool last_wake_up_button_state_;
};


int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<rclcpp::Node>("commander");
    auto commander = Commander(node);
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
