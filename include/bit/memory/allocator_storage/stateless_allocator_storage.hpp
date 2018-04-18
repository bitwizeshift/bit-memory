/*****************************************************************************
 * \file
 * \brief This header contains a storage policy for stateless allocators
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
#ifndef BIT_MEMORY_ALLOCATOR_STORAGE_STATELESS_ALLOCATOR_STORAGE_HPP
#define BIT_MEMORY_ALLOCATOR_STORAGE_STATELESS_ALLOCATOR_STORAGE_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "../utilities/ebo_storage.hpp"

#include "../concepts/Stateless.hpp"
#include "../concepts/Allocator.hpp"

namespace bit {
  namespace memory {

    ///////////////////////////////////////////////////////////////////////////
    /// \brief A storage type for holding onto stateless allocators
    ///
    /// \satisfies{AllocatorStorage}
    ///
    /// \tparam StatelessAllocator an \ref Allocator type that additionally
    ///         satisfies \ref Stateless
    ///////////////////////////////////////////////////////////////////////////
    template<typename StatelessAllocator>
    class stateless_allocator_storage
      : private ebo_storage<StatelessAllocator>
    {
      using base_type = ebo_storage<StatelessAllocator>;

      static_assert( is_stateless<StatelessAllocator>::value &&
                     is_allocator<StatelessAllocator>::value,
                     "stateless_allocator_storage can only be used with"
                     "Allocator types that satisfy Stateless and Allocator");

      //-----------------------------------------------------------------------
      // Public Member Types
      //-----------------------------------------------------------------------
    public:

      using allocator_type = StatelessAllocator;

      //-----------------------------------------------------------------------
      // Constructors / Assignment
      //-----------------------------------------------------------------------
    public:

      /// \brief Default-constructs the stateless_allocator_storage
      stateless_allocator_storage() = default;

      /// \brief Move-constructs this shared_allocator_storage from an
      ///        existing one
      ///
      /// \param other the other storage to move
      stateless_allocator_storage( stateless_allocator_storage&& other ) noexcept = default;

      /// \brief Copy-constructs this shared_allocator_storage from an
      ///        existing one
      ///
      /// \param other the other storage to copy
      stateless_allocator_storage( const stateless_allocator_storage& other ) noexcept = default;

      //-----------------------------------------------------------------------

      /// \brief Move-assigns this stateless_allocator_storage from an
      ///        existing one
      ///
      /// \param other the other storage to move
      /// \return reference to \c (*this)
      stateless_allocator_storage& operator=( stateless_allocator_storage&& other ) noexcept = default;

      /// \brief Copy-assigns this stateless_allocator_storage from an
      ///        existing one
      ///
      /// \param other the other storage to copy
      /// \return reference to \c (*this)
      stateless_allocator_storage& operator=( const stateless_allocator_storage& other ) noexcept = default;

      //-----------------------------------------------------------------------
      // Observers
      //-----------------------------------------------------------------------
    public:

      /// \{
      /// \brief Gets the stored allocator
      ///
      /// \return reference to the stored allocator
      StatelessAllocator& get_allocator() noexcept;
      const StatelessAllocator& get_allocator() const noexcept;
      /// \}
    };

    //-------------------------------------------------------------------------
    // Utilities
    //-------------------------------------------------------------------------

    /// \brief Makes a stateless_allocator_storage of the given allocator type
    ///
    /// \note This function is mostly superflous, since the constructor of
    ///       stateless_allocator_storage does the same functionality. This
    ///       exists for symmetry with the other storage 'make' functions
    ///
    /// \return the stateless allocator storage
    template<typename Allocator>
    stateless_allocator_storage<Allocator> make_stateless_allocator_storage();

  } // namespace memory
} // namespace bit

#include "detail/stateless_allocator_storage.inl"

#endif /* BIT_MEMORY_ALLOCATOR_STORAGE_STATELESS_ALLOCATOR_STORAGE_HPP */
