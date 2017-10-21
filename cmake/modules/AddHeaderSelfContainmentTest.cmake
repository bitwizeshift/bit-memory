cmake_minimum_required(VERSION 2.8.8)

#.rst:
# AddHeaderSelfContainmentTest
# ---------
#
# Creates a C or C++ library that includes each specified header file
# independently to ensure that each header carries no expected ordering.
#
# This is used to avoid accidental dependencies based on the order of
# inclusion.
#
# ::
#
#     add_header_self_containment_test( <target> [C|CXX] [headers]... )
#
#     <target>      - The name of the target to create
#     [C|CXX]       - The language check. By default, this is CXX
#     [headers]...  - List of headers to compile
#
function(add_header_self_containment_test target arg0)

  ############################### Setup output ###############################

  if( arg0 STREQUAL "C" ) # C
    set(headers ${ARGN})
    set(extension "c")
  elseif( arg0 STREQUAL "CXX" ) # CXX
    set(headers ${ARGN})
    set(extension "cpp")
  else()
    set(headers ${arg0} ${ARGN})
    set(extension "cpp")
  endif()

  set(output_dir "${CMAKE_CURRENT_BINARY_DIR}/self_containment_test")

  ############################### Create files ###############################

  set(source_files)
  foreach( header ${headers} )

    get_filename_component(path_segment "${header}" PATH)
    get_filename_component(absolute_header "${header}" ABSOLUTE)
    file(RELATIVE_PATH relative_header "${output_dir}/${path_segment}" "${absolute_header}")

    set(output_path "${output_dir}/${header}.${extension}")

    if( NOT EXISTS "${output_path}" OR "${absolute_header}" IS_NEWER_THAN "${output_path}" )

      file(WRITE "${output_path}" "#include \"${relative_header}\" // IWYU pragma: keep")

    endif()

    set_property( DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES "${output_path}")
    set_property( DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS "${output_path}")

    list(APPEND source_files "${output_path}")

  endforeach()

  ###################### Create self-containment Target ######################

  add_library("${target}" OBJECT ${source_files} ${headers})

endfunction()
