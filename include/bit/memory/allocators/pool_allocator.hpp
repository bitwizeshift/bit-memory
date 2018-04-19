/*****************************************************************************
 * \file
 * \brief This header contains the definition for an allocator that creates
 *        fixed-sized allocations from a reused pool
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
#ifndef BIT_MEMORY_ALLOCATORS_POOL_ALLOCATOR_HPP
#define BIT_MEMORY_ALLOCATORS_POOL_ALLOCATOR_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "detail/named_allocator.hpp" // detail::named_allocator

#include "../utilities/freelist.hpp"          // freelist
#include "../utilities/macros.hpp"            // BIT_MEMORY_ASSUME
#include "../utilities/memory_block.hpp"      // memory_block
#include "../utilities/owner.hpp"             // owner
#include "../utilities/pointer_utilities.hpp" // is_power_of_two

#include <cassert>

namespace bit {
  namespace memory {

    ///////////////////////////////////////////////////////////////////////////
    /// \brief This allocator creates a pool of fixed-sized chunk entries for
    ///        allocations
    ///
    /// \satisfies{ExtendedAllocator}
    ///////////////////////////////////////////////////////////////////////////
    class pool_allocator
    {
      // TODO: Add template-argument for chunk-size

      //-----------------------------------------------------------------------
      // Public Member Types
      //-----------------------------------------------------------------------
    public:

      /// The max alignment is limited to 128 bytes due to an internal
      /// requirement that it stores the offset information
      using max_alignment = std::integral_constant<std::size_t,128>;

      //-----------------------------------------------------------------------
      // Constructors / Assignment
      //-----------------------------------------------------------------------
    public:

      /// \brief Constructs a pool allocator with chunk sizes of \p chunk_size,
      ///        in the arena indicated by \p block
      ///
      /// \param chunk_size the size of each entry in the pool allocator
      /// \param block the block to allocate from
      pool_allocator( std::size_t chunk_size, memory_block block );

      /// \brief Move-constructs the pool_allocator from another allocator
      ///
      /// \param other the other allocator to move
      pool_allocator( pool_allocator&& other ) noexcept = default;

      // Deleted copy construction
      pool_allocator( const pool_allocator& other ) = delete;

      //-----------------------------------------------------------------------

      // Deleted move assignment
      pool_allocator& operator=( pool_allocator&& other ) = delete;

      // Deleted copy assignment
      pool_allocator& operator=( const pool_allocator& other ) = delete;

      //-----------------------------------------------------------------------
      // Allocations / Deallocations
      //-----------------------------------------------------------------------
    public:

      /// \brief Tries to allocate \p size bytes with the alignment of \p align,
      ///        offset by \p offset
      ///
      /// \param size the requested size of the allocation
      /// \param align the requested alignment of the allocation
      /// \param offset the amount to offset the alignment
      /// \return pointer to the allocated memory, or \c nullptr on failure
      owner<void*> try_allocate( std::size_t size,
                                 std::size_t align,
                                 std::size_t offset = 0 ) noexcept;

      /// \brief Deallocates memory previously allocated from a call to
      ///        \c try_allocate
      ///
      /// \param p the pointer to the memory to deallocate
      /// \param size the size of the memory previously provided to try_allocate
      void deallocate( owner<void*> p, std::size_t size );

      /// \brief Deallocates all memory in this pool_allocator
      void deallocate_all();

      //-----------------------------------------------------------------------
      // Observers
      //-----------------------------------------------------------------------
    public:

      /// \brief Determines whether the pointer \p p is owned by this allocator
      ///
      /// \return \c true if the pointer \p p is originally from this allocator
      bool owns( const void* p ) const noexcept;

      /// \brief Determines the max size that this allocator can allocate
      ///
      /// \return the max size
      std::size_t max_size() const noexcept;

      //----------------------------------------------------------------------

      /// \brief Gets the info about this allocator
      ///
      /// This defaults to 'pool_allocator'. Use a
      /// named_pool_allocator to override this
      ///
      /// \return the info for this allocator
      allocator_info info() const noexcept;

      //-----------------------------------------------------------------------
      // Private Members
      //-----------------------------------------------------------------------
    private:

      freelist     m_freelist;
      memory_block m_block;
      std::size_t  m_chunk_size;

      //-----------------------------------------------------------------------
      // Private Member Functions
      //-----------------------------------------------------------------------
    private:

      /// \brief Creates the pool of instances to be used by the allocator
      void create_pool();
    };

    //-------------------------------------------------------------------------
    // Utilities
    //-------------------------------------------------------------------------

    using named_pool_allocator = detail::named_allocator<pool_allocator>;

  } // namespace memory
} // namespace bit

#include "detail/pool_allocator.inl"

#endif /* BIT_MEMORY_ALLOCATORS_POOL_ALLOCATOR_HPP */
