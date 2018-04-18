/*****************************************************************************
 * \file
 * \brief This header contains the definition of a null (no-op) MemoryTracker
 *****************************************************************************/

/*
  The MIT License (MIT)

  Copyright (c) 2018 Matthew Rodusek

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
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
