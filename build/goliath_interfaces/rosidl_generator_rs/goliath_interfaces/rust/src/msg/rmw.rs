#[cfg(feature = "serde")]
use serde::{Deserialize, Serialize};


#[link(name = "goliath_interfaces__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__goliath_interfaces__msg__PoseCommand() -> *const std::ffi::c_void;
}

#[link(name = "goliath_interfaces__rosidl_generator_c")]
extern "C" {
    fn goliath_interfaces__msg__PoseCommand__init(msg: *mut PoseCommand) -> bool;
    fn goliath_interfaces__msg__PoseCommand__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<PoseCommand>, size: usize) -> bool;
    fn goliath_interfaces__msg__PoseCommand__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<PoseCommand>);
    fn goliath_interfaces__msg__PoseCommand__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<PoseCommand>, out_seq: *mut rosidl_runtime_rs::Sequence<PoseCommand>) -> bool;
}

// Corresponds to goliath_interfaces__msg__PoseCommand
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]


// This struct is not documented.
#[allow(missing_docs)]

#[repr(C)]
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
    unsafe {
      let mut msg = std::mem::zeroed();
      if !goliath_interfaces__msg__PoseCommand__init(&mut msg as *mut _) {
        panic!("Call to goliath_interfaces__msg__PoseCommand__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for PoseCommand {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { goliath_interfaces__msg__PoseCommand__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { goliath_interfaces__msg__PoseCommand__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { goliath_interfaces__msg__PoseCommand__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for PoseCommand {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for PoseCommand where Self: Sized {
  const TYPE_NAME: &'static str = "goliath_interfaces/msg/PoseCommand";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__goliath_interfaces__msg__PoseCommand() }
  }
}


