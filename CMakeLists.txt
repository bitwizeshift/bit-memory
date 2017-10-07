cmake_minimum_required(VERSION 3.1)

enable_testing()

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake/modules")

include(AddIndependenceCheck)
include(MakeVersionHeader)

#-----------------------------------------------------------------------------
# Project Setup
#-----------------------------------------------------------------------------

option(BIT_MEMORY_COMPILE_INDEPENDENCE_TESTS "Include each header independently in a .cpp file to determine header independence" on)
option(BIT_MEMORY_COMPILE_UNIT_TESTS "Compile and run the unit tests for this library" on)

project("BitMemory")

if( CMAKE_BUILD_TYPE STREQUAL "" )
  message(STATUS "CMAKE_BUILD_TYPE not set; defaulting to 'Debug'")
  set(CMAKE_BUILD_TYPE DEBUG)
endif()

#-----------------------------------------------------------------------------
# Version Info
#-----------------------------------------------------------------------------

set(BIT_MEMORY_VERSION_MAJOR 0 CACHE STRING "major version of bit::memory" FORCE)
set(BIT_MEMORY_VERSION_MINOR 1 CACHE STRING "minor version of bit::memory" FORCE)
set(BIT_MEMORY_VERSION_PATCH 188 CACHE STRING "patch version of bit::memory" FORCE)
set(BIT_MEMORY_VERSION "${BIT_MEMORY_VERSION_MAJOR}.${BIT_MEMORY_VERSION_MINOR}.${BIT_MEMORY_VERSION_PATCH}" CACHE STRING "version of bit::memory" FORCE)

message(STATUS "bit::memory ${BIT_MEMORY_VERSION}")

#-----------------------------------------------------------------------------
# bit::memory
#-----------------------------------------------------------------------------

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED true)

make_version_header("${CMAKE_CURRENT_BINARY_DIR}/include/bit/memory/version.hpp"
  MAJOR ${BIT_MEMORY_VERSION_MAJOR}
  MINOR ${BIT_MEMORY_VERSION_MINOR}
  PATCH ${BIT_MEMORY_VERSION_PATCH}
  PREFIX BIT_MEMORY
  SUFFIX "a"
)

set(headers
  # Utilities
  include/bit/memory/detail/allocator_function_traits.hpp
  include/bit/memory/detail/block_allocator_function_traits.hpp
  include/bit/memory/detail/ebo_storage.hpp
  include/bit/memory/detail/void_t.hpp
  include/bit/memory/detail/identity.hpp
  include/bit/memory/detail/dynamic_size_type.hpp

  include/bit/memory/allocator_info.hpp
  include/bit/memory/allocator_deleter.hpp
  include/bit/memory/allocator_reference.hpp
  include/bit/memory/byte.hpp
  include/bit/memory/concepts.hpp
  include/bit/memory/debugging.hpp
  include/bit/memory/endian.hpp
  include/bit/memory/errors.hpp
  include/bit/memory/freelist.hpp
  include/bit/memory/macros.hpp
  include/bit/memory/memory_block.hpp
  include/bit/memory/memory_block_cache.hpp
  include/bit/memory/not_null.hpp
  include/bit/memory/owner.hpp
  include/bit/memory/std_allocator_adapter.hpp
  include/bit/memory/unaligned_memory.hpp
  include/bit/memory/uninitialized_storage.hpp

  # Taggers
  include/bit/memory/taggers/allocator_tagger.hpp
  include/bit/memory/taggers/block_allocator_tagger.hpp
  include/bit/memory/taggers/null_tagger.hpp

  # Trackers
  include/bit/memory/trackers/detail/stat_recording_tracker.hpp
  include/bit/memory/trackers/detailed_leak_tracker.hpp
  include/bit/memory/trackers/leak_tracker.hpp
  include/bit/memory/trackers/null_tracker.hpp

  # Bounds Checkers
  include/bit/memory/bounds_checkers/debug_bounds_checker.hpp
  include/bit/memory/bounds_checkers/null_bounds_checker.hpp

  # Lockables
  include/bit/memory/lockables/null_lock.hpp

  # Memory Allocations
  include/bit/memory/virtual_memory.hpp
  include/bit/memory/aligned_memory.hpp
  include/bit/memory/allocator_traits.hpp
  include/bit/memory/block_allocator_traits.hpp

  # Allocators
  include/bit/memory/allocators/detail/named_allocator.hpp
  include/bit/memory/allocators/aligned_allocator.hpp
  include/bit/memory/allocators/aligned_offset_allocator.hpp
  include/bit/memory/allocators/any_allocator.hpp
  include/bit/memory/allocators/policy_allocator.hpp
  include/bit/memory/allocators/malloc_allocator.hpp
  include/bit/memory/allocators/min_aligned_allocator.hpp
  include/bit/memory/allocators/new_allocator.hpp
  include/bit/memory/allocators/null_allocator.hpp
  include/bit/memory/allocators/stack_allocator.hpp

  # Block Allocators
  include/bit/memory/block_allocators/detail/named_block_allocator.hpp
  include/bit/memory/block_allocators/aligned_block_allocator.hpp
  include/bit/memory/block_allocators/any_block_allocator.hpp
  include/bit/memory/block_allocators/cached_block_allocator.hpp
  include/bit/memory/block_allocators/malloc_block_allocator.hpp
  include/bit/memory/block_allocators/new_block_allocator.hpp
  include/bit/memory/block_allocators/null_block_allocator.hpp
  include/bit/memory/block_allocators/policy_block_allocator.hpp
  include/bit/memory/block_allocators/stack_block_allocator.hpp
  include/bit/memory/block_allocators/static_block_allocator.hpp
  include/bit/memory/block_allocators/virtual_block_allocator.hpp
)

