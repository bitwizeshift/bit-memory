cmake_minimum_required(VERSION 2.6.3)

include("${CMAKE_CURRENT_LIST_DIR}/CopyTargetProperties.cmake")

#.rst:
# CopyTarget
# ---------
#
# Copies a list of properties from a given target to another target
#
# ::
#
#     copy_target( <target> <from-target> <visibility> )
#
#     <target>      - The target to copy properties to
#     <from-target> - The target to copy properties from
#     <visibility>  - The visibility
#
macro(copy_target to_target from_target visibility)

  if( NOT TARGET "${to_target}")
    message(FATAL_ERROR "copy_target: '${to_target}' is not a valid target")
  endif()

  if( NOT TARGET "${from_target}")
    message(FATAL_ERROR "copy_target: '${from_target}' is not a valid target")
  endif()

  if( NOT visibility STREQUAL "INTERFACE" AND
      NOT visibility STREQUAL "PUBLIC" AND
      NOT visibility STREQUAL "PRIVATE")
    message(FATAL_ERROR "copy_target: invalid visibility, ${visibility}")
  endif()

  if( visibility STREQUAL "INTERFACE" OR visibility STREQUAL "PUBLIC" )

    copy_target_properties(${to_target} ${from_target} PROPERTIES
      INTERFACE_INCLUDE_DIRECTORIES
      INTERFACE_SYSTEM_INCLUDE_DIRECTORIES
      INTERFACE_COMPILE_OPTIONS
      INTERFACE_COMPILE_DEFINITIONS
      INTERFACE_COMPILE_FEATURES
      INTERFACE_LINK_LIBRARIES
      INTERFACE_SOURCES
    )

  endif()

  if( visibility STREQUAL "PRIVATE" OR visibility STREQUAL "PUBLIC" )

    copy_target_properties(${to_target} ${from_target} PROPERTIES
      INCLUDE_DIRECTORIES
      SYSTEM_INCLUDE_DIRECTORIES
      COMPILE_OPTIONS
      COMPILE_DEFINITIONS
      COMPILE_FEATURES
      LINK_LIBRARIES
      SOURCES
    )

  endif()

endmacro()
