/*****************************************************************************
 * \file
 * \brief This header contains the definition of a stack allocator for
 *        temporary, automatic reclamation
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
#ifndef BIT_MEMORY_ALLOCATORS_STACK_ALLOCATOR_HPP
#define BIT_MEMORY_ALLOCATORS_STACK_ALLOCATOR_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "detail/named_allocator.hpp" // detail::named_allocator

#include "../utilities/macros.hpp"            // BIT_MEMORY_UNLIKELY
#include "../utilities/pointer_utilities.hpp" // offset_align_forward

#include <cstddef>     // std::size_t, std::max_align_t
#include <type_traits> // std::true_type, std::integral_constant
#include <cassert>     // assert

namespace bit {
  namespace memory {

    ///////////////////////////////////////////////////////////////////////////
    /// \brief A stack-allocation strategy that allocates off of stack-memory
    ///
    /// This offers two different forms of deallocations; either truncated
    /// deallocations via deallocate_all, or individual deallocations through
    /// deallocate(...) calls. The order of deallocation must be the inverse
    /// of the order of construction, otherwise it is undefined behaviour.
    ///
    /// \note Since this allocator uses stack memory, care should be taken to
    ///       avoid overflowing the stack
    ///
    /// \tparam Size The size of the storage in this allocator
    /// \tparam Align the alignment of the storage
    ///
    /// \satisfies{ExtendedAllocator}
    ///////////////////////////////////////////////////////////////////////////
    template<std::size_t Size, std::size_t Align = alignof(std::max_align_t)>
    class stack_allocator
    {
      //-----------------------------------------------------------------------
      // Public Member Types
      //-----------------------------------------------------------------------
    public:

      using max_alignment = std::integral_constant<std::size_t,Align>;

      //-----------------------------------------------------------------------
      // Constructors
      //-----------------------------------------------------------------------
    public:

      /// \brief Default-constructs a stack_allocator
      stack_allocator() noexcept;

      // Deleted move constructor.
      stack_allocator( stack_allocator&& other ) = delete;

      // Deleted copy constructor
      stack_allocator( const stack_allocator& other ) = delete;

      //----------------------------------------------------------------------

      /// \brief Deallocates all memory from the stack
      ~stack_allocator();

      //----------------------------------------------------------------------

      // Deleted copy assignment
      stack_allocator& operator=( const stack_allocator& ) = delete;

      // Deleted move assignment
      stack_allocator& operator=( stack_allocator&& other ) = delete;

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
      void* try_allocate( std::size_t size,
                          std::size_t align,
                          std::size_t offset = 0 ) noexcept;

      /// \brief Does nothing for linear_allocator. Use deallocate_all
      ///
      /// \param p the pointer
      /// \param size the size of the allocation
      void deallocate( void* p, std::size_t size );

      /// \brief Deallocates all memory in this allocator
      void deallocate_all();

      //-----------------------------------------------------------------------
      // Observers
      //-----------------------------------------------------------------------
    public:

      /// \brief Checks whether \p linear_allocator contains the pointer \p p
      ///
      /// \param p the pointer to check
      /// \return \c true if \p p is contained in this allocator
      bool owns( const void* p ) const noexcept;

      /// \brief Gets the info about this allocator
      ///
      /// This defaults to 'stack_allocator'. Use a
      /// named_stack_allocator to override this
      ///
      /// \return the info for this allocator
      allocator_info info() const noexcept;

      //-----------------------------------------------------------------------
      // Private Members
      //-----------------------------------------------------------------------
    private:

      alignas(Align) char m_storage[Size];
      void* m_current;
    };

    //-------------------------------------------------------------------------
    // Utilities
    //-------------------------------------------------------------------------

    template<std::size_t Size, std::size_t Align = alignof(std::max_align_t)>
    using named_stack_allocator = detail::named_allocator<stack_allocator<Size,Align>>;

  } // namespace memory
} // namespace bit

#include "detail/stack_allocator.inl"

#endif /* BIT_MEMORY_ALLOCATORS_STACK_ALLOCATOR_HPP */
