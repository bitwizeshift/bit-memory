cmake_minimum_required(VERSION 3.1)

include(CMakeParseArguments)

#.rst:
# GitBuildInformation
# ---------
#
# Gets build information from a git repository
#
# Determines the git commit count to use as a build number
#
# ::
#
#     git_build_information( [PATCH_VARIABLE <variable>]
#                            [BUILD_VARIABLE <variable>]
#                            [BRANCH_VARIABLE <variable>]
#                            [REF <hash>]
#                            [PATHSPECS <specs>...)
#
#     PATCH_VARIABLE <variable>  - The variable to store the patch number
#     BUILD_VARIABLE <variable>  - The variable to store the build number
#     BRANCH_VARIABLE <variable> - The variable to store the minor number
#     REF <hash>                 - The git reference of the most recent major revision.
#                                  This may be an identifier, tag, or hash
#     PATHSPECS <specs>...       - The path specs to use for identifiers
#
macro(git_build_information)

  set(__single_args PATCH_VARIABLE BUILD_VARIABLE BRANCH_VARIABLE REF)
  set(__multi_args PATHSPECS)
  cmake_parse_arguments("GIT" "" "${__single_args}" "${__multi_args}" "${ARGN}")

  if( NOT GIT_REF )
    message(FATAL_ERROR "git_build_information: REF not specified")
  endif()

  if( NOT GIT_EXECUTABLE_PATH )
    find_program(GIT_EXECUTABLE_PATH git)
  endif()

  execute_process(COMMAND "${GIT_EXECUTABLE_PATH}" --version
                  OUTPUT_VARIABLE GIT_VERSION
                  OUTPUT_STRIP_TRAILING_WHITESPACE)

  message(STATUS "Git version identified as: ${GIT_VERSION}")

  if( NOT GIT_EXECUTABLE_PATH )
    if( GIT_PATCH_VARIABLE STREQUAL "" )
      set("${GIT_PATCH_VARIABLE}" "${GIT_PATCH_VARIABLE}-NOTFOUND")
    endif()
    if( GIT_BUILD_VARIABLE STREQUAL "" )
      set("${GIT_BUILD_VARIABLE}" "${GIT_BUILD_VARIABLE}-NOTFOUND")
    endif()
    if( GIT_BRANCH_VARIABLE STREQUAL "" )
      set("${GIT_BRANCH_VARIABLE}" "${GIT_BRANCH_VARIABLE}-NOTFOUND")
    endif()

    return()
  endif()

  ############################## PATCH VERSION #############################

  if( GIT_PATCH_VARIABLE )

    execute_process(COMMAND "${GIT_EXECUTABLE_PATH}" rev-list --count ${GIT_REF} HEAD -- ${GIT_PATHSPECS}
                    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
                    OUTPUT_VARIABLE "${GIT_PATCH_VARIABLE}"
                    ERROR_VARIABLE "_error"
                    RESULT_VARIABLE "_result"
                    OUTPUT_STRIP_TRAILING_WHITESPACE
                    ERROR_STRIP_TRAILING_WHITESPACE )

    if( NOT _result EQUAL 0 )
      message(FATAL_ERROR "git_build_information: Error retrieving patch revision:\n ${_error}")
    endif()

  endif()

  ############################## BUILD VERSION #############################

  if( GIT_BUILD_VARIABLE )

    execute_process(COMMAND "${GIT_EXECUTABLE_PATH}" rev-list --count HEAD -- ${GIT_PATHSPECS}
                    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
                    OUTPUT_VARIABLE "${GIT_BUILD_VARIABLE}"
                    ERROR_VARIABLE "_error"
                    RESULT_VARIABLE "_result"
                    OUTPUT_STRIP_TRAILING_WHITESPACE
                    ERROR_STRIP_TRAILING_WHITESPACE )

    if( NOT _result EQUAL 0 )
      message(FATAL_ERROR "git_build_information: Error retrieving build number:\n ${_error}")
    endif()

  endif()

  ############################## BRANCH NAME ###############################

  if( GIT_BRANCH_VARIABLE )

    execute_process(COMMAND "${GIT_EXECUTABLE_PATH}" rev-parse --abbrev-ref HEAD
                    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
                    OUTPUT_VARIABLE "${GIT_BRANCH_VARIABLE}"
                    ERROR_VARIABLE "_error"
                    RESULT_VARIABLE "_result"
                    OUTPUT_STRIP_TRAILING_WHITESPACE
                    ERROR_STRIP_TRAILING_WHITESPACE )

    if( NOT _result EQUAL 0 )
      message(FATAL_ERROR "git_build_information: Error retrieving branch name. ${_error}")
    endif()

  endif()

endmacro()
