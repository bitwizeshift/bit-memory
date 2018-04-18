/*****************************************************************************
 * \file
 * \brief This header contains the definition of a basic leak tracker
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
#ifndef BIT_MEMORY_POLICIES_TRACKERS_LEAK_TRACKER_HPP
#define BIT_MEMORY_POLICIES_TRACKERS_LEAK_TRACKER_HPP

#include "detail/stat_recording_tracker.hpp" // detail::stat_recording_tracker

#include "../../utilities/allocator_info.hpp" // allocator_info
#include "../../utilities/errors.hpp"         // get_leak_handler
#include "../../utilities/macros.hpp"         // BIT_MEMORY_UNUSED

#include "../../concepts/MemoryTracker.hpp" // is_memory_tracker

#include <cstddef> // std::size_t, std::ptrdiff_t

namespace bit {
  namespace memory {

    ///////////////////////////////////////////////////////////////////////////
    /// \brief This class is a basic leak-tracker for allocations
    ///
    /// \satisfies{MemoryTracker}
    ///////////////////////////////////////////////////////////////////////////
    class leak_tracker
    {
      //-----------------------------------------------------------------------
      // Tracking
      //-----------------------------------------------------------------------
    public:

      /// \brief Records an allocation after allocating the size
      ///
      /// \param p the pointer to the memory to allocate
      /// \param bytes the size of the allocation, in bytes
      /// \param align the alignment of the allocation
      void on_allocate( void* p, std::size_t bytes, std::size_t align ) noexcept;

      /// \brief Records the deallocation before deallocating the bytes
      ///
      /// \param info the info for the allocator deallocating
      /// \param p the pointer to the memory to deallocate
      /// \param bytes the size of the deallocation, in bytes
      void on_deallocate( const allocator_info& info, void* p, std::size_t bytes ) noexcept;

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

    static_assert( is_memory_tracker_v<leak_tracker>,
                   "leak_tracker must satisfy MemoryTracker" );
    static_assert( is_memory_tracker_v<stat_recording_leak_tracker>,
                   "stat_recording_leak_tracker must satisfy MemoryTracker" );

  } // namespace memory
} // namespace bit

#include "detail/leak_tracker.inl"

#endif /* BIT_MEMORY_POLICIES_TRACKERS_LEAK_TRACKER_HPP */
