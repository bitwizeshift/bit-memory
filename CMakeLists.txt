cmake_minimum_required(VERSION 3.1)

enable_testing()

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake/modules")

include(AddHeaderSelfContainmentTest)
include(MakeVersionHeader)
include(CopyTargetProperties)

#-----------------------------------------------------------------------------
# Project Setup
#-----------------------------------------------------------------------------

option(BIT_MEMORY_COMPILE_HEADER_SELF_CONTAINMENT_TESTS "Include each header independently in a .cpp file to determine header self-containment" off)
option(BIT_MEMORY_COMPILE_UNIT_TESTS "Compile and run the unit tests for this library" off)
option(BIT_MEMORY_GENERATE_DOCUMENTATION "Generates doxygen documentation" off)
option(BIT_MEMORY_COMPILE_BENCHMARKS "Compile the benchmark tests for this library" off)

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
# bit::memory: Sources
#-----------------------------------------------------------------------------

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_EXTENSIONS off)

make_version_header("${CMAKE_CURRENT_BINARY_DIR}/include/bit/memory/version.hpp"
  MAJOR ${BIT_MEMORY_VERSION_MAJOR}
  MINOR ${BIT_MEMORY_VERSION_MINOR}
  PATCH ${BIT_MEMORY_VERSION_PATCH}
  PREFIX BIT_MEMORY
  SUFFIX "a"
)

set(headers
  # Concepts
  include/bit/memory/concepts/detail/void_t.hpp
  include/bit/memory/concepts/detail/identity.hpp
  include/bit/memory/concepts/Stateless.hpp
  include/bit/memory/concepts/Allocator.hpp
  include/bit/memory/concepts/BlockAllocator.hpp
  include/bit/memory/concepts/BasicLockable.hpp
  include/bit/memory/concepts/BoundsChecker.hpp
  include/bit/memory/concepts/ExtendedAllocator.hpp
  include/bit/memory/concepts/MemoryTagger.hpp
  include/bit/memory/concepts/MemoryTracker.hpp

  # Utilities
  include/bit/memory/detail/ebo_storage.hpp
  include/bit/memory/detail/dynamic_size_type.hpp
  include/bit/memory/aligned_memory.hpp
  include/bit/memory/allocator_traits.hpp
  include/bit/memory/allocator_info.hpp
  include/bit/memory/allocator_deleter.hpp
  include/bit/memory/allocator_reference.hpp
  include/bit/memory/block_allocator_traits.hpp
  include/bit/memory/debugging.hpp
  include/bit/memory/endian.hpp
  include/bit/memory/errors.hpp
  include/bit/memory/freelist.hpp
  include/bit/memory/macros.hpp
  include/bit/memory/memory_block.hpp
  include/bit/memory/memory_block_cache.hpp
  include/bit/memory/not_null.hpp
  include/bit/memory/owner.hpp
  include/bit/memory/pointer_utilities.hpp
  include/bit/memory/std_allocator_adapter.hpp
  include/bit/memory/std_any_allocator_adapter.hpp
  include/bit/memory/unaligned_memory.hpp
  include/bit/memory/uninitialized_storage.hpp
  include/bit/memory/virtual_memory.hpp

  # Taggers
  include/bit/memory/taggers/allocator_tagger.hpp
  include/bit/memory/taggers/block_allocator_tagger.hpp
  include/bit/memory/taggers/null_tagger.hpp

  # Trackers
  include/bit/memory/trackers/detail/stat_recording_tracker.hpp
  include/bit/memory/trackers/detailed_leak_tracker.hpp
  include/bit/memory/trackers/leak_tracker.hpp
  include/bit/memory/trackers/null_tracker.hpp
  include/bit/memory/trackers/stdout_tracker.hpp

  # Bounds Checkers
  include/bit/memory/bounds_checkers/debug_bounds_checker.hpp
  include/bit/memory/bounds_checkers/null_bounds_checker.hpp

  # Lockables
  include/bit/memory/lockables/null_lock.hpp

  # Block Allocators
  include/bit/memory/block_allocators/detail/cached_block_allocator.hpp
  include/bit/memory/block_allocators/detail/named_block_allocator.hpp
  include/bit/memory/block_allocators/aligned_block_allocator.hpp
  include/bit/memory/block_allocators/any_block_allocator.hpp
  include/bit/memory/block_allocators/growing_aligned_block_allocator.hpp
  include/bit/memory/block_allocators/growing_malloc_block_allocator.hpp
  include/bit/memory/block_allocators/growing_new_block_allocator.hpp
  include/bit/memory/block_allocators/growing_virtual_block_allocator.hpp
  include/bit/memory/block_allocators/malloc_block_allocator.hpp
  include/bit/memory/block_allocators/new_block_allocator.hpp
  include/bit/memory/block_allocators/null_block_allocator.hpp
  include/bit/memory/block_allocators/policy_block_allocator.hpp
  include/bit/memory/block_allocators/stack_block_allocator.hpp
  include/bit/memory/block_allocators/static_block_allocator.hpp
  include/bit/memory/block_allocators/virtual_block_allocator.hpp

  # Allocators
  include/bit/memory/allocators/detail/named_allocator.hpp
  include/bit/memory/allocators/aligned_allocator.hpp
  include/bit/memory/allocators/aligned_offset_allocator.hpp
  include/bit/memory/allocators/any_allocator.hpp
  include/bit/memory/allocators/bump_down_allocator.hpp
  include/bit/memory/allocators/bump_down_lifo_allocator.hpp
  include/bit/memory/allocators/bump_up_allocator.hpp
  include/bit/memory/allocators/bump_up_lifo_allocator.hpp
  include/bit/memory/allocators/fallback_allocator.hpp
  include/bit/memory/allocators/policy_allocator.hpp
  include/bit/memory/allocators/malloc_allocator.hpp
  include/bit/memory/allocators/min_aligned_allocator.hpp
  include/bit/memory/allocators/new_allocator.hpp
  include/bit/memory/allocators/null_allocator.hpp
  include/bit/memory/allocators/pool_allocator.hpp
  include/bit/memory/allocators/stack_allocator.hpp
  
    
  # Allocator Storage
  include/bit/memory/allocator_storage/stateless_allocator_storage.hpp
  include/bit/memory/allocator_storage/shared_allocator_storage.hpp
  include/bit/memory/allocator_storage/referenced_allocator_storage.hpp
)

