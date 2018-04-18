/*****************************************************************************
 * \file
 * \brief This header contains the implementation of an allocator used for
 *        offset aligned allocations
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
#ifndef BIT_MEMORY_ALLOCATORS_ALIGNED_OFFSET_ALLOCATOR_HPP
#define BIT_MEMORY_ALLOCATORS_ALIGNED_OFFSET_ALLOCATOR_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "detail/named_allocator.hpp" // detail::named_allocator

#include "../utilities/allocator_info.hpp" // allocator_info
#include "../utilities/macros.hpp"         // BIT_MEMORY_UNUSED
#include "../utilities/owner.hpp"          // owner

#include "../regions/aligned_heap_memory.hpp" // aligned_malloc, aligned_free

#include <cstddef>     // std::max_align_t
#include <type_traits> // std::true_type

namespace bit {
  namespace memory {

    ///////////////////////////////////////////////////////////////////////////
    /// \brief This stateless allocator performs all of its allocation calls
    ///        using aligned memory invocations aligned to an offset
    ///
    /// \satisfies{ExtendedAllocator}
    /// \satisfies{Stateless}
    ///////////////////////////////////////////////////////////////////////////
    class aligned_offset_allocator
    {
      //-----------------------------------------------------------------------
      // Public Static Members
      //-----------------------------------------------------------------------
    public:

      using default_alignment = std::integral_constant<std::size_t,1>;

      //-----------------------------------------------------------------------
      // Constructor / Assignment
      //-----------------------------------------------------------------------
    public:

      /// \brief Default-constructs a aligned_offset_allocator
      aligned_offset_allocator() = default;

      /// \brief Move-constructs a aligned_offset_allocator from another allocator
      ///
      /// \param other the other aligned_offset_allocator to move
      aligned_offset_allocator( aligned_offset_allocator&& other ) noexcept = default;

      /// \brief Copy-constructs a aligned_offset_allocator from another allocator
      ///
      /// \param other the other aligned_offset_allocator to copy
      aligned_offset_allocator( const aligned_offset_allocator& other ) noexcept = default;

      //-----------------------------------------------------------------------

      /// \brief Move-assigns an aligned_offset_allocator from another allocator
      ///
      /// \param other the other aligned_offset_allocator to copy
      /// \return reference to \c (*this)
      aligned_offset_allocator& operator=( aligned_offset_allocator&& other ) noexcept = default;

      /// \brief Copy-assigns an aligned_offset_allocator from another allocator
      ///
      /// \param other the other aligned_offset_allocator to copy
      /// \return reference to \c (*this)
      aligned_offset_allocator& operator=( const aligned_offset_allocator& other ) noexcept = default;

      //-----------------------------------------------------------------------
      // Allocations / Deallocation
      //-----------------------------------------------------------------------
    public:

      /// \brief Allocates aligned memory of size \p size, with alignment to a
      ///        boundary of at least \p align, aligned to an offset
      ///
      /// \param size the size of this allocation
      /// \param align the requested alignment
      /// \param offset the offset to align to
      /// \return the allocated pointer, or nullptr on failure
      owner<void*> try_allocate( std::size_t size,
                                 std::size_t align,
                                 std::size_t offset = 0 ) noexcept;

      /// \brief Deallocates a pointer \p p with the allocation size of \p size
      ///
      /// \param p the pointer to deallocate
      /// \param size the size to deallocate
      void deallocate( owner<void*> p, std::size_t size );

      //---------------------------------------------------------------------
      // Observers
      //---------------------------------------------------------------------
    public:

      /// \brief Gets the info about this allocator
      ///
      /// This defaults to 'aligned_offset_allocator'. Use a
      /// named_aligned_offset_allocator to override this
      ///
      /// \return the info for this allocator
      allocator_info info() const noexcept;
    };

    //-------------------------------------------------------------------------
    // Equality
    //-------------------------------------------------------------------------

    /// \{
    /// \brief Compares equality between two aligned_allocator
    ///
    /// Two aligned_allocator are always considered the same
    bool operator==( const aligned_offset_allocator& lhs,
                     const aligned_offset_allocator& rhs ) noexcept;
    bool operator!=( const aligned_offset_allocator& lhs,
                     const aligned_offset_allocator& rhs ) noexcept;
    /// \}

    //-------------------------------------------------------------------------
    // Utilities
    //-------------------------------------------------------------------------

    using named_aligned_allocator = detail::named_allocator<aligned_offset_allocator>;

  } // namespace memory
} // namespace bit

#include "detail/aligned_offset_allocator.inl"

#endif /* BIT_MEMORY_ALLOCATORS_ALIGNED_OFFSET_ALLOCATOR_HPP */
