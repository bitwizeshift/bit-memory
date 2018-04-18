/*****************************************************************************
 * \file
 * \brief This header defines the null block allocator, null_block_allocator
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
#ifndef BIT_MEMORY_BLOCK_ALLOCATORS_NULL_BLOCK_ALLOCATOR_HPP
#define BIT_MEMORY_BLOCK_ALLOCATORS_NULL_BLOCK_ALLOCATOR_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "detail/named_block_allocator.hpp" // detail::named_block_allocator

#include "../utilities/allocator_info.hpp"    // allocator_info
#include "../utilities/macros.hpp"            // BIT_MEMORY_UNLIKELY
#include "../utilities/memory_block.hpp"      // memory_block
#include "../utilities/owner.hpp"             // owner

#include <type_traits> // std::integral_constant, std::true_false
#include <cstddef>     // std::max_align_t

namespace bit {
  namespace memory {

    //////////////////////////////////////////////////////////////////////////
    /// \brief A block allocator that only distributes null blocks
    ///
    /// \satisfies{BlockAllocator}
    /// \satisfies{Stateless}
    //////////////////////////////////////////////////////////////////////////
    class null_block_allocator
    {
      //----------------------------------------------------------------------
      // Public Member Types
      //----------------------------------------------------------------------
    public:

      using default_block_alignment = std::integral_constant<std::size_t,1>;

      //-----------------------------------------------------------------------
      // Constructor / Assignment
      //-----------------------------------------------------------------------
    public:

      /// \brief Default-constructs a null_block_allocator
      null_block_allocator() = default;

      /// \brief Move-constructs a null_block_allocator from another allocator
      ///
      /// \param other the other null_block_allocator to move
      null_block_allocator( null_block_allocator&& other ) noexcept = default;

      /// \brief Copy-constructs a null_block_allocator from another allocator
      ///
      /// \param other the other null_block_allocator to copy
      null_block_allocator( const null_block_allocator& other ) noexcept = default;

      //-----------------------------------------------------------------------

      /// \brief Move-assigns a null_block_allocator from another allocator
      ///
      /// \param other the other null_block_allocator to move
      /// \return reference to \c (*this)
      null_block_allocator& operator=( null_block_allocator&& other ) noexcept = default;

      /// \brief Copy-assigns a null_block_allocator from another allocator
      ///
      /// \param other the other null_block_allocator to copy
      /// \return reference to \c (*this)
      null_block_allocator& operator=( const null_block_allocator& other ) noexcept = default;

      //----------------------------------------------------------------------
      // Block Allocations
      //----------------------------------------------------------------------
    public:

      /// \brief Allocates a null memory_block
      ///
      /// \return a null memory_block
      owner<memory_block> allocate_block() noexcept;

      /// \brief Deallocates a memory_block
      ///
      /// \param block the block to deallocate
      void deallocate_block( owner<memory_block> block ) noexcept;

      //-----------------------------------------------------------------------
      // Observers
      //-----------------------------------------------------------------------
    public:

      /// \brief Queries the next block size expected from this allocator
      ///
      /// \return the size of the next allocated block
      std::size_t next_block_size() const noexcept;

      /// \brief Gets the info about this allocator
      ///
      /// This defaults to 'null_block_allocator'.
      /// Use a named_null_block_allocator to override this
      ///
      /// \return the info for this allocator
      allocator_info info() const noexcept;
    };

    //-------------------------------------------------------------------------
    // Equality
    //-------------------------------------------------------------------------

    bool operator==( const null_block_allocator& lhs,
                     const null_block_allocator& rhs ) noexcept;
    bool operator!=( const null_block_allocator& lhs,
                     const null_block_allocator& rhs ) noexcept;

    //-------------------------------------------------------------------------
    // Utilities
    //-------------------------------------------------------------------------

    using named_null_block_allocator = detail::named_block_allocator<null_block_allocator>;

  } // namespace memory
} // namespace bit

#include "detail/null_block_allocator.inl"

#endif /* BIT_MEMORY_BLOCK_ALLOCATORS_NULL_BLOCK_ALLOCATOR_HPP */
