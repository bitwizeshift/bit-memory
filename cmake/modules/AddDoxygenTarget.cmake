cmake_minimum_required(VERSION 3.1)

#.rst:
# add_test_executable
# ---------
# Creates an executable that performs a unit test execution
#
# This acts identically to add_executable except that it imlicitly adds a
# test case that executes the program as part of the build
#
# ::
#
#     add_doxygen_target( <target> )
#
#     <target>      - The name of the target to create
#
#
function(add_doxygen_target target)
  if( NOT DOXYGEN_PATH )
    set(temp_state ${CMAKE_FIND_APPBUNDLE})
    set(CMAKE_FIND_APPBUNDLE "NEVER")

    find_program(DOXYGEN_PATH doxygen)

    set(CMAKE_FIND_APPBUNDLE ${temp_state})
  endif()

  # Add target for generating doxygen
  add_custom_target("${target}" ALL
                    COMMAND "${DOXYGEN_PATH}" "Doxyfile"
                    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
                    COMMENT "Generating Doxygen documentation for 'bit::memory'"
                    VERBATIM )
endfunction()
