/**
 * \file null_tracker.hpp
 *
 * \brief This header contains the definition of a null (no-op) MemoryTracker
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */
#ifndef BIT_MEMORY_TRACKERS_NULL_TRACKER_HPP
#define BIT_MEMORY_TRACKERS_NULL_TRACKER_HPP

#include "detail/stat_recording_tracker.hpp" // detail::stat_recording_tracker
#include "../allocator_info.hpp"             // allocator_info

#include <cstddef> // std::size_t

namespace bit {
  namespace memory {

    ///////////////////////////////////////////////////////////////////////////
    /// \brief This is a null (no-op) memory tracker meant for composition in
    ///        the arena_allocator
    ///
    /// \satisfies MemoryTracker
    ///////////////////////////////////////////////////////////////////////////
    struct null_tracker
    {
      //-----------------------------------------------------------------------
      // Tracking
      //-----------------------------------------------------------------------
    public:

      void on_allocate( void*, std::size_t ){}
      void on_deallocate( void*, std::size_t ){}
      void finalize( const allocator_info& ){}
    };

    using stat_recording_null_tracker = detail::stat_recording_tracker<null_tracker>;

  } // namespace memory
} // namespace bit

#endif /* BIT_MEMORY_TRACKERS_NULL_TRACKER_HPP */
