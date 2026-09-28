#[cfg(feature = "serde")]
use serde::{Deserialize, Serialize};



// Corresponds to goliath_interfaces__msg__PoseCommand

// This struct is not documented.
#[allow(missing_docs)]

#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct PoseCommand {

    // This member is not documented.
    #[allow(missing_docs)]
    pub x: f64,


    // This member is not documented.
    #[allow(missing_docs)]
    pub y: f64,


    // This member is not documented.
    #[allow(missing_docs)]
    pub z: f64,


    // This member is not documented.
    #[allow(missing_docs)]
    pub roll: f64,


    // This member is not documented.
    #[allow(missing_docs)]
    pub pitch: f64,


    // This member is not documented.
    #[allow(missing_docs)]
    pub yaw: f64,


    // This member is not documented.
    #[allow(missing_docs)]
    pub cartesian_path: bool,

}



impl Default for PoseCommand {
  fn default() -> Self {
    <Self as rosidl_runtime_rs::Message>::from_rmw_message(super::msg::rmw::PoseCommand::default())
  }
}

impl rosidl_runtime_rs::Message for PoseCommand {
  type RmwMsg = super::msg::rmw::PoseCommand;

  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> {
    match msg_cow {
      std::borrow::Cow::Owned(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        x: msg.x,
        y: msg.y,
        z: msg.z,
        roll: msg.roll,
        pitch: msg.pitch,
        yaw: msg.yaw,
        cartesian_path: msg.cartesian_path,
      }),
      std::borrow::Cow::Borrowed(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
      x: msg.x,
      y: msg.y,
      z: msg.z,
      roll: msg.roll,
      pitch: msg.pitch,
      yaw: msg.yaw,
      cartesian_path: msg.cartesian_path,
      })
    }
  }

  fn from_rmw_message(msg: Self::RmwMsg) -> Self {
    Self {
      x: msg.x,
      y: msg.y,
      z: msg.z,
      roll: msg.roll,
      pitch: msg.pitch,
      yaw: msg.yaw,
      cartesian_path: msg.cartesian_path,
    }
  }
}


