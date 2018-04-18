/*****************************************************************************
 * \file
 * \brief This header contains an AllocatorStorage type that has
 *        shared-ownership semantics of the underlying allocator
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
#ifndef BIT_MEMORY_ALLOCATOR_STORAGE_SHARED_ALLOCATOR_STORAGE_HPP
#define BIT_MEMORY_ALLOCATOR_STORAGE_SHARED_ALLOCATOR_STORAGE_HPP

#include "../concepts/Allocator.hpp"

#include <memory> // std::shared_ptr

namespace bit {
  namespace memory {

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Shared-ownership storage of a heap-allocated allocator
    ///
    /// \satisfies{AllocatorStorage}
    ///
    /// \tparam Allocator an \ref Allocator type
    ///////////////////////////////////////////////////////////////////////////
    template<typename Allocator>
    class shared_allocator_storage
    {
      static_assert( is_allocator<Allocator>::value,
                     "shared_allocator_storage can only be used with"
                     "types that satisfy the Allocator concept");

      //-----------------------------------------------------------------------
      // Public Member Types
      //-----------------------------------------------------------------------
    public:

      using allocator_type = Allocator;

      //-----------------------------------------------------------------------
      // Constructors / Assignment
      //-----------------------------------------------------------------------
    public:

      /// \brief Constructs a shared_allocator_storage from a shared pointer
      ///
      /// \param alloc the shared pointer to the allocator
      explicit shared_allocator_storage( std::shared_ptr<Allocator> alloc ) noexcept;

      /// \brief Move-constructs this shared_allocator_storage from an
      ///        existing one
      ///
      /// \param other the other storage to move
      shared_allocator_storage( shared_allocator_storage&& other ) = default;

      /// \brief Copy-constructs this shared_allocator_storage from an
      ///        existing one
      ///
      /// \param other the other storage to copy
      shared_allocator_storage( const shared_allocator_storage& other ) = default;

      //-----------------------------------------------------------------------

      /// \brief Move-assigns this shared_allocator_storage from an
      ///        existing one
      ///
      /// \param other the other storage to move
      /// \return reference to \c (*this)
      shared_allocator_storage& operator=( shared_allocator_storage&& ) = default;

      /// \brief Copy-assigns this shared_allocator_storage from an
      ///        existing one
      ///
      /// \param other the other storage to copy
      /// \return reference to \c (*this)
      shared_allocator_storage& operator=( const shared_allocator_storage& ) = default;

      //-----------------------------------------------------------------------
      // Observers
      //-----------------------------------------------------------------------
    public:

      /// \{
      /// \brief Retrieves a reference to the underlying allocator
      ///
      /// \return the reference to the allocator
      Allocator& get_allocator() noexcept;
      const Allocator& get_allocator() const noexcept;
      /// \}

      //-----------------------------------------------------------------------
      // Private Members
      //-----------------------------------------------------------------------
    private:

      std::shared_ptr<Allocator> m_allocator;
    };

    //-------------------------------------------------------------------------
    // Utilities
    //-------------------------------------------------------------------------

    template<typename Allocator, typename...Args>
    shared_allocator_storage<Allocator>
      make_shared_allocator_storage( Args&&...args );

    template<typename Allocator, typename UAllocator, typename...Args>
    shared_allocator_storage<Allocator>
      allocate_shared_allocator_storage( UAllocator& alloc, Args&&...args );

  } // namespace memory
} // namespace bit

#include "detail/shared_allocator_storage.inl"

#endif /* BIT_MEMORY_ALLOCATOR_STORAGE_SHARED_ALLOCATOR_STORAGE_HPP */
