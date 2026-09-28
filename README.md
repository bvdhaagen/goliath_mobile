

![Screencastfrom05-26-2025031019PM-ezgif com-video-to-gif-converter](https://github.com/user-attachments/assets/ff9e2ffa-efec-4e6e-8547-04a1040732de)


# goliath_mobile
Prerequisites & Dependencies

Make sure you are running Ubuntu 22.04 LTS with ROS 2 Humble installed.

Required system packages:

sudo apt update && sudo apt install -y \
  ros-humble-ros2-control \
  ros-humble-ros2-controllers \
  ros-humble-moveit \
  ros-humble-nav2-bringup \
  ros-humble-joint-state-publisher-gui \
  ros-humble-xacro \
  python3-colcon-common-extensions \
  python3-rosdep

Step-by-Step Installation1.Set Up workspace Directory:
Ensure environment is clean before cloning.

  mkdir ~/goliath_ws
  cd ~/goliath_ws
  

Clone your Goliath packages (description, bringup, moveit_config, and hardware driver nodes)

  git clone -b humble https://github.com/bvdhaagen/goliath_mobile.git
  cd ~/goliath_ws

Install deps   

  sudo rosdep init # Run only if rosdep hasn't been initialized yet
  rosdep update
  rosdep install --from-paths src --ignore-src -y -r

Build workspace

  source /opt/ros/humble/setup.bash
  colcon build --symlink-install

Source your workspace 
  
  source ~/goliath_ws/install/setup.bash

  
Launch Full Bringup (Hardware + MoveIt 2)

  ros2 launch goliath_bringup goliath_bringup.launch.py


  
  
