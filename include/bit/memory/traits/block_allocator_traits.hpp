/*****************************************************************************
 * \file
 * \brief This header defines traits for block_allocators
 *
 * Currently, this library does not contain any optional features
 * that a block_allocator may define; but is defined and used for
 * ensuring forward-compatibility as things change
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
#ifndef BIT_MEMORY_TRAITS_BLOCK_ALLOCATOR_TRAITS_HPP
#define BIT_MEMORY_TRAITS_BLOCK_ALLOCATOR_TRAITS_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "../utilities/allocator_info.hpp" // allocator_info
#include "../utilities/macros.hpp"         // BIT_MEMORY_UNUSED
#include "../utilities/memory_block.hpp"   // memory_block
#include "../utilities/owner.hpp"          // owner

#include "../concepts/BlockAllocator.hpp" // is_block_allocator

#include <type_traits> // std::true_type, std::false_type, etc
#include <cstddef>     // std::size_t, std::ptrdiff_t
#include <memory>      // std::addressof
#include <typeinfo>    // std::type_info

namespace bit {
  namespace memory {

    //////////////////////////////////////////////////////////////////////////
    /// \brief The block_allocator_traits class template provides a
    ///        standardized way to access block allocator functionality
    ///
    /// This will infer defaults for any optional values that may not be
    /// present in the allocator
    ///
    /// \note In the present version of this library, BlockAllocator does not
    ///       define any optional variations. This is here for forward-
    ///       compatibility.
    ///
    /// \tparam BlockAllocator the block allocator type. Must satisfy
    ///         BlockAllocator concept
    //////////////////////////////////////////////////////////////////////////
    template<typename BlockAllocator>
    class block_allocator_traits
    {
      static_assert( is_block_allocator<BlockAllocator>::value, "BlockAllocator must be a BlockAllocator" );

      //----------------------------------------------------------------------
      // Public Member Types
      //----------------------------------------------------------------------
    public:

      using has_block_alignment = block_allocator_has_default_block_alignment<BlockAllocator>;

      //----------------------------------------------------------------------
      // Block Allocations
      //----------------------------------------------------------------------
    public:

      /// \brief Allocates a block from the underlying allocator
      ///
      /// \param alloc the allocator to allocate from
      /// \return the memory block
      static owner<memory_block> allocate_block( BlockAllocator& alloc );

      /// \brief Deallocates a block from the underlying container
      ///
      /// \param alloc the allocator to deallocate from
      /// \param block the block to deallocate
      static void deallocate_block( BlockAllocator& alloc,
                                    owner<memory_block> block );

      //----------------------------------------------------------------------
      // Observers
      //----------------------------------------------------------------------
    public:

      /// \brief Gets the name of the specified block allocator
      ///
      /// \note Not all allocators are nameable or have a name specified.
      ///       For these allocators, the string returned is
      ///       \c typeid(BlockAllocator).name()
      ///
      /// \note The lifetime of the pointer returned is unmanaged, and is NOT
      ///       the responsibility of the caller to free.
      ///
      /// \param alloc the block allocator to get the name of
      /// \return the name of the allocator
      static allocator_info info( const BlockAllocator& alloc ) noexcept;

      /// \brief Gets the size of the next block from the block allocator
      ///
      /// \param alloc the block allocator to get the block size
      /// \return the next block size from the allocator
      static std::size_t next_block_size( const BlockAllocator& alloc ) noexcept;

      /// \brief Gets the alignment of the next block from the block allocator
      ///
      /// If the block alignment is statically defined as 'default_alignment',
      /// then this value is returned. Otherwise it falls back to
      /// 'next_block_alignment()' if it exists; and returning '1' if it
      /// cannot be determined.
      ///
      /// \param alloc the block allocator to get the block alignment
      /// \return the next block size from the allocator
      static std::size_t next_block_alignment( const BlockAllocator& alloc ) noexcept;

      //----------------------------------------------------------------------
      // Private Implementation
      //----------------------------------------------------------------------
    private:

      /// \{
      /// \brief Determines the info for the allocator, either by calling
      ///        \c BlockAllocator::info or by assuming "Unnamed" for the name
      ///
      /// \param alloc the allocator
      /// \return the name of the allocator
      static allocator_info do_info( std::true_type,
                                     const BlockAllocator& alloc );
      static allocator_info do_info( std::false_type,
                                     const BlockAllocator& alloc );
      /// \}

      static std::size_t do_next_block_size( std::true_type,
                                             const BlockAllocator& alloc );
      static std::size_t do_next_block_size( std::false_type,
                                             const BlockAllocator& alloc );

      static std::size_t do_next_block_align_from_type( std::true_type,
                                                        const BlockAllocator& alloc );
      static std::size_t do_next_block_align_from_type( std::false_type,
                                                        const BlockAllocator& alloc);
      static std::size_t do_next_block_align_from_fn( std::true_type,
                                                      const BlockAllocator& alloc );
      static std::size_t do_next_block_align_from_fn( std::false_type,
                                                      const BlockAllocator& alloc );
    };

  } // namespace memory
} // namespace bit

#include "detail/block_allocator_traits.inl"

#endif /* BIT_MEMORY_TRAITS_BLOCK_ALLOCATOR_TRAITS_HPP */
