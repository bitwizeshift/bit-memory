#.rst:
# FindCatch
# ---------
#
# Find Catch
#
# Find Philsquared's "Catch" unit test library.
#
# ::
#
#     philsquared::Catch - Interface library target for the Catch headers
#     Catch_FOUND        - TRUE if Catch was found
#     Catch_INCLUDE_DIRS - The include path for catch headers
#
find_path(
  Catch_INCLUDE_DIR
  NAMES "catch.hpp"
  DOC "Catch unit-test include directory"
)

set(Catch_INCLUDE_DIRS ${Catch_INCLUDE_DIR} CACHE FILEPATH "Include directory for the CATCH unit test")

# Create a custom Catch target if not already defined
if( NOT TARGET philsquared::Catch AND Catch_INCLUDE_DIRS )

  add_library(philsquared::Catch INTERFACE IMPORTED)
  set_target_properties(philsquared::Catch PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES ${Catch_INCLUDE_DIRS}
  )

endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Catch DEFAULT_MSG Catch_INCLUDE_DIR)
mark_as_advanced(Catch_INCLUDE_DIRS)
