cmake_minimum_required(VERSION 3.1)

enable_testing()

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake/modules")

include(AddIndependenceCheck)
include(GitBuildInformation)
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

git_build_information( PATCH_VARIABLE BIT_MEMORY_VERSION_PATCH
                       BUILD_VARIABLE BIT_MEMORY_BUILD_NUMBER
                       BRANCH_VARIABLE BIT_MEMORY_BRANCH
                       REF v0.1
                       PATHSPECS ${CMAKE_CURRENT_LIST_DIR}/include
                                 ${CMAKE_CURRENT_LIST_DIR}/cmake
                                 ${CMAKE_CURRENT_LIST_FILE}
)

set(BIT_MEMORY_VERSION_MAJOR 0 CACHE STRING "major version of bit::memory" FORCE)
set(BIT_MEMORY_VERSION_MINOR 1 CACHE STRING "minor version of bit::memory" FORCE)
set(BIT_MEMORY_VERSION_PATCH ${BIT_MEMORY_VERSION_PATCH} CACHE STRING "patch version of bit::memory" FORCE)
set(BIT_MEMORY_VERSION "${BIT_MEMORY_VERSION_MAJOR}.${BIT_MEMORY_VERSION_MINOR}.${BIT_MEMORY_VERSION_PATCH}" CACHE STRING "version of bit::memory" FORCE)
set(BIT_MEMORY_BUILD_NUMBER "${BIT_MEMORY_BUILD_NUMBER}" CACHE STRING "build number of bit::memory" FORCE)

message(STATUS "bit::memory ${BIT_MEMORY_VERSION} (${BIT_MEMORY_BRANCH} ${BIT_MEMORY_BUILD_NUMBER})")

#-----------------------------------------------------------------------------
# bit::memory
#-----------------------------------------------------------------------------

set(CMAKE_CXX_VERSION 14)

make_version_header("${CMAKE_CURRENT_BINARY_DIR}/include/bit/memory/version.hpp"
  MAJOR ${BIT_MEMORY_VERSION_MAJOR}
  MINOR ${BIT_MEMORY_VERSION_MINOR}
  PATCH ${BIT_MEMORY_VERSION_PATCH}
  BUILD ${BIT_MEMORY_BUILD_NUMBER}
  PREFIX BIT_MEMORY
  SUFFIX "a"
  TAG ${BIT_MEMORY_BRANCH}
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
  include/bit/memory/concepts.hpp
  include/bit/memory/debugging.hpp
  include/bit/memory/endian.hpp
  include/bit/memory/errors.hpp
  include/bit/memory/freelist.hpp
  include/bit/memory/macros.hpp
  include/bit/memory/memory.hpp
  include/bit/memory/memory_block.hpp
  include/bit/memory/memory_block_cache.hpp
  include/bit/memory/not_null.hpp
  include/bit/memory/std_allocator_adapter.hpp

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

set_target_properties(memory PROPERTIES
  CXX_STANDARD 14
  CXX_STANDARD_REQUIRED true
)

target_include_directories(memory PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
)
target_compile_options(memory PUBLIC -std=c++14
  $<$<CONFIG:DEBUG>:-DDEBUG -Wall -pedantic>
  $<$<CONFIG:RELEASE>:-DNDEBUG -DRELEASE>
)

#-----------------------------------------------------------------------------
# bit::memory : Independence Tests
#-----------------------------------------------------------------------------

if( BIT_MEMORY_COMPILE_INDEPENDENCE_TESTS )

  add_independence_check(memory_independence ${headers})
  target_compile_options(memory_independence PRIVATE -std=c++14
    $<$<CONFIG:DEBUG>:-DDEBUG>
    $<$<CONFIG:RELEASE>:-DNDEBUG -DRELEASE>
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
