#.rst:
# FindBit
# ---------
#
# Find Bit include dirs and libraries
#
# Use this module by invoking find_package with the form::
#
#   find_package(Bit
#     [version] [EXACT]      # Minimum or EXACT version e.g. 1.36.0
#     [REQUIRED]             # Fail with error if Bit is not found
#     [COMPONENTS <libs>...] # Bit libraries by their canonical name
#     )                      # e.g. "stl" for "bit_stl"
#
# This module finds headers and requested component libraries using CMake
# package configuration files provided by a Bit cmake build.

if( "$ENV{BIT_HOME}" STREQUAL "" )
  if( NOT "$ENV{BIT_ROOT}" STREQUAL "" )
    set($ENV{BIT_HOME} $ENV{BIT_ROOT})
  elseif (NOT "$ENV{BITROOT}" STREQUAL "")
    set($ENV{BIT_HOME} $ENV{BITROOT})
  endif()
endif()

set(_Bit_QUIET)
set(_Bit_REQUIRED)

if( Bit_FIND_QUIETLY )
  set(_Bit_QUIET QUIET)
endif()
if( Bit_FIND_VERSION_EXACT )
  set(_Bit_EXACT EXACT)
endif()
if( Bit_FIND_REQUIRED )
  set(_Bit_REQUIRED REQUIRED)
endif()

set(_Bit_COMPONENT_VERSION)
if(NOT Bit_FIND_VERSION STREQUAL "")
  set(_Bit_COMPONENT_VERSION ${Bit_FIND_VERSION})
endif()

function(_Bit_GET_CANDIDATE_VERSIONS result component version)
  file(GLOB _Bit_VERSIONS RELATIVE "$ENV{BIT_HOME}/${component}/" "$ENV{BIT_HOME}/${component}/*")
  list(SORT _Bit_VERSIONS)
  list(REVERSE _Bit_VERSIONS)

  set(_Bit_CANDIDATE_VERSIONS)
  if(version STREQUAL "")
    set(_Bit_CANDIDATE_VERSIONS ${_Bit_VERSIONS})
  else()
    foreach(_Bit_VERSION ${_Bit_VERSIONS})
      if("${_Bit_VERSION}" VERSION_GREATER "${version}" OR "${_Bit_VERSION}" VERSION_EQUAL "${version}")
        list(APPEND _Bit_CANDIDATE_VERSIONS ${_Bit_VERSION})
      endif()
    endforeach()
  endif()

  set(${result} ${_Bit_CANDIDATE_VERSIONS} PARENT_SCOPE)
endfunction()

foreach( _Bit_FIND_COMPONENT ${Bit_FIND_COMPONENTS})

  # Ignore components that have already been included, or exist in the current
  # source tree.
  string(TOLOWER "${_BIT_FIND_COMPONENT}" _BIT_FIND_COMPONENT_LOWER)
  if( TARGET "bit::${_BIT_FIND_COMPONENT_LOWER}" )
    continue()
  endif()

  _Bit_GET_CANDIDATE_VERSIONS(_CANDIDATE_VERSIONS "${_Bit_FIND_COMPONENT}" "${_Bit_COMPONENT_VERSION}")

  set(_SUFFIXES)
  foreach(_CANDIDATE_VERSION ${_CANDIDATE_VERSIONS})
    list(APPEND _SUFFIXES "${_CANDIDATE_VERSION}/cmake")
  endforeach()

  set(_COMPONENT_VERSION ${_Bit_COMPONENT_VERSION})
  if(_CANDIDATE_VERSIONS AND ("${_Bit_COMPONENT_VERSION}" STREQUAL ""))
    list(GET _CANDIDATE_VERSIONS 0 _version)
    set(_COMPONENT_VERSION ${_version})
  endif()

  find_package( ${_Bit_FIND_COMPONENT} ${_COMPONENT_VERSION}
    ${_Bit_EXACT} ${_Bit_QUIET} ${_Bit_REQUIRED}
    PATHS $ENV{BIT_HOME}/${_Bit_FIND_COMPONENT}
    PATH_SUFFIXES ${_SUFFIXES}
  )

endforeach()

if(Bit_DEBUG AND NOT Bit_FIND_QUIETLY)
  message(STATUS "Bit_FIND_COMPONENTS      = ${Bit_FIND_COMPONENTS}")
  message(STATUS "Bit_FIND_VERSION         = ${Bit_FIND_VERSION}")
  message(STATUS "Bit_FIND_VERSION_MAJOR   = ${Bit_FIND_VERSION_MAJOR}")
  message(STATUS "Bit_FIND_VERSION_MINOR   = ${Bit_FIND_VERSION_MINOR}")
  message(STATUS "Bit_FIND_VERSION_PATCH   = ${Bit_FIND_VERSION_PATCH}")
  message(STATUS "Bit_FIND_VERSION_EXACT   = ${Bit_FIND_VERSION_EXACT}")
  message(STATUS "Bit_FIND_REQUIRED        = ${Bit_FIND_REQUIRED}")
  message(STATUS "Bit_FIND_QUIETLY         = ${Bit_FIND_QUIETLY}")
  message(STATUS "Bit_ROOT_DIR             = ${Bit_ROOT_DIR}")
  message(STATUS "Bit_FOUND                = ${Bit_FOUND}")
  message(STATUS "Bit_LIBRARIES            = ${Bit_LIBRARIES}")
  message(STATUS "Bit_INCLUDE_DIRS         = ${Bit_INCLUDE_DIRS}")
  message(STATUS "Bit_LIBRARY_DIRS         = ${Bit_LIBRARY_DIRS}")
  message(STATUS "Bit_INCLUDE_SEARCH_PATHS = '${Bit_INCLUDE_SEARCH_PATHS}'")
  message(STATUS "Bit_LIBRARY_SEARCH_PATHS = '${Bit_LIBRARY_SEARCH_PATHS}'")
  message(STATUS "Bit testing versions     = ${_Bit_TEST_VERSIONS}")
endif()
