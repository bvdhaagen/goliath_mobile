cmake_minimum_required(VERSION 3.8)
project(arm_move_joy)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(tf2 REQUIRED)
find_package(tf2_geometry_msgs REQUIRED)
find_package(moveit_ros_planning_interface REQUIRED)

add_executable(move_arm_joy src/move_arm_joy.cpp)

target_include_directories(move_arm_joy PUBLIC
  ${moveit_ros_planning_interface_INCLUDE_DIRS}
)

target_link_libraries(move_arm_joy
  ${moveit_ros_planning_interface_LIBRARIES}
  ${rclcpp_LIBRARIES}
  ${tf2_LIBRARIES}
)

ament_target_dependencies(move_arm_joy
  rclcpp
  sensor_msgs
  geometry_msgs
  tf2
  tf2_geometry_msgs
  moveit_ros_planning_interface
)

install(TARGETS
  move_arm_joy
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()
