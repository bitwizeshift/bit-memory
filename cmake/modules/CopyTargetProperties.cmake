cmake_minimum_required(VERSION 2.6.3)

include(CMakeParseArguments)

#.rst:
# CopyTargetProperties
# ---------
#
# Copies a list of properties from a given target to another target
#
# ::
#
#     copy_target_properties( <target> <from-target> PROPERTIES [properties]... )
#
#     <target>        - The target to copy properties to
#     <from-target    - The target to copy properties from
#     [properties]... - List of properties to copy
#
macro(copy_target_properties to_target from_target)

  cmake_parse_arguments("COPY" "" "" "PROPERTIES" ${ARGN})

  if( NOT TARGET "${to_target}")
    message(FATAL_ERROR "copy_target_properties: '${to_target}' is not a valid target")
  endif()

  if( NOT TARGET "${from_target}")
    message(FATAL_ERROR "copy_target_properties: '${from_target}' is not a valid target")
  endif()

  if( NOT COPY_PROPERTIES )
    message(FATAL_ERROR "copy_target_properties: PROPERTIES not specified")
  endif()

  if( COPY_UNPARSED_ARGUMENTS )
    message(FATAL_ERROR "copy_target_properties: Unknown arguments: ${COPY_UNPARSED_ARGUMENTS}")
  endif()

  foreach( prop ${COPY_PROPERTIES} )

    get_property(prop_val TARGET ${from_target} PROPERTY ${prop})
    set_target_properties(${to_target} PROPERTIES "${prop}" "${prop_val}")

  endforeach()

  unset(COPY_PROPERTIES)

endmacro()
