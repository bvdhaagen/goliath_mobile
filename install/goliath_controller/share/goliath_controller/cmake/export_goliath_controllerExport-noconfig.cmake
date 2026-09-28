#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "goliath_controller::goliath_controller" for configuration ""
set_property(TARGET goliath_controller::goliath_controller APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(goliath_controller::goliath_controller PROPERTIES
  IMPORTED_LINK_DEPENDENT_LIBRARIES_NOCONFIG "Boost::thread"
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libgoliath_controller.so"
  IMPORTED_SONAME_NOCONFIG "libgoliath_controller.so"
  )

list(APPEND _IMPORT_CHECK_TARGETS goliath_controller::goliath_controller )
list(APPEND _IMPORT_CHECK_FILES_FOR_goliath_controller::goliath_controller "${_IMPORT_PREFIX}/lib/libgoliath_controller.so" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
