/*****************************************************************************
 * \file
 * \brief This header contains the trait type \c extended_allocator_traits that
 *        provides access to capabilities required for an extended allocator
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
#ifndef BIT_MEMORY_TRAITS_EXTENDED_ALLOCATOR_TRAITS_HPP
#define BIT_MEMORY_TRAITS_EXTENDED_ALLOCATOR_TRAITS_HPP

#include "allocator_traits.hpp"

#include "../utilities/allocator_info.hpp" // allocator_info
#include "../utilities/errors.hpp"         // get_out_of_memory_handler
#include "../utilities/macros.hpp"         // BIT_MEMORY_UNUSED

#include "../concepts/ExtendedAllocator.hpp" // allocator_has_extended_try_allocate

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

namespace bit {
  namespace memory {

    ///////////////////////////////////////////////////////////////////////////
    /// \brief The extended_allocator_traits class template provides a
    ///        standardized way to access the extneded allocator functionality
    ///
    /// This extends the existing functionality of 'allocator_traits' by
    /// including extended allocation facilities which allow for offset
    /// allocations.
    ///
    /// \tparam Allocator the allocator type. Must satisfy ExtendedAllocator
    ///                   concept
    ///////////////////////////////////////////////////////////////////////////
    template<typename Allocator>
    class extended_allocator_traits : public allocator_traits<Allocator>
    {
      using base_type = allocator_traits<Allocator>;

      static_assert( is_extended_allocator<Allocator>::value,
                     "Allocator must be an ExtendedAllocator" );

      //-----------------------------------------------------------------------
      // Public Members
      //-----------------------------------------------------------------------
    public:

      // aliases
      using pointer         = typename base_type::pointer;
      using const_pointer   = typename base_type::const_pointer;
      using size_type       = typename base_type::size_type;
      using difference_type = typename base_type::difference_type;

      // rebinding
      template<typename T>
      using pointer_rebind = typename base_type::template pointer_rebind<T>;

      // properties
      using default_alignment          = typename base_type::default_alignment;
      using max_alignment              = typename base_type::max_alignment;
      using can_truncate_deallocations = typename base_type::can_truncate_deallocations;
      using knows_ownership            = typename base_type::knows_ownership;
      using uses_pretty_pointers       = typename base_type::uses_pretty_pointers;

      //-----------------------------------------------------------------------
      // Allocation
      //-----------------------------------------------------------------------
    public:

      /// \{
      /// \brief Attempts to allocate memory of at least \p size bytes,
      ///        aligned to \p align boundary with an offset of \p offset bytes
      ///
      /// On failure, this function returns \p nullptr
      ///
      /// \param alloc the allocator to allocate from
      /// \param hint pointer to a nearby memory location to allocate near
      /// \param size the size of the allocation
      /// \param align the alignment of the allocation
      /// \param offset the offset of the allocation
      /// \return the pointer to the allocated memory
      static pointer try_allocate( Allocator& alloc,
                                   size_type size,
                                   size_type align,
                                   size_type offset ) noexcept;
      static pointer try_allocate( Allocator& alloc,
                                   const_pointer hint,
                                   size_type size,
                                   size_type align,
                                   size_type offset ) noexcept;
      /// \}

      //-----------------------------------------------------------------------

      /// \{
      /// \brief Allocates memory of at least \p size bytes, aligned to \p
      ///        align boundary with an offset of \p offset bytes
      ///
      /// On failure, this function may throw or invoke the out_of_memory
      /// handler before returning \p nullptr
      ///
      /// \param alloc the allocator to allocate from
      /// \param hint pointer to a nearby memory location to allocate near
      /// \param size the size of the allocation
      /// \param align the alignment of the allocation
      /// \param offset the offset of the allocation
      /// \return the pointer to the allocated member
      static pointer allocate( Allocator& alloc,
                               size_type size,
                               size_type align,
                               size_type offset );
      static pointer allocate( Allocator& alloc,
                               const_pointer hint,
                               size_type size,
                               size_type align,
                               size_type offset );
      /// \}
    };

  } // namespace memory
} // namespace bit

#include "detail/extended_allocator_traits.inl"

#endif /* BIT_MEMORY_TRAITS_EXTENDED_ALLOCATOR_TRAITS_HPP */
