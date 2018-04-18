/*****************************************************************************
 * \file
 * \brief This header contains the definition for the RawAllocator,
 *        malloc_allocator
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
#ifndef BIT_MEMORY_ALLOCATORS_MALLOC_ALLOCATOR_HPP
#define BIT_MEMORY_ALLOCATORS_MALLOC_ALLOCATOR_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "detail/named_allocator.hpp" // detail::named_allocator

#include "../utilities/allocator_info.hpp" // allocator_info
#include "../utilities/owner.hpp"          // owner
#include "../utilities/macros.hpp"         // BIT_MEMORY_UNUSED

#include <cstdlib>     // std::malloc, std::free, std::size_t
#include <cstddef>     // std::max_align_t
#include <type_traits> // std::true_type

namespace bit {
  namespace memory {

    ///////////////////////////////////////////////////////////////////////////
    /// \brief This stateless allocator performs all of its allocation calls
    ///        using raw calls to 'malloc', and frees memory with 'free'
    ///
    /// Any alignment requests are ignored
    ///
    /// \satisfies{Allocator}
    /// \satisfies{Stateless}
    ///////////////////////////////////////////////////////////////////////////
    class malloc_allocator
    {
      //-----------------------------------------------------------------------
      // Public Static Members
      //-----------------------------------------------------------------------
    public:

      using default_alignment = std::integral_constant<std::size_t,alignof(std::max_align_t)>;

      //-----------------------------------------------------------------------
      // Constructor / Assignment
      //-----------------------------------------------------------------------
    public:

      /// \brief Default-constructs a malloc_allocator
      malloc_allocator() = default;

      /// \brief Move-constructs a malloc_allocator from another allocator
      ///
      /// \param other the other malloc_allocator to move
      malloc_allocator( malloc_allocator&& other ) noexcept = default;

      /// \brief Copy-constructs a malloc_allocator from another allocator
      ///
      /// \param other the other malloc_allocator to copy
      malloc_allocator( const malloc_allocator& other ) noexcept = default;

      //-----------------------------------------------------------------------

      /// \brief Move-assigns a malloc_allocator from another allocator
      ///
      /// \param other the other malloc_allocator to move
      /// \return reference to \c (*this)
      malloc_allocator& operator=( malloc_allocator&& other ) noexcept = default;

      /// \brief Copy-assigns a malloc_allocator from another allocator
      ///
      /// \param other the other malloc_allocator to copy
      /// \return reference to \c (*this)
      malloc_allocator& operator=( const malloc_allocator& other ) noexcept = default;

      //-----------------------------------------------------------------------
      // Allocations / Deallocation
      //-----------------------------------------------------------------------
    public:

      /// \brief Attempts to allocate memory of size \p size, returning nullptr
      ///        on failure
      ///
      /// The alignment is ignored for calls to this allocator. The alignment
      /// is always guaranteed to be at least \c alignof(std::max_align_t)
      ///
      /// \param size the size of this allocation
      /// \param align the requested alignment (ignored)
      /// \return the allocated pointer
      owner<void*> try_allocate( std::size_t size, std::size_t align ) noexcept;

      /// \brief Deallocates a pointer \p p with the allocation size of \p size
      ///
      /// \param p the pointer to deallocate
      /// \param size the size to deallocate
      void deallocate( owner<void*> p, std::size_t size );

      //-----------------------------------------------------------------------
      // Observers
      //-----------------------------------------------------------------------
    public:

      /// \brief Gets the info about this allocator
      ///
      /// This defaults to 'malloc_allocator'. Use a
      /// named_malloc_allocator to override this
      ///
      /// \return the info for this allocator
      allocator_info info() const noexcept;
    };

    //-------------------------------------------------------------------------
    // Equality
    //-------------------------------------------------------------------------

    /// \{
    /// \brief Compares equality between two malloc_allocators
    ///
    /// Two malloc_allocators are always considered the same
    bool operator==( const malloc_allocator& lhs,
                     const malloc_allocator& rhs ) noexcept;
    bool operator!=( const malloc_allocator& lhs,
                     const malloc_allocator& rhs ) noexcept;
    /// \}

    //-------------------------------------------------------------------------
    // Utilities
    //-------------------------------------------------------------------------

    using named_malloc_allocator = detail::named_allocator<malloc_allocator>;

  } // namespace memory
} // namespace bit

#include "detail/malloc_allocator.inl"

#endif /* BIT_MEMORY_ALLOCATORS_MALLOC_ALLOCATOR_HPP */
