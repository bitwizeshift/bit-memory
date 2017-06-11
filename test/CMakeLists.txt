cmake_minimum_required(VERSION 3.1)

find_package(Catch REQUIRED)

set(sources
      main.test.cpp

      bit/memory/null_block_allocator.test.cpp
        bit/memory/malloc_block_allocator.test.cpp)

add_executable(memory_test ${sources})

target_link_libraries(memory_test PRIVATE "bit::memory" "philsquared::Catch")

#-----------------------------------------------------------------------------
# Testing
#-----------------------------------------------------------------------------

add_test( NAME "memory_test_default"
          WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
          COMMAND "$<TARGET_FILE:memory_test>" )

#-----------------------------------------------------------------------------

add_test( NAME "memory_test_all"
          WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
          COMMAND "$<TARGET_FILE:memory_test>" "*" )
