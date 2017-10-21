cmake_minimum_required(VERSION 2.6.3)

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
#     add_test_executable( <target> [sources]... )
#
#     <target>      - The name of the target to create
#     [sources]...  - List of source files to use for the unit test
#
#
function(add_test_executable target)

	set(source_files ${ARGN})

	add_executable(${target} ${source_files})

	add_custom_command(
		TARGET ${target}
		POST_BUILD
		COMMAND "$<TARGET_FILE:${target}>"
		WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
		COMMENT "Running Unit Test '${target}'"
	)

endfunction()
