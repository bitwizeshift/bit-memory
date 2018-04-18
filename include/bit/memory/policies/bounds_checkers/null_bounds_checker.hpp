/*****************************************************************************
 * \file
 * \brief This header contains the definition of a null (no-operation)
 *        BoundsChecker
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
#ifndef BIT_MEMORY_POLICIES_BOUNDS_CHECKERS_NULL_BOUNDS_CHECKER_HPP
#define BIT_MEMORY_POLICIES_BOUNDS_CHECKERS_NULL_BOUNDS_CHECKER_HPP

#include "../../utilities/allocator_info.hpp" // allocator_info

#include "../../concepts/BoundsChecker.hpp" // is_bounds_checker

#include <cstddef>               // std::size_t

namespace bit {
  namespace memory {

    ///////////////////////////////////////////////////////////////////////////
    /// \brief This bounds checker performs no operation, and is intended to
    ///        be used for non-debug variations of the arena_allocator
    ///
    /// \satisfies{BoundsChecker}
    ///////////////////////////////////////////////////////////////////////////
    class null_bounds_checker
    {
      //-----------------------------------------------------------------------
      // Public Static Members
      //-----------------------------------------------------------------------
    public:

      static constexpr std::size_t front_size = 0;
      static constexpr std::size_t back_size  = 0;

      //-----------------------------------------------------------------------
      // Preparation
      //-----------------------------------------------------------------------
    public:

      void prepare_front_fence( void*, std::size_t ){}
      void prepare_back_fence( void*, std::size_t ){}

      //-----------------------------------------------------------------------
      // Checking
      //-----------------------------------------------------------------------
    public:

      void check_front_fence( const allocator_info&, void*, std::size_t ){}
      void check_back_fence( const allocator_info&, void*, std::size_t ){}
    };

    static_assert( is_bounds_checker_v<null_bounds_checker>,
                   "null_bounds_checker must satisfy BoundsChecker" );

  } // namespace memory
} // namespace bit

#endif /* BIT_MEMORY_POLICIES_BOUNDS_CHECKERS_NULL_BOUNDS_CHECKER_HPP */
