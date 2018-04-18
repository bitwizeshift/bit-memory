/*****************************************************************************
 * \file
 * \brief This header contains definitions for an intrinsically linked list
 *        of available memory_blocks
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
#ifndef BIT_MEMORY_UTILITIES_MEMORY_BLOCK_CACHE_HPP
#define BIT_MEMORY_UTILITIES_MEMORY_BLOCK_CACHE_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "owner.hpp"                 // owner
#include "memory_block.hpp"          // memory_block
#include "pointer_utilities.hpp"     // align_of
#include "uninitialized_storage.hpp" // uninitialized_construct_at

#include <cassert> // assert

namespace bit {
  namespace memory {

    ///////////////////////////////////////////////////////////////////////////
    /// \brief A cache containing an intrinsically linked list of
    ///        memory_blocks
    ///
    /// Blocks in this cache are not guaranteed to all be of the same size.
    /// Memory blocks may originate from different allocators, and represent
    /// different regions of memory -- however this is not the recommended
    /// practice.
    ///
    /// Every memory_block in the memory_block_cache must be aligned to at
    /// least \c alignof(memory_block) bytes -- otherwise it is undefined
    /// behavior
    ///////////////////////////////////////////////////////////////////////////
    class memory_block_cache
    {
      //-----------------------------------------------------------------------
      // Constructor / Assignment
      //-----------------------------------------------------------------------
    public:

      /// \brief Default constructs a block cache
      memory_block_cache() noexcept;

      /// \brief Move-constructs a memory_block_cache from an existing one
      ///
      /// \param other the other memory_block_cache to move
      memory_block_cache( memory_block_cache&& other ) noexcept = default;

      // Deleted copy constructor
      memory_block_cache( const memory_block_cache& other ) = delete;

      //-----------------------------------------------------------------------

      // Deleted move assignment
      memory_block_cache& operator=( memory_block_cache&& other ) = delete;

      // Deleted copy assignment
      memory_block_cache& operator=( const memory_block_cache& other ) = delete;

      //-----------------------------------------------------------------------
      // Observers
      //-----------------------------------------------------------------------
    public:

      /// \brief Returns whether or not this memory_block_cache is empty
      ///
      /// \return \c true if this memory_block_cache is empty, \c false otherwise
      bool empty() const noexcept;

      /// \brief Returns the number of memory_blocks in this cache
      ///
      /// This function is lazily computed, and is written with \c O(n)
      /// complexity
      ///
      /// \return the number of memory_blocks in this cache
      std::size_t size() const noexcept;

      /// \brief Returns the size in bytes of all the memory_blocks in the
      ///        cache
      /// \return the size in bytes
      std::size_t size_bytes() const noexcept;

      /// \brief Evaluates whether \p ptr exists within this memory_block_cache
      ///
      /// \param ptr the pointer to evaluate
      /// \return \c true whether \p ptr
      bool contains( const void* ptr ) const noexcept;

      //-----------------------------------------------------------------------
      // Element Access
      //-----------------------------------------------------------------------
    public:

      /// \brief Views the front memory block of this cache
      ///
      /// \pre !empty()
      ///
      /// \note It is undefined behaviour to invoke this function if the cache
      ///       is empty
      ///
      /// \return the front cache entry
      const memory_block& peek() const noexcept;

      /// \brief Requests a block from the current block cache.
      ///
      /// If one can be provided, the block is released from this cache and is
      /// now owned by the accessor
      ///
      /// \return a memory block, if available, or a null block if unable
      owner<memory_block> request_block() noexcept;

      /// \brief Requests a block from
      ///
      /// \param alloc the allocator
      /// \return
      template<typename BlockAllocator>
      owner<memory_block> request_block( BlockAllocator& alloc );

      /// \brief Acquires an entry from another cache
      ///
      /// \param other the cache to acquire the entry from
      void steal_block( memory_block_cache& other ) noexcept;

      /// \brief Stores an allocated block inside this memory_block_cache
      ///
      /// \pre \c block.data() points to memory that is aligned to at least
      ///      \c alignof(memory_block) bytes.
      ///
      /// \pre \c block points to a valid memory_block
      ///
      /// \param block the block to store
      void store_block( owner<memory_block> block ) noexcept;

      //----------------------------------------------------------------------
      // Modifiers
      //----------------------------------------------------------------------
    public:

      /// \brief Swaps this with another block cache
      ///
      /// \param other the other memory_block_cache to swap with
      void swap( memory_block_cache& other ) noexcept;

      //----------------------------------------------------------------------
      // Private Members
      //----------------------------------------------------------------------
    private:

      memory_block m_head; ///< The head of the cache
    };

    //------------------------------------------------------------------------
    // Public Functions
    //------------------------------------------------------------------------

    /// \brief Swaps two memory block caches
    ///
    /// \param lhs the left memory_block_cache
    /// \param rhs the right memory_block_cache
    void swap( memory_block_cache& lhs, memory_block_cache& rhs ) noexcept;

  } // namespace memory
} // namespace bit

#include "detail/memory_block_cache.inl"

#endif /* BIT_MEMORY_UTILITIES_MEMORY_BLOCK_CACHE_HPP */
