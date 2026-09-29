#include <rclcpp/rclcpp.hpp>
#include <moveit/move_group_interface/move_group_interface.h>
#include <moveit_msgs/msg/move_it_error_codes.hpp>

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<rclcpp::Node>("test_moveit");
    
    auto arm = moveit::planning_interface::MoveGroupInterface(node, "arm");
    
    // Example Cartesian path
    geometry_msgs::msg::Pose target_pose;
    target_pose.position.x = 0.5;
    target_pose.position.y = 0.0;
    target_pose.position.z = 0.5;
    target_pose.orientation.w = 1.0;
    
    std::vector<geometry_msgs::msg::Pose> waypoints;
    waypoints.push_back(target_pose);
    
    moveit_msgs::msg::RobotTrajectory trajectory;
    
    // Fixed computeCartesianPath call
    double jump_threshold = 0.0;
    double eef_step = 0.01;
    bool avoid_collisions = true;
    moveit_msgs::msg::MoveItErrorCodes error_code;
    
    double fraction = arm.computeCartesianPath(
        waypoints, 
        eef_step, 
        jump_threshold, 
        trajectory,
        avoid_collisions,
        &error_code
    );
    
    if (fraction >= 0.9) {
        arm.execute(trajectory);
        RCLCPP_INFO(node->get_logger(), "Cartesian path succeeded with fraction: %.2f%%", fraction * 100.0);
    } else {
        RCLCPP_WARN(node->get_logger(), "Cartesian path failed with fraction: %.2f%%", fraction * 100.0);
        RCLCPP_WARN(node->get_logger(), "Error code: %d", error_code.val);
    }
    
    rclcpp::shutdown();
    return 0;
}
