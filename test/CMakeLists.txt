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
  bit/memory/utilities/memory_block_cache.test.cpp
  bit/memory/utilities/endian.test.cpp

  # Allocators
  bit/memory/allocators/allocator_reference.test.cpp

  # Block Allocators
  bit/memory/block_allocators/aligned_block_allocator.test.cpp
  bit/memory/block_allocators/block_allocator_reference.test.cpp
  bit/memory/block_allocators/null_block_allocator.test.cpp
  bit/memory/block_allocators/malloc_block_allocator.test.cpp
  bit/memory/block_allocators/new_block_allocator.test.cpp
  bit/memory/block_allocators/stack_block_allocator.test.cpp
  bit/memory/block_allocators/static_block_allocator.test.cpp
  bit/memory/block_allocators/virtual_block_allocator.test.cpp

  ${platform_source_files}
)

# Unfortunately, certain AppleClang versions don't support 'thread_local'; so to
# avoid failing independence tests, they have been appended here
if( NOT "${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang" )
  list(APPEND source_files bit/memory/block_allocators/thread_local_block_allocator.test.cpp)
endif()


add_executable(bit_memory_test ${source_files})

target_link_libraries(bit_memory_test PRIVATE "bit::memory" "philsquared::Catch")

#-----------------------------------------------------------------------------
# Testing
#-----------------------------------------------------------------------------

add_test( NAME "memory_test_default"
          WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
          COMMAND "$<TARGET_FILE:bit_memory_test>" )

#-----------------------------------------------------------------------------

add_test( NAME "memory_test_all"
          WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
          COMMAND "$<TARGET_FILE:bit_memory_test>" "*" )
