/**
 * \file stdout_tracker.hpp
 *
 * \brief TODO: Add description
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */
#ifndef BIT_MEMORY_POLICIES_TRACKERS_TRACKERS_STDOUT_TRACKER_HPP
#define BIT_MEMORY_POLICIES_TRACKERS_TRACKERS_STDOUT_TRACKER_HPP

#include "../../utilities/allocator_info.hpp" // allocator_info
#include "../../utilities/macros.hpp"         // BIT_MEMORY_UNUSED

#include "../../concepts/MemoryTracker.hpp"   // is_memory_tracker

#include <cstdio> // std::ptrintf
#include <cstddef> // std::size_t

namespace bit {
  namespace memory {

    ///////////////////////////////////////////////////////////////////////////
    /// \brief A short squawker tracking policy that output all logging to
    ///        stdout.
    ///////////////////////////////////////////////////////////////////////////
    struct stdout_tracker
    {
      /// \brief Logs the allocation to stdout
      ///
      /// \param p the pointer to the memory to allocate
      /// \param bytes the size of the allocation, in bytes
      /// \param align the alignment of the allocation
      void on_allocate( void* p, std::size_t bytes, std::size_t align ) noexcept;

      /// \brief Logs the deallocation to stdout
      ///
      /// \param info the info for the allocator deallocating
      /// \param p the pointer to the memory to deallocate
      /// \param bytes the size of the deallocation, in bytes
      void on_deallocate( const allocator_info& info, void* p, std::size_t bytes ) noexcept;

      /// \brief Logs the truncated deallocation to stdout
      void on_deallocate_all() noexcept;

      /// \brief Logs to stdout that the finalization occurred
      ///
      /// \param info the allocator info
      void finalize( const allocator_info& info ) noexcept;
    };

    static_assert( is_memory_tracker_v<stdout_tracker>,
                   "stdout_tracker must satisfy MemoryTracker" );

  } // namespace memory
} // namespace bit

#include "detail/stdout_tracker.inl"

#endif /* BIT_MEMORY_POLICIES_TRACKERS_TRACKERS_STDOUT_TRACKER_HPP */
