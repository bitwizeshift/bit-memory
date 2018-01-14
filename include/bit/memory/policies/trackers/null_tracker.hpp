/**
 * \file null_tracker.hpp
 *
 * \brief This header contains the definition of a null (no-op) MemoryTracker
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */
#ifndef BIT_MEMORY_POLICIES_TRACKERS_NULL_TRACKER_HPP
#define BIT_MEMORY_POLICIES_TRACKERS_NULL_TRACKER_HPP

#include "detail/stat_recording_tracker.hpp" // detail::stat_recording_tracker

#include "../../utilities/allocator_info.hpp"   // allocator_info
#include "../../concepts/MemoryTracker.hpp"     // is_memory_tracker

#include <cstddef> // std::size_t

namespace bit {
  namespace memory {

    ///////////////////////////////////////////////////////////////////////////
    /// \brief This is a null (no-op) memory tracker meant for composition in
    ///        the arena_allocator
    ///
    /// \satisfies{MemoryTracker}
    ///////////////////////////////////////////////////////////////////////////
    struct null_tracker
    {
      //-----------------------------------------------------------------------
      // Tracking
      //-----------------------------------------------------------------------
    public:

      void on_allocate( void*, std::size_t, std::size_t ){}
      void on_deallocate( const allocator_info&, void*, std::size_t ){}
      void on_deallocate_all(){}
      void finalize( const allocator_info& ){}
    };

    using stat_recording_null_tracker = detail::stat_recording_tracker<null_tracker>;

    static_assert( is_memory_tracker_v<null_tracker>,
                   "null_tracker must satisfy MemoryTracker" );
    static_assert( is_memory_tracker_v<stat_recording_null_tracker>,
                   "stat_recording_null_tracker must satisfy MemoryTracker" );

  } // namespace memory
} // namespace bit

#endif /* BIT_MEMORY_POLICIES_TRACKERS_NULL_TRACKER_HPP */
