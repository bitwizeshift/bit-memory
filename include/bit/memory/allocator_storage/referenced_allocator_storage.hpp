/*****************************************************************************
 * \file
 * \brief This header contains a storage policy using allocators accessed by
 *        a reference
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
#ifndef BIT_MEMORY_ALLOCATOR_STORAGE_REFERENCED_ALLOCATOR_STORAGE_HPP
#define BIT_MEMORY_ALLOCATOR_STORAGE_REFERENCED_ALLOCATOR_STORAGE_HPP

#include "../concepts/Allocator.hpp"

#include <memory> // std::addressof

namespace bit {
  namespace memory {

    ///////////////////////////////////////////////////////////////////////////
    /// \brief A storage type for holding onto allocators by a reference.
    ///
    /// The lifetime of the underlying referenced allocator is the same as the
    /// original allocator being referenced; meaning that this storage type
    /// should be treated carefully to avoid dangling references.
    ///
    /// \satisfies{AllocatorStorage}
    ///
    /// \tparam Allocator an \ref Allocator type
    ///////////////////////////////////////////////////////////////////////////
    template<typename Allocator>
    class referenced_allocator_storage
    {
      static_assert( is_allocator<Allocator>::value,
                     "referenced_allocator_storage can only be used with"
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

      /// \brief Constructs a referenced_allocator_storage from an allocator
      ///
      /// \param alloc the allocator to reference
      explicit referenced_allocator_storage( Allocator& alloc ) noexcept;

      /// \brief Move-constructs this referenced_allocator_storage from an
      ///        existing one
      ///
      /// \param other the other storage to move
      referenced_allocator_storage( referenced_allocator_storage&& other ) noexcept = default;

      /// \brief Copy-constructs this referenced_allocator_storage from an
      ///        existing one
      ///
      /// \param other the other storage to copy
      referenced_allocator_storage( const referenced_allocator_storage& other ) noexcept = default;

      //-----------------------------------------------------------------------

      /// \brief Move-assigns this referenced_allocator_storage from an
      ///        existing one
      ///
      /// \param other the other storage to move
      /// \return reference to \c (*this)
      referenced_allocator_storage& operator=( referenced_allocator_storage&& other ) noexcept = default;

      /// \brief Copy-assigns this referenced_allocator_storage from an
      ///        existing one
      ///
      /// \param other the other storage to copy
      /// \return reference to \c (*this)
      referenced_allocator_storage& operator=( const referenced_allocator_storage& other ) noexcept = default;

      //-----------------------------------------------------------------------
      // Observers
      //-----------------------------------------------------------------------
    public:

      /// \{
      /// \brief Gets the stored allocator
      ///
      /// \return reference to the stored allocator
      Allocator& get_allocator() noexcept;
      const Allocator& get_allocator() const noexcept;
      /// \}

      //-----------------------------------------------------------------------
      // Private Members
      //-----------------------------------------------------------------------
    private:

      Allocator* m_allocator;
    };

    //-------------------------------------------------------------------------
    // Utilities
    //-------------------------------------------------------------------------

    /// \brief Makes a referenced_allocator_storage of the given allocator
    ///
    /// \param alloc the allocator to reference
    /// \return the stateless allocator storage
    template<typename Allocator>
    referenced_allocator_storage<Allocator>
      make_referenced_allocator_storage( Allocator& alloc ) noexcept;

  } // namespace memory
} // namespace bit

#include "detail/referenced_allocator_storage.inl"

#endif /* BIT_MEMORY_ALLOCATOR_STORAGE_REFERENCED_ALLOCATOR_STORAGE_HPP */
