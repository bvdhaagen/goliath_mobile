// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from goliath_interfaces:msg/PoseCommand.idl
// generated code does not contain a copyright notice

#ifndef GOLIATH_INTERFACES__MSG__DETAIL__POSE_COMMAND__BUILDER_HPP_
#define GOLIATH_INTERFACES__MSG__DETAIL__POSE_COMMAND__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "goliath_interfaces/msg/detail/pose_command__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace goliath_interfaces
{

namespace msg
{

namespace builder
{

class Init_PoseCommand_cartesian_path
{
public:
  explicit Init_PoseCommand_cartesian_path(::goliath_interfaces::msg::PoseCommand & msg)
  : msg_(msg)
  {}
  ::goliath_interfaces::msg::PoseCommand cartesian_path(::goliath_interfaces::msg::PoseCommand::_cartesian_path_type arg)
  {
    msg_.cartesian_path = std::move(arg);
    return std::move(msg_);
  }

private:
  ::goliath_interfaces::msg::PoseCommand msg_;
};

class Init_PoseCommand_yaw
{
public:
  explicit Init_PoseCommand_yaw(::goliath_interfaces::msg::PoseCommand & msg)
  : msg_(msg)
  {}
  Init_PoseCommand_cartesian_path yaw(::goliath_interfaces::msg::PoseCommand::_yaw_type arg)
  {
    msg_.yaw = std::move(arg);
    return Init_PoseCommand_cartesian_path(msg_);
  }

private:
  ::goliath_interfaces::msg::PoseCommand msg_;
};

class Init_PoseCommand_pitch
{
public:
  explicit Init_PoseCommand_pitch(::goliath_interfaces::msg::PoseCommand & msg)
  : msg_(msg)
  {}
  Init_PoseCommand_yaw pitch(::goliath_interfaces::msg::PoseCommand::_pitch_type arg)
  {
    msg_.pitch = std::move(arg);
    return Init_PoseCommand_yaw(msg_);
  }

private:
  ::goliath_interfaces::msg::PoseCommand msg_;
};

class Init_PoseCommand_roll
{
public:
  explicit Init_PoseCommand_roll(::goliath_interfaces::msg::PoseCommand & msg)
  : msg_(msg)
  {}
  Init_PoseCommand_pitch roll(::goliath_interfaces::msg::PoseCommand::_roll_type arg)
  {
    msg_.roll = std::move(arg);
    return Init_PoseCommand_pitch(msg_);
  }

private:
  ::goliath_interfaces::msg::PoseCommand msg_;
};

class Init_PoseCommand_z
{
public:
  explicit Init_PoseCommand_z(::goliath_interfaces::msg::PoseCommand & msg)
  : msg_(msg)
  {}
  Init_PoseCommand_roll z(::goliath_interfaces::msg::PoseCommand::_z_type arg)
  {
    msg_.z = std::move(arg);
    return Init_PoseCommand_roll(msg_);
  }

private:
  ::goliath_interfaces::msg::PoseCommand msg_;
};

class Init_PoseCommand_y
{
public:
  explicit Init_PoseCommand_y(::goliath_interfaces::msg::PoseCommand & msg)
  : msg_(msg)
  {}
  Init_PoseCommand_z y(::goliath_interfaces::msg::PoseCommand::_y_type arg)
  {
    msg_.y = std::move(arg);
    return Init_PoseCommand_z(msg_);
  }

private:
  ::goliath_interfaces::msg::PoseCommand msg_;
};

class Init_PoseCommand_x
{
public:
  Init_PoseCommand_x()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_PoseCommand_y x(::goliath_interfaces::msg::PoseCommand::_x_type arg)
  {
    msg_.x = std::move(arg);
    return Init_PoseCommand_y(msg_);
  }

private:
  ::goliath_interfaces::msg::PoseCommand msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::goliath_interfaces::msg::PoseCommand>()
{
  return goliath_interfaces::msg::builder::Init_PoseCommand_x();
}

}  // namespace goliath_interfaces

#endif  // GOLIATH_INTERFACES__MSG__DETAIL__POSE_COMMAND__BUILDER_HPP_
