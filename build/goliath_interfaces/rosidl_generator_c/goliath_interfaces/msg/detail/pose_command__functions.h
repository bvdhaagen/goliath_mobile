// generated from rosidl_generator_c/resource/idl__functions.h.em
// with input from goliath_interfaces:msg/PoseCommand.idl
// generated code does not contain a copyright notice

#ifndef GOLIATH_INTERFACES__MSG__DETAIL__POSE_COMMAND__FUNCTIONS_H_
#define GOLIATH_INTERFACES__MSG__DETAIL__POSE_COMMAND__FUNCTIONS_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stdlib.h>

#include "rosidl_runtime_c/visibility_control.h"
#include "goliath_interfaces/msg/rosidl_generator_c__visibility_control.h"

#include "goliath_interfaces/msg/detail/pose_command__struct.h"

/// Initialize msg/PoseCommand message.
/**
 * If the init function is called twice for the same message without
 * calling fini inbetween previously allocated memory will be leaked.
 * \param[in,out] msg The previously allocated message pointer.
 * Fields without a default value will not be initialized by this function.
 * You might want to call memset(msg, 0, sizeof(
 * goliath_interfaces__msg__PoseCommand
 * )) before or use
 * goliath_interfaces__msg__PoseCommand__create()
 * to allocate and initialize the message.
 * \return true if initialization was successful, otherwise false
 */
ROSIDL_GENERATOR_C_PUBLIC_goliath_interfaces
bool
goliath_interfaces__msg__PoseCommand__init(goliath_interfaces__msg__PoseCommand * msg);

/// Finalize msg/PoseCommand message.
/**
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_goliath_interfaces
void
goliath_interfaces__msg__PoseCommand__fini(goliath_interfaces__msg__PoseCommand * msg);

/// Create msg/PoseCommand message.
/**
 * It allocates the memory for the message, sets the memory to zero, and
 * calls
 * goliath_interfaces__msg__PoseCommand__init().
 * \return The pointer to the initialized message if successful,
 * otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_goliath_interfaces
goliath_interfaces__msg__PoseCommand *
goliath_interfaces__msg__PoseCommand__create();

/// Destroy msg/PoseCommand message.
/**
 * It calls
 * goliath_interfaces__msg__PoseCommand__fini()
 * and frees the memory of the message.
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_goliath_interfaces
void
goliath_interfaces__msg__PoseCommand__destroy(goliath_interfaces__msg__PoseCommand * msg);

/// Check for msg/PoseCommand message equality.
/**
 * \param[in] lhs The message on the left hand size of the equality operator.
 * \param[in] rhs The message on the right hand size of the equality operator.
 * \return true if messages are equal, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_goliath_interfaces
bool
goliath_interfaces__msg__PoseCommand__are_equal(const goliath_interfaces__msg__PoseCommand * lhs, const goliath_interfaces__msg__PoseCommand * rhs);

/// Copy a msg/PoseCommand message.
/**
 * This functions performs a deep copy, as opposed to the shallow copy that
 * plain assignment yields.
 *
 * \param[in] input The source message pointer.
 * \param[out] output The target message pointer, which must
 *   have been initialized before calling this function.
 * \return true if successful, or false if either pointer is null
 *   or memory allocation fails.
 */
ROSIDL_GENERATOR_C_PUBLIC_goliath_interfaces
bool
goliath_interfaces__msg__PoseCommand__copy(
  const goliath_interfaces__msg__PoseCommand * input,
  goliath_interfaces__msg__PoseCommand * output);

/// Initialize array of msg/PoseCommand messages.
/**
 * It allocates the memory for the number of elements and calls
 * goliath_interfaces__msg__PoseCommand__init()
 * for each element of the array.
 * \param[in,out] array The allocated array pointer.
 * \param[in] size The size / capacity of the array.
 * \return true if initialization was successful, otherwise false
 * If the array pointer is valid and the size is zero it is guaranteed
 # to return true.
 */
ROSIDL_GENERATOR_C_PUBLIC_goliath_interfaces
bool
goliath_interfaces__msg__PoseCommand__Sequence__init(goliath_interfaces__msg__PoseCommand__Sequence * array, size_t size);

/// Finalize array of msg/PoseCommand messages.
/**
 * It calls
 * goliath_interfaces__msg__PoseCommand__fini()
 * for each element of the array and frees the memory for the number of
 * elements.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_goliath_interfaces
void
goliath_interfaces__msg__PoseCommand__Sequence__fini(goliath_interfaces__msg__PoseCommand__Sequence * array);

/// Create array of msg/PoseCommand messages.
/**
 * It allocates the memory for the array and calls
 * goliath_interfaces__msg__PoseCommand__Sequence__init().
 * \param[in] size The size / capacity of the array.
 * \return The pointer to the initialized array if successful, otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_goliath_interfaces
goliath_interfaces__msg__PoseCommand__Sequence *
goliath_interfaces__msg__PoseCommand__Sequence__create(size_t size);

/// Destroy array of msg/PoseCommand messages.
/**
 * It calls
 * goliath_interfaces__msg__PoseCommand__Sequence__fini()
 * on the array,
 * and frees the memory of the array.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_goliath_interfaces
void
goliath_interfaces__msg__PoseCommand__Sequence__destroy(goliath_interfaces__msg__PoseCommand__Sequence * array);

/// Check for msg/PoseCommand message array equality.
/**
 * \param[in] lhs The message array on the left hand size of the equality operator.
 * \param[in] rhs The message array on the right hand size of the equality operator.
 * \return true if message arrays are equal in size and content, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_goliath_interfaces
bool
goliath_interfaces__msg__PoseCommand__Sequence__are_equal(const goliath_interfaces__msg__PoseCommand__Sequence * lhs, const goliath_interfaces__msg__PoseCommand__Sequence * rhs);

/// Copy an array of msg/PoseCommand messages.
/**
 * This functions performs a deep copy, as opposed to the shallow copy that
 * plain assignment yields.
 *
 * \param[in] input The source array pointer.
 * \param[out] output The target array pointer, which must
 *   have been initialized before calling this function.
 * \return true if successful, or false if either pointer
 *   is null or memory allocation fails.
 */
ROSIDL_GENERATOR_C_PUBLIC_goliath_interfaces
bool
goliath_interfaces__msg__PoseCommand__Sequence__copy(
  const goliath_interfaces__msg__PoseCommand__Sequence * input,
  goliath_interfaces__msg__PoseCommand__Sequence * output);

#ifdef __cplusplus
}
#endif

#endif  // GOLIATH_INTERFACES__MSG__DETAIL__POSE_COMMAND__FUNCTIONS_H_
