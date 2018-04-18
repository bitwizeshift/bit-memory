/*****************************************************************************
 * \file
 * \brief This header contains the definition of a block allocator that uses
 *        the stack for temporary, automatic cleanup
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
#ifndef BIT_MEMORY_BLOCK_ALLOCATORS_STACK_BLOCK_ALLOCATOR_HPP
#define BIT_MEMORY_BLOCK_ALLOCATORS_STACK_BLOCK_ALLOCATOR_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "detail/named_block_allocator.hpp" // detail::named_block_allocator

#include "../utilities/allocator_info.hpp"     // allocator_info
#include "../utilities/macros.hpp"             // BIT_MEMORY_UNUSED
#include "../utilities/memory_block.hpp"       // memory_block
#include "../utilities/memory_block_cache.hpp" // memory_block_cache
#include "../utilities/owner.hpp"              // owner
#include "../utilities/pointer_utilities.hpp"  // is_power_of_two

#include <cstddef> // std::max_align_t
#include <cassert> // assert

namespace bit {
  namespace memory {

    ///////////////////////////////////////////////////////////////////////////
    /// \brief An allocator with automatic storage reclamation.
    ///
    /// Memory is allocated from the stack, rather than from the heap.
    ///
    /// \note Caution should be used when allocating memory from the stack,
    ///       otherwise a stack-overflow condition may occur
    ///
    /// \tparam BlockSize The size of each block
    /// \tparam Blocks the number of blocks in the block allocator
    /// \tparam Align the alignment of the blocks
    ///
    /// \satisfies{BlockAllocator}
    ///////////////////////////////////////////////////////////////////////////
    template<std::size_t BlockSize,
             std::size_t Blocks=1u,
             std::size_t Align=alignof(std::max_align_t)>
    class stack_block_allocator
    {
      static_assert( Blocks > 0,
                     "Must have at least one block" );
      static_assert( is_power_of_two(Align),
                     "Alignment must be a power of two" );
      static_assert( Blocks == 1 || BlockSize % Align == 0,
                     "Block size must must be an increment of the alignment" );

      //-----------------------------------------------------------------------
      // Public Member Types
      //-----------------------------------------------------------------------
    public:

      using block_size      = std::integral_constant<std::size_t,BlockSize>;
      using block_alignment = std::integral_constant<std::size_t,Align>;

      //-----------------------------------------------------------------------
      // Constructors
      //-----------------------------------------------------------------------
    public:

      /// \brief Constructs a stack_block_allocator that will distribute
      ///        blocks of the specified \p size
      ///
      /// \param size the size of each block allocation
      explicit stack_block_allocator() noexcept;

      // Deleted move constructor
      stack_block_allocator( stack_block_allocator&& other ) = delete;

      // Deleted copy constructor
      stack_block_allocator( const stack_block_allocator& other ) = delete;

      //-----------------------------------------------------------------------

      // Deleted move assignment
      stack_block_allocator& operator=( stack_block_allocator&& other ) = delete;

      // Deleted copy assignment
      stack_block_allocator& operator=( const stack_block_allocator& other ) = delete;

      //-----------------------------------------------------------------------
      // Block Allocations
      //-----------------------------------------------------------------------
    public:

      /// \brief Allocates stack memory
      ///
      /// \return the memory block
      owner<memory_block> allocate_block() noexcept;

      /// \brief Deallocates a stack memory block
      ///
      /// \note This call is unnecessary, since the stack_block_allocator has
      ///       automatic RAII cleanup.
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
      /// This defaults to 'stack_block_allocator'.
      /// Use a named_stack_block_allocator to override this
      ///
      /// \return the info for this allocator
      allocator_info info() const noexcept;

      //-----------------------------------------------------------------------
      // Private Members
      //-----------------------------------------------------------------------
    private:

      static constexpr auto s_storage_size = BlockSize * Blocks;

      alignas(Align) char m_storage[s_storage_size];
      memory_block_cache  m_cache;
    };

    //-------------------------------------------------------------------------
    // Utilities
    //-------------------------------------------------------------------------

    template<std::size_t BlockSize, std::size_t Blocks=1, std::size_t Align=alignof(std::max_align_t)>
    using named_stack_block_allocator = detail::named_block_allocator<stack_block_allocator<BlockSize,Blocks,Align>>;

  } // namespace memory
} // namespace bit

#include "detail/stack_block_allocator.inl"

#endif /* BIT_MEMORY_BLOCK_ALLOCATORS_STACK_BLOCK_ALLOCATOR_HPP */
