cmake_minimum_required(VERSION 3.1)

find_package(Catch REQUIRED)

if( WIN32 OR UNIX OR APPLE )
  set(platform_source_files
         bit/memory/block_allocators/virtual_block_allocator.test.cpp
  )
endif()

set(source_files
  main.test.cpp
  # Utilities
  bit/memory/memory_block_cache.test.cpp
  # Memory Allocations

  # Block Allocators
  bit/memory/block_allocators/null_block_allocator.test.cpp
  bit/memory/block_allocators/malloc_block_allocator.test.cpp
  bit/memory/block_allocators/new_block_allocator.test.cpp

  # Allocators

  ${platform_source_files}
)

add_executable(memory_test ${source_files})

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