# Though the inline headers are not strictly used (or even required by CMake),
# this is added for IDEs like CLion to accurately track headers.
set(inline_headers
  # Utilities
  include/bit/memory/detail/allocator_deleter.inl
  include/bit/memory/detail/allocator_info.inl
  include/bit/memory/detail/allocator_reference.inl
  include/bit/memory/detail/allocator_traits.inl
  include/bit/memory/detail/block_allocator_traits.inl
  include/bit/memory/detail/debugging.inl
  include/bit/memory/detail/endian.inl
  include/bit/memory/detail/freelist.inl
  include/bit/memory/detail/memory_block.inl
  include/bit/memory/detail/memory_block_cache.inl
  include/bit/memory/detail/not_null.inl
  include/bit/memory/detail/pointer_utilities.inl
  include/bit/memory/detail/std_allocator_adapter.inl
  include/bit/memory/detail/std_any_allocator_adapter.inl
  include/bit/memory/detail/unaligned_memory.inl
  include/bit/memory/detail/uninitialized_storage.inl

  # Memory Allocations
  include/bit/memory/detail/virtual_memory.inl

  # Taggers
  include/bit/memory/taggers/detail/allocator_tagger.inl
  include/bit/memory/taggers/detail/block_allocator_tagger.inl

  # Trackers
  include/bit/memory/trackers/detail/detailed_leak_tracker.inl
  include/bit/memory/trackers/detail/leak_tracker.inl
  include/bit/memory/trackers/detail/stat_recording_tracker.inl
  include/bit/memory/trackers/detail/stdout_tracker.inl

  # Bounds Checkers
  include/bit/memory/bounds_checkers/detail/debug_bounds_checker.inl

  # Block Allocators
  include/bit/memory/block_allocators/detail/aligned_block_allocator.inl
  include/bit/memory/block_allocators/detail/any_block_allocator.inl
  include/bit/memory/block_allocators/detail/cached_block_allocator.inl
  include/bit/memory/block_allocators/detail/growing_aligned_block_allocator.inl
  include/bit/memory/block_allocators/detail/growing_malloc_block_allocator.inl
  include/bit/memory/block_allocators/detail/growing_new_block_allocator.inl
  include/bit/memory/block_allocators/detail/malloc_block_allocator.inl
  include/bit/memory/block_allocators/detail/named_block_allocator.inl
  include/bit/memory/block_allocators/detail/new_block_allocator.inl
  include/bit/memory/block_allocators/detail/null_block_allocator.inl
  include/bit/memory/block_allocators/detail/policy_block_allocator.inl
  include/bit/memory/block_allocators/detail/stack_block_allocator.inl
  include/bit/memory/block_allocators/detail/static_block_allocator.inl

  # Allocators
  include/bit/memory/allocators/detail/aligned_allocator.inl
  include/bit/memory/allocators/detail/aligned_offset_allocator.inl
  include/bit/memory/allocators/detail/any_allocator.inl
  include/bit/memory/allocators/detail/bump_down_allocator.inl
  include/bit/memory/allocators/detail/bump_down_lifo_allocator.inl
  include/bit/memory/allocators/detail/bump_up_allocator.inl
  include/bit/memory/allocators/detail/bump_up_lifo_allocator.inl
  include/bit/memory/allocators/detail/fallback_allocator.inl
  include/bit/memory/allocators/detail/malloc_allocator.inl
  include/bit/memory/allocators/detail/min_aligned_allocator.inl
  include/bit/memory/allocators/detail/named_allocator.inl
  include/bit/memory/allocators/detail/new_allocator.inl
  include/bit/memory/allocators/detail/null_allocator.inl
  include/bit/memory/allocators/detail/policy_allocator.inl
  include/bit/memory/allocators/detail/pool_allocator.inl
  include/bit/memory/allocators/detail/stack_allocator.inl
  
  # Allocator Storage
  include/bit/memory/allocator_storage/detail/stateless_allocator_storage.inl
  include/bit/memory/allocator_storage/detail/shared_allocator_storage.inl
  include/bit/memory/allocator_storage/detail/referenced_allocator_storage.inl
)