if( WIN32 )
  set(platform_source_files
    src/bit/memory/win32/virtual_memory.cpp
    src/bit/memory/win32/aligned_memory.cpp
  )
elseif( UNIX )
  set(platform_source_files
    src/bit/memory/posix/virtual_memory.cpp
    src/bit/memory/posix/aligned_memory.cpp
  )
elseif( APPLE )
  set(platform_source_files
    src/bit/memory/posix/virtual_memory.cpp
    src/bit/memory/posix/aligned_memory.cpp
  )
else()
  message(FATAL_ERROR "unknown or unsupported target memory")
endif()

set(source_files
  # Utilities
  src/bit/memory/debugging.cpp
  src/bit/memory/errors.cpp

  # Memory Allocations
  src/bit/memory/virtual_memory.cpp

  # Block Allocators
  src/bit/memory/block_allocators/virtual_block_allocator.cpp

  # memory-specific
  ${platform_source_files}
)

add_library(memory ${source_files} ${headers})
add_library(bit::memory ALIAS memory)

# Add include directories
target_include_directories(memory PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
)

# Add DEBUG, NDEBUG, and RELEASE macro definitions
target_compile_definitions(memory PUBLIC
  $<$<CONFIG:DEBUG>:DEBUG>
  $<$<CONFIG:RELEASE>:DEBUG RELEASE>
)

# Add compiler-specific flags
if( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  target_compile_options(memory PUBLIC -Wall -Wstrict-aliasing -pedantic)
elseif( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" )
  target_compile_options(memory PUBLIC -Wall -Wstrict-aliasing -pedantic)
elseif( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC" )
  # TODO: Determine MSVC necessary compiler flags
endif()


#-----------------------------------------------------------------------------
# bit::memory : Independence Tests
#-----------------------------------------------------------------------------

if( BIT_MEMORY_COMPILE_INDEPENDENCE_TESTS )

  add_independence_check(memory_independence ${headers})

  target_compile_definitions(memory_independence PUBLIC
    $<$<CONFIG:DEBUG>:DEBUG>
    $<$<CONFIG:RELEASE>:DEBUG RELEASE>
  )

  target_include_directories(memory_independence PRIVATE $<TARGET_PROPERTY:memory,INCLUDE_DIRECTORIES>)

  add_library(bit::memory::independence ALIAS memory_independence)

endif()

#-----------------------------------------------------------------------------
# bit::memory : Unit Tests
#-----------------------------------------------------------------------------

if( BIT_MEMORY_COMPILE_UNIT_TESTS )

  add_subdirectory(test)

endif()

#-----------------------------------------------------------------------------
# bit::memory : Export
#-----------------------------------------------------------------------------

include(ExportLibrary)

if( EXISTS "$ENV{BIT_HOME}" )
  set(CMAKE_INSTALL_PREFIX "$ENV{BIT_HOME}")
endif()

export_library( TARGETS memory
                PACKAGE Memory
                VERSION ${BIT_MEMORY_VERSION}
                MAJOR_VERSION ${BIT_MEMORY_VERSION_MAJOR}
                MINOR_VERSION ${BIT_MEMORY_VERSION_MINOR}
                INCLUDE_PATHS "${CMAKE_CURRENT_LIST_DIR}/include"
                              "${CMAKE_CURRENT_BINARY_DIR}/include"
                NAMESPACE "bit::"
                PREFIX_PACKAGE )
