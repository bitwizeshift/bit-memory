cmake_minimum_required(VERSION 3.1.3)

include(CMakeParseArguments)

#.rst:
# MakeVersionHeader
# ---------
#
# Makes a version header
#
# ::
#
#     make_version_header( <output path>
#                          MAJOR <major>
#                          MINOR <minor>
#                          PATCH <patch>
#                          BUILD <build>
#                          PREFIX <prefix>
#                         [SUFFIX <suffix>]
#                         [TAG <tag>] )
#
#     <output path>   - The path to output the header
#     MAJOR <major>   - The major version number
#     MINOR <minor>   - The minor version number
#     PATCH <patch>   - The patch version number
#     BUILD <build>   - The build number (optional)
#     PREFIX <prefix> - The prefix for version variables
#     SUFFIX <suffix> - The version suffix
#     TAG <tag>       - A tag identifier for the current build flow. Often
#                       a branch name
#
macro(make_version_header output_path )

  set(__single_args MAJOR MINOR PATCH BUILD PREFIX SUFFIX TAG)
  cmake_parse_arguments("VERSION" "" "${__single_args}" "" "${ARGN}")

  ############################ Default Arguments #############################

  set(MAJOR_VERSION  ${VERSION_MAJOR})
  set(MINOR_VERSION  ${VERSION_MINOR})
  set(PATCH_VERSION  ${VERSION_PATCH})
  set(BUILD_NUMBER   ${VERSION_BUILD})
  set(PREFIX         "${VERSION_PREFIX}")
  set(TAG_VERSION    "${VERSION_TAG}")
  set(VERSION_SUFFIX "${VERSION_SUFFIX}")
  set(VERSION_STRING "")
  set(FULL_VERSION_STRING "")


  ######################## Validate Required Arguments #######################

  if( NOT MAJOR_VERSION MATCHES "[0-9]+")
    message(FATAL_ERROR "make_version_header: Incorrect major version. Expected numeric entry, received '${MAJOR_VERSION}'")
  endif()

  if( NOT MINOR_VERSION MATCHES "[0-9]+")
    message(FATAL_ERROR "make_version_header: Incorrect minor version. Expected numeric entry, received '${MINOR_VERSION}'")
  endif()

  if( NOT "${PATCH_VERSION}" STREQUAL "" AND NOT PATCH_VERSION MATCHES "[0-9]+")
    message(FATAL_ERROR "make_version_header: Incorrect patch version. Expected numeric entry, received '${PATCH_VERSION}'")
  endif()

  if( NOT "${BUILD_NUMBER}" STREQUAL "" AND NOT BUILD_NUMBER MATCHES "[0-9]+")
    message(FATAL_ERROR "make_version_header: Incorrect build number. Expected numeric entry, received '${BUILD_NUMBER}'")
  endif()

  ############################ Template Generation ###########################

  set(template_file "${CMAKE_BINARY_DIR}/MakeVersionHeader/version.in")
  if( NOT EXISTS "${template_file}" )
    file(WRITE "${template_file}"
"
/// Major version of this library
#ifndef @PREFIX@VERSION_MAJOR
# define @PREFIX@VERSION_MAJOR        @MAJOR_VERSION@
#endif

/// Minor version of this library
#ifndef @PREFIX@VERSION_MINOR
# define @PREFIX@VERSION_MINOR        @MINOR_VERSION@
#endif

/// Patch version of this library
#ifndef @PREFIX@VERSION_PATCH
# define @PREFIX@VERSION_PATCH        @PATCH_VERSION@
#endif

/// The version suffix
#ifndef @PREFIX@VERSION_SUFFIX
# define @PREFIX@VERSION_SUFFIX       @VERSION_SUFFIX@
#endif

/// The current build number
#ifndef @PREFIX@BUILD_NUMBER
# define @PREFIX@BUILD_NUMBER         @BUILD_NUMBER@
#endif

/// The tag name for this (normally a branch name)
#ifndef @PREFIX@VERSION_TAG
# define @PREFIX@VERSION_TAG          \"@TAG_VERSION@\"
#endif

/// The version string (major.minor.patch)
#ifndef @PREFIX@VERSION_STRING
# define @PREFIX@VERSION_STRING       \"@VERSION_STRING@\"
#endif

/// The full version string (major.minor.patch (tag build))
#ifndef @PREFIX@VERSION_STRING_FULL
# define @PREFIX@VERSION_STRING_FULL  \"@FULL_VERSION_STRING@\"
#endif
")
  endif()

  ############################### Configuring ################################

  if(PREFIX)
    string(TOUPPER "${PREFIX}" PREFIX)
    set(PREFIX "${PREFIX}_")
  endif()

  set(VERSION_STRING "${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION}")

  if( NOT "${TAG_VERSION}" STREQUAL "" )
    set(FULL_VERSION_STRING "${VERSION_STRING}${VERSION_SUFFIX} (${TAG_VERSION} ${BUILD_NUMBER})")
  else()
    set(FULL_VERSION_STRING "${VERSION_STRING}${VERSION_SUFFIX}")
  endif()

  configure_file("${template_file}" "${output_path}" @ONLY)

endmacro()
