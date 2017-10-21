cmake_minimum_required(VERSION 3.1)


include(CMakeParseArguments)

set(_ExportLibrary_DIR ${CMAKE_CURRENT_LIST_DIR})

#.rst:
# ExportLibrary
# ---------
#
# Exports library targets of the specified version with the given
# namespace.
#
# ::
#
#     export_library( TARGETS <targets>...
#                     PACKAGE <package>
#                     VERSION <version>
#                     [MAJOR_VERSION <major>]
#                     [MINOR_VERSION <minor>]
#                     [INCLUDE_PATHS <paths>...]
#                     [NAMESPACE <namespace>
#                     [PREFIX_PACKAGE] )
#
#     <targets>...    - Targets to export
#     <package>       - The name of the package
#     <version>       - The version to export
#     <major>         - The major version
#     <minor>         - The minor version
#     <paths>...      - paths to include
#     <namespace>     - The namespace to export
#     PREFIX_PACKAGE  - Whether to prefix the export directory with
#                       the package name
#
function(export_library)

  set(option_args PREFIX_PACKAGE)
  set(single_args PACKAGE NAMESPACE VERSION MAJOR_VERSION MINOR_VERSION )
  set(multi_args TARGETS INCLUDE_PATHS)

  cmake_parse_arguments("EXPORT" "${option_args}" "${single_args}" "${multi_args}" ${ARGN} )

  if( "${EXPORT_TARGETS}" STREQUAL "" )
    message(FATAL_ERROR "export_library: No targets specified")
  endif()
  if( "${EXPORT_VERSION}" STREQUAL "")
    message(FATAL_ERROR "export_library: No version specified")
  endif()
  if( "${EXPORT_MAJOR_VERSION}" STREQUAL "")
    message(FATAL_ERROR "export_library: No major version number specified")
  endif()

  set(PACKAGE_NAME ${EXPORT_PACKAGE})
  set(PACKAGE_VERSION_MAJOR "${EXPORT_MAJOR_VERSION}")
  set(PACKAGE_VERSION_MINOR "${EXPORT_MINOR_VERSION}")
  set(PACKAGE_VERSION       "${EXPORT_VERSION}")

  configure_file( "${_ExportLibrary_DIR}/templates/PackageConfig.cmake.in"
                  "${CMAKE_CURRENT_BINARY_DIR}/${PACKAGE_NAME}Config.cmake"
                  @ONLY )

  configure_file( "${_ExportLibrary_DIR}/templates/PackageConfigVersion.cmake.in"
                  "${CMAKE_CURRENT_BINARY_DIR}/${PACKAGE_NAME}ConfigVersion.cmake"
                  @ONLY )

  set(PACKAGE_VERSION_MAJOR)
  set(PACKAGE_VERSION_MINOR)
  set(PACKAGE_VERSION)

  if( NOT EXPORT_MINOR_VERSION STREQUAL "" )
    set(target_dir "${PACKAGE_NAME}/${EXPORT_MAJOR_VERSION}.${EXPORT_MINOR_VERSION}")
  elseif( EXPORT_MAJOR_VERSION )
    set(target_dir "${PACKAGE_NAME}/${EXPORT_MAJOR_VERSION}")
  else()
    set(target_dir "${PACKAGE_NAME}")
  endif()

  set(prefix)
  if( EXPORT_PREFIX_PACKAGE )
    set(prefix ${target_dir}/)
  endif()

  install( DIRECTORY ${EXPORT_INCLUDE_PATHS}
           DESTINATION "${prefix}" )

  install( FILES ${CMAKE_CURRENT_BINARY_DIR}/${PACKAGE_NAME}ConfigVersion.cmake
                 ${CMAKE_CURRENT_BINARY_DIR}/${PACKAGE_NAME}Config.cmake
           DESTINATION "${prefix}cmake" )

  install( TARGETS "${EXPORT_TARGETS}"
           EXPORT "${PACKAGE_NAME}Targets"
           LIBRARY DESTINATION "${prefix}lib"
           ARCHIVE DESTINATION "${prefix}lib"
           RUNTIME DESTINATION "${prefix}bin"
           INCLUDES DESTINATION "${prefix}include" )

  if( EXPORT_NAMESPACE )
    set(namespace NAMESPACE ${EXPORT_NAMESPACE})
  endif()

  install( EXPORT "${PACKAGE_NAME}Targets"
           ${namespace}
           DESTINATION "${prefix}cmake"
           FILE "${PACKAGE_NAME}Targets.cmake" )

endfunction()