# Unfortunately, certain AppleClang versions don't support 'thread_local'; so to
# avoid failing independence tests, they have been appended here
if( NOT "${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang" )
  list(APPEND headers include/bit/memory/block_allocators/thread_local_block_allocator.hpp)
  list(APPEND inline_headers include/bit/memory/block_allocators/detail/thread_local_block_allocator.inl)
endif()


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
  src/bit/memory/virtual_memory.cpp

  # Block Allocators
  src/bit/memory/block_allocators/virtual_block_allocator.cpp
  src/bit/memory/block_allocators/growing_virtual_block_allocator.cpp

  # memory-specific
  ${platform_source_files}
)

#-----------------------------------------------------------------------------
# bit::memory: Library
#-----------------------------------------------------------------------------

add_library(memory ${source_files} ${headers} ${inline_headers})
add_library(bit::memory ALIAS memory)

# Add include directories
target_include_directories(memory PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
)

# Add DEBUG, NDEBUG, and RELEASE macro definitions
target_compile_definitions(memory PUBLIC
  $<$<CONFIG:DEBUG>:DEBUG>
  $<$<CONFIG:RELEASE>:RELEASE>
)

# Add compiler-specific flags
if( "${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
  target_compile_options(memory PRIVATE -Wall -Wstrict-aliasing -pedantic -Werror)
elseif( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" )
  target_compile_options(memory PRIVATE -Wall -Wstrict-aliasing -pedantic -Werror)
elseif( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC" )
  # TODO: Determine MSVC necessary compiler flags
endif()

#-----------------------------------------------------------------------------
# bit::memory : Header-self-containment Tests
#-----------------------------------------------------------------------------

if( BIT_MEMORY_COMPILE_HEADER_SELF_CONTAINMENT_TESTS )

  # Add containment test and alias as 'bit::memory::header_self_containment_test
  add_header_self_containment_test(memory_header_self_containment_test ${headers})
  add_library(bit::memory::header_self_containment_test ALIAS memory_header_self_containment_test)

  copy_target_properties(memory_header_self_containment_test memory PROPERTIES
    COMPILE_DEFINITIONS
    COMPILE_OPTIONS
    INCLUDE_DIRECTORIES
  )

endif()

#-----------------------------------------------------------------------------
# bit::memory : Unit Tests
#-----------------------------------------------------------------------------

if( BIT_MEMORY_COMPILE_UNIT_TESTS )
  add_subdirectory(test)
endif()

#-----------------------------------------------------------------------------
# bit::memory : Benchmarks
#-----------------------------------------------------------------------------

if( BIT_MEMORY_COMPILE_BENCHMARKS )
  add_subdirectory(benchmarks)
endif()

#-----------------------------------------------------------------------------
# bit::memory : Documentation
#-----------------------------------------------------------------------------

if( BIT_MEMORY_GENERATE_DOCUMENTATION )
  add_subdirectory(doc)
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
