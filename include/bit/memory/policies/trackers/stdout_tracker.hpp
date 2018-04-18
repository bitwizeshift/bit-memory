/*****************************************************************************
 * \file
 * \brief TODO: Add description
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
#ifndef BIT_MEMORY_POLICIES_TRACKERS_TRACKERS_STDOUT_TRACKER_HPP
#define BIT_MEMORY_POLICIES_TRACKERS_TRACKERS_STDOUT_TRACKER_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

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
