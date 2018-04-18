/*****************************************************************************
 * \file
 * \brief This header contains the definition for the RawAllocator,
 *        new_allocator
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
#ifndef BIT_MEMORY_ALLOCATORS_NEW_ALLOCATOR_HPP
#define BIT_MEMORY_ALLOCATORS_NEW_ALLOCATOR_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "detail/named_allocator.hpp" // detail::named_allocator

#include "../utilities/allocator_info.hpp" // allocator_info
#include "../utilities/macros.hpp"         // BIT_MEMORY_UNUSED
#include "../utilities/owner.hpp"          // owner

#include <cstddef>     // std::size_t, std::max_align_t
#include <new>         // ::operator new, ::operator delete, std::nothrow
#include <type_traits> // std::true_type

namespace bit {
  namespace memory {

    ///////////////////////////////////////////////////////////////////////////
    /// \brief This stateless allocator performs all of its allocation calls
    ///        using raw calls to 'new', and frees memory with 'delete'
    ///
    /// Any alignment requests are ignored
    ///
    /// \satisfies{Allocator}
    /// \satisfies{Stateless}
    ///////////////////////////////////////////////////////////////////////////
    class new_allocator
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

      /// \brief Default-constructs a new_allocator
      new_allocator() = default;

      /// \brief Move-constructs a new_allocator from another allocator
      ///
      /// \param other the other new_allocator to move
      new_allocator( new_allocator&& other ) noexcept = default;

      /// \brief Copy-constructs a new_allocator from another allocator
      ///
      /// \param other the other new_allocator to copy
      new_allocator( const new_allocator& other ) noexcept = default;

      //-----------------------------------------------------------------------

      /// \brief Move-assigns a new_allocator from another allocator
      ///
      /// \param other the other new_allocator to move
      /// \return reference to \c (*this)
      new_allocator& operator=( new_allocator&& other ) noexcept = default;

      /// \brief Copy-assigns a new_allocator from another allocator
      ///
      /// \param other the other new_allocator to copy
      /// \return reference to \c (*this)
      new_allocator& operator=( const new_allocator& other ) noexcept = default;

      //-----------------------------------------------------------------------
      // Allocations
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
    /// \brief Compares equality between two new_allocators
    ///
    /// Two new_allocators are always considered the same
    bool operator==( const new_allocator&, const new_allocator& ) noexcept;
    bool operator!=( const new_allocator&, const new_allocator& ) noexcept;
    /// \}

    //-------------------------------------------------------------------------
    // Utilities
    //-------------------------------------------------------------------------

    using named_new_allocator = detail::named_allocator<new_allocator>;

  } // namespace memory
} // namespace bit

#include "detail/new_allocator.inl"

#endif /* BIT_MEMORY_ALLOCATORS_NEW_ALLOCATOR_HPP */
