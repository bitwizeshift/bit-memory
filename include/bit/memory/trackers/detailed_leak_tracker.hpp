/**
 * \file detailed_leak_tracker.hpp
 *
 * \brief This header contains a detailed memory tracker used to watch every
 *        individual allocation
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */
#ifndef BIT_MEMORY_TRACKERS_DETAILED_LEAK_TRACKER_HPP
#define BIT_MEMORY_TRACKERS_DETAILED_LEAK_TRACKER_HPP

#include "detail/stat_recording_tracker.hpp" // detail::stat_recording_tracker
#include "../allocator_info.hpp"             // allocator_info

#include <cstddef> // std::size_t, std::ptrdiff_t
#include <map>     // std::map

namespace bit {
  namespace memory {

    ///////////////////////////////////////////////////////////////////////////
    /// \brief This tracker watches individual allocations to determine which
    ///        allocations may be missing a corresponding deallocation
    ///
    /// \satisfies MemoryTracker
    ///////////////////////////////////////////////////////////////////////////
    class detailed_leak_tracker
    {
      //-----------------------------------------------------------------------
      // Tracking
      //-----------------------------------------------------------------------
    public:

      /// \brief Records an allocation of the pointer \p p with size \p bytes
      ///
      /// \param p the pointer
      /// \param bytes the size of the memory, in bytes
      void on_allocate( void* p, std::size_t bytes ) noexcept;

      /// \brief Records a deallocation of the pointer \p p with size \p bytes
      ///
      /// \param p the pointer
      /// \param bytes the size of the memory, in bytes
      void on_deallocate( void* p, std::size_t bytes ) noexcept;

      /// \brief Records all allocations being truncated into a single
      ///        deallocation
      ///
      /// This clears all open records of allocations
      void on_deallocate_all() noexcept;

      /// \brief Finalizes the leak tracker, determining any leaks that may
      ///        have occurred
      ///
      /// \param info the allocator_info for the tracker
      void finalize( const allocator_info& info );

      //-----------------------------------------------------------------------
      // Private Members
      //-----------------------------------------------------------------------
    private:

      std::map<void*,std::size_t> m_allocations;
      std::ptrdiff_t m_allocated = 0;
    };

    using stat_recording_detailed_tracker = detail::stat_recording_tracker<detailed_leak_tracker>;

  } // namespace memory
} // namespace bit

#include "detail/detailed_leak_tracker.inl"

#endif /* BIT_MEMORY_TRACKERS_DETAILED_LEAK_TRACKER_HPP */
