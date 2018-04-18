/*****************************************************************************
 * \file
 * \brief This header contains the implementation of a debug bounds checker
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
#ifndef BIT_MEMORY_POLICIES_BOUNDS_CHECKERS_DEBUG_BOUNDS_CHECKER_HPP
#define BIT_MEMORY_POLICIES_BOUNDS_CHECKERS_DEBUG_BOUNDS_CHECKER_HPP

#include "../../utilities/allocator_info.hpp" // allocator_info
#include "../../utilities/debugging.hpp"      // debug_tag_start_bytes, etc
#include "../../utilities/errors.hpp"         // get_buffer_overflow_handler

#include "../../concepts/BoundsChecker.hpp" // is_bounds_checker

#include <cstddef> // std::size_t

namespace bit {
  namespace memory {

    ///////////////////////////////////////////////////////////////////////////
    /// \brief This type is used for checking boundaries around memory
    ///        allocations to determine if buffer-overflows have occurred
    ///
    /// On any failure, the buffer overflow handler will be called
    ///
    /// \satisfies{BoundsChecker}
    ///////////////////////////////////////////////////////////////////////////
    template<std::size_t FrontSize, std::size_t BackSize = FrontSize>
    class debug_bounds_checker
    {
      static_assert( FrontSize > 0, "FrontSize must be greater than 0" );
      static_assert( BackSize > 0, "BackSize must be greater than 0" );

      //-----------------------------------------------------------------------
      // Public Static Members
      //-----------------------------------------------------------------------
    public:

      static constexpr std::size_t front_size = FrontSize;
      static constexpr std::size_t back_size  = BackSize;

      //-----------------------------------------------------------------------
      // Preparation
      //-----------------------------------------------------------------------
    public:

      void prepare_front_fence( void* p, std::size_t size ) noexcept;

      void prepare_back_fence( void* p, std::size_t size ) noexcept;

      //-----------------------------------------------------------------------
      // Checking
      //-----------------------------------------------------------------------
    public:

      void check_front_fence( const allocator_info& info,
                              void* p,
                              std::size_t size ) noexcept;

      void check_back_fence( const allocator_info& info,
                             void* p,
                             std::size_t size ) noexcept;
    };

    static_assert( is_bounds_checker_v<debug_bounds_checker<1,1>>,
                   "debug_bounds_checker must satisfy BoundsChecker" );

  } // namespace memory
} // namespace bit

#include "detail/debug_bounds_checker.inl"

#endif /* BIT_MEMORY_POLICIES_BOUNDS_CHECKERS_DEBUG_BOUNDS_CHECKER_HPP */
