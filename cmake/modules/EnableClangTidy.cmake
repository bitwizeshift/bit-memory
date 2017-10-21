#.rst:
# EnableClangTidy
# ---------
#
# Enables clang-tidy support in CMake
#
# This module only works for CMake versions 3.6 or above; no error is
# emitted on anything lower, the module simply becomes a no-op
#
# ::
#
#     enable_clang_tidy( [LANGUAGES <language names>...]
#                        [ARGS <args to clang-tidy>...] )
#
#     target_enable_clang_tidy( target
#                               [LANGUAGES <language names>...]
#                               [ARGS <args to clang-tidy>...]  )
#
# The first form sets the clang-tidy language and arguments globally for all
# targets, the second form only enables it for the specified target
#

include(CMakeParseArguments)

# Don't bother searching unless we are using CMake version 3.6 or above
if(CMAKE_VERSION VERSION_GREATER 3.5)
  find_program(CLANG_TIDY_PATH clang-tidy QUIET)
  mark_as_advanced(CLANG_TIDY_PATH)
endif()


macro(enable_clang_tidy)
  if( NOT CMAKE_VERSION VERSION_GREATER 3.5 )
    return()
  endif()

  if( NOT CLANG_TIDY_PATH )
    return()
  endif()

  message(STATUS "Enabling clang-tidy")

  cmake_parse_arguments("__clang_tidy" "" "" "LANGUAGES;ARGS" ${ARGN})

  if( NOT __clang_tidy_LANGUAGES )
    set(__clang_tidy_LANGUAGES CXX) # default to C++ support
  endif()

  foreach( lang ${__clang_tidy_LANGUAGES} )
    set(CMAKE_${lang}_CLANG_TIDY "${CLANG_TIDY_PATH};${__clang_tidy_ARGS}")
  endforeach()

  set(CMAKE_CLANG_TIDY_ENABLED On CACHE INTERNAL "")
  set(CMAKE_EXPORT_COMPILE_COMMANDS On)

  unset(__clang_tidy_LANGUAGES)
  unset(__clang_tidy_ARGS)

  message(STATUS "Enabling clang-tidy - done")
endmacro()


macro(target_enable_clang_tidy target)
  if( NOT CMAKE_VERSION VERSION_GREATER 3.5 )
    return()
  endif()

  if( NOT CLANG_TIDY_PATH )
    return()
  endif()

  cmake_parse_arguments("__clang_tidy" "" "" "LANGUAGES;ARGS" ${ARGN})

  if( NOT __clang_tidy_LANGUAGES )
    set(__clang_tidy_LANGUAGES CXX) # default to C++ support
  endif()

  foreach( lang ${__clang_tidy_LANGUAGES} )
    set_target_properties(${target} PROPERTY ${lang}_CLANG_TIDY "${CLANG_TIDY_PATH};${__clang_tidy_ARGS}")
  endforeach()

  unset(__clang_tidy_LANGUAGES)
  unset(__clang_tidy_ARGS)
endmacro()
