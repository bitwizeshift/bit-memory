cmake_minimum_required(VERSION 3.1)

find_package(Catch REQUIRED)

option(BIT_MEMORY_COMPILE_ASAN "Compile and run the address sanetizer" off)
option(BIT_MEMORY_COMPILE_USAN "Compile and run the undefined behavior sanitizer" off)
option(BIT_MEMORY_COMPILE_TSAN "Compile and run the thread sanitizer" off)
option(BIT_MEMORY_COMPILE_COVERAGE "Compile and run code coverage" off)

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
if( NOT "${CMAKE_CXX_COMPILER_ID}" MATCHES "AppleClang" )
  list(APPEND source_files bit/memory/block_allocators/thread_local_block_allocator.test.cpp)
endif()


add_executable(bit_memory_test ${source_files})
group_source_tree("${CMAKE_CURRENT_LIST_DIR}" ${source_files})

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

# The following code is explicitly for G++/LLVM-Clang, so ignore any other compiler
if( NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" AND
    NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" )
  return()
endif()

#-----------------------------------------------------------------------------
# Address Sanitizer
#-----------------------------------------------------------------------------

if( BIT_MEMORY_COMPILE_ASAN )

  target_link_libraries(bit_memory_test PRIVATE -fsanitize=address
                                                -fsanitize=leak
                                                -fuse-ld=gold)

  target_compile_options(bit_memory_test PRIVATE
    -g
    -O1
    -fno-omit-frame-pointer
    -fsanitize=address
    -fsanitize=leak
    -fuse-ld=gold
  )

endif()

#-----------------------------------------------------------------------------
# Undefined Behavior Sanitizer
#-----------------------------------------------------------------------------

if( BIT_MEMORY_COMPILE_UBSAN )

  target_compile_options(bit_memory_test PRIVATE -fsanitize=undefined -fuse-ld=gold)
  target_link_libraries(bit_memory_test PRIVATE -fsanitize=undefined -fuse-ld=gold)

endif()

#-----------------------------------------------------------------------------
# Thread Sanitizer
#-----------------------------------------------------------------------------

if( BIT_MEMORY_COMPILE_TSAN )

  target_compile_options(bit_memory_test PRIVATE -fsanitize=thread -fuse-ld=gold)
  target_link_libraries(bit_memory_test PRIVATE -fsanitize=thread -fuse-ld=gold)

endif()

#-----------------------------------------------------------------------------
# Code Coverage
#-----------------------------------------------------------------------------

if( BIT_MEMORY_COMPILE_COVERAGE )


  target_link_libraries(bit_memory_test PRIVATE --coverage)
  target_compile_options(bit_memory_test PRIVATE
    -g
    -O0
    -fprofile-arcs
    -ftest-coverage
  )

endif()
