/*****************************************************************************
 * \file
 * \brief This header contains the definition of the ExtendedAllocator class,
 *        bump_up_allocator.
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
#ifndef BIT_MEMORY_ALLOCATORS_BUMP_UP_ALLOCATOR_HPP
#define BIT_MEMORY_ALLOCATORS_BUMP_UP_ALLOCATOR_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "detail/named_allocator.hpp" // detail::named_allocator

#include "../utilities/macros.hpp"            // BIT_MEMORY_UNLIKELY
#include "../utilities/memory_block.hpp"      // memory_block
#include "../utilities/owner.hpp"             // owner
#include "../utilities/pointer_utilities.hpp" // offset_align_forward

#include <cassert>     // assert
#include <cstddef>     // std::size_t, std::ptrdiff_t
#include <type_traits> // std:integral_constant, std::true_type, etc

namespace bit {
  namespace memory {

    ///////////////////////////////////////////////////////////////////////////
    /// \brief An allocator that allocates memory in contiguous memory by
    ///        'bumping' the head pointer to higher memory addresses
    ///
    /// Allocations are distributed in an increasing memory-address pattern.
    ///
    /// This allocator can only deallocate memory with truncated deallocations
    /// through \c deallocate_all
    ///
    /// \satisfies{ExtendedAllocator}
    ///////////////////////////////////////////////////////////////////////////
    class bump_up_allocator
    {
      //-----------------------------------------------------------------------
      // Public Member Types
      //-----------------------------------------------------------------------
    public:

      using max_alignment = std::integral_constant<std::size_t,(1 << (sizeof(std::size_t)-1))>;

      //-----------------------------------------------------------------------
      // Constructors
      //-----------------------------------------------------------------------
    public:

      /// \brief Constructs a bump_up_allocator
      explicit bump_up_allocator( memory_block block ) noexcept;

      /// \brief Move-constructs a bump_up_allocator from another allocator
      ///
      /// \param other the other bump_up_allocator to move
      bump_up_allocator( bump_up_allocator&& other ) noexcept = default;

      // Deleted copy constructor
      bump_up_allocator( const bump_up_allocator& other ) = delete;

      // Deleted nullblock constructor
      bump_up_allocator( nullblock_t ) = delete;

      //----------------------------------------------------------------------

      // Deleted copy assignment
      bump_up_allocator& operator=( const bump_up_allocator& ) = delete;

      // Deleted move assignment
      bump_up_allocator& operator=( bump_up_allocator&& other ) = delete;

      //-----------------------------------------------------------------------
      // Allocations / Deallocations
      //-----------------------------------------------------------------------
    public:

      /// \brief Tries to allocate memory of size \p size, aligned to the
      ///        boundary \p align, offset by \p offset
      ///
      /// \param size the size of the allocation
      /// \param align the requested alignment of the allocation
      /// \param offset the amount to offset the alignment by
      /// \return the allocated pointer on success, \c nullptr on failure
      owner<void*> try_allocate( std::size_t size,
                                 std::size_t align,
                                 std::size_t offset = 0 ) noexcept;

      /// \brief Does nothing for bump_up_allocator. Use deallocate_all
      ///
      /// \param p the pointer
      /// \param size the size of the allocation
      void deallocate( owner<void*> p, std::size_t size );

      /// \brief Deallocates everything from this allocator
      void deallocate_all() noexcept;

      //-----------------------------------------------------------------------
      // Observers
      //-----------------------------------------------------------------------
    public:

      /// \brief Checks whether \p bump_up_allocator contains the pointer \p p
      ///
      /// \param p the pointer to check
      /// \return \c true if \p p is contained in this allocator
      bool owns( const void* p ) const noexcept;

      /// \brief Gets the info about this allocator
      ///
      /// This defaults to 'bump_up_allocator'. Use a
      /// named_bump_up_allocator to override this
      ///
      /// \return the info for this allocator
      allocator_info info() const noexcept;

      //-----------------------------------------------------------------------
      // Private Members
      //-----------------------------------------------------------------------
    private:

      memory_block m_block;
      void*        m_current;
    };

    //-------------------------------------------------------------------------
    // Utilities
    //-------------------------------------------------------------------------

    using named_bump_up_allocator = detail::named_allocator<bump_up_allocator>;

  } // namespace memory
} // namespace bit

#include "detail/bump_up_allocator.inl"

#endif /* BIT_MEMORY_ALLOCATORS_BUMP_UP_ALLOCATOR_HPP */
