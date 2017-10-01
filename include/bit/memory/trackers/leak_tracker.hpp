/**
 * \file leak_tracker.hpp
 *
 * \brief This header contains the definition of a basic leak tracker
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */
#ifndef BIT_MEMORY_TRACKERS_LEAK_TRACKER_HPP
#define BIT_MEMORY_TRACKERS_LEAK_TRACKER_HPP

#include "detail/stat_recording_tracker.hpp" // detail::stat_recording_tracker
#include "../allocator_info.hpp"             // allocator_info

#include <cstddef> // std::size_t, std::ptrdiff_t

namespace bit {
  namespace memory {

    ///////////////////////////////////////////////////////////////////////////
    /// \brief This class is a basic leak-tracker for allocations
    ///
    /// \satisfies MemoryTracker
    ///////////////////////////////////////////////////////////////////////////
    class leak_tracker
    {
      //-----------------------------------------------------------------------
      // Tracking
      //-----------------------------------------------------------------------
    public:

      /// \brief Records an allocation after allocating the size
      ///
      /// \param bytes the size of the allocation, in bytes
      void on_allocate( void*, std::size_t bytes ) noexcept;

      /// \brief Records the deallocation before deallocating the bytes
      ///
      /// \param bytes the size of the deallocation, in bytes
      void on_deallocate( void*, std::size_t bytes ) noexcept;

      /// \brief Records when deallocations have been truncated
      void on_deallocate_all() noexcept;

      /// \brief Finalizes the tracking (called during destruction)
      ///
      /// \param info the info about the allocator that is finalizing the
      ///             tracking
      void finalize( const allocator_info& info );

      //-----------------------------------------------------------------------
      // Private Members
      //-----------------------------------------------------------------------
    private:

      std::ptrdiff_t m_allocated = 0;
    };

    using stat_recording_leak_tracker = detail::stat_recording_tracker<leak_tracker>;

  } // namespace memory
} // namespace bit

#include "detail/leak_tracker.inl"

#endif /* BIT_MEMORY_TRACKERS_LEAK_TRACKER_HPP */
