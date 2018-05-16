cmake_minimum_required(VERSION 3.1.3)

#.rst:
# GroupSourceTree
# ---------
#
# Makes a version header
#
# ::
#
#     group_source_tree( <root> [files]... )
#
#     <root>     - The root to start the source tree
#     [files]... - The
#
function(group_source_tree root)

  if( ${CMAKE_VERSION} VERSION_GREATER 3.8.1 )
    source_group( TREE "${root}" FILES ${ARGN} )
  else()
    foreach( file ${ARGN} )
      get_filename_component(group "${file}" REALPATH)
      file(RELATIVE_PATH group "${root}" "${group}")
      get_filename_component(group "${group}" DIRECTORY)
      string(REPLACE "/" "\\" group "${group}")
      source_group("${group}" FILES ${file})
    endforeach()
  endif()

endfunction()
