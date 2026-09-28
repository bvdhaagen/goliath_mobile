#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "gripper_controller::gripper_controller" for configuration ""
set_property(TARGET gripper_controller::gripper_controller APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(gripper_controller::gripper_controller PROPERTIES
  IMPORTED_LINK_DEPENDENT_LIBRARIES_NOCONFIG "Boost::thread"
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libgripper_controller.so"
  IMPORTED_SONAME_NOCONFIG "libgripper_controller.so"
  )

list(APPEND _IMPORT_CHECK_TARGETS gripper_controller::gripper_controller )
list(APPEND _IMPORT_CHECK_FILES_FOR_gripper_controller::gripper_controller "${_IMPORT_PREFIX}/lib/libgripper_controller.so" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
