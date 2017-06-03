/**
 * \file fallback_block_allocator.hpp
 *
 * \brief todo: fill in documentation
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */

#ifndef BIT_MEMORY_FALLBACK_BLOCK_ALLOCATOR_HPP
#define BIT_MEMORY_FALLBACK_BLOCK_ALLOCATOR_HPP

#include "memory.hpp"
#include "memory_block.hpp"
#include "cached_block_allocator.hpp"
#include "debug_block_allocator.hpp"

#include <tuple>   // std::tuple
#include <utility> // std::index_sequence

namespace bit {
  namespace memory {

    //////////////////////////////////////////////////////////////////////////
    /// \brief The fallback_block_allocator allows an arbitrary number of
    ///        block allocators to be used for block allocations
    ///
    /// Any time a block allocation fails, it will fallback to the next block
    /// allocator in sequence until it reaches the end.
    ///
    /// \note The fallback_block_allocator is non-owning; and thus
    ///       all BlockAllocators must be constructed and managed outside of
    ///       this class. It is undefined behaviour if this class is used
    ///       after one of the block allocators goes out of scope.
    ///
    /// \satisfies BlockAllocator
    //////////////////////////////////////////////////////////////////////////
    template<typename BlockAllocator0, typename...BlockAllocatorN>
    class fallback_block_allocator
    {
      //----------------------------------------------------------------------
      // Constructors
      //----------------------------------------------------------------------
    public:

      /// \brief Constructs a fallback_block_allocator out of n block allocators
      ///
      /// \param a0 the first block allocator
      /// \param an the rest of the block allocators
      fallback_block_allocator( BlockAllocator0& a0, BlockAllocatorN&...an );

      fallback_block_allocator( const fallback_block_allocator& other ) = default;

      fallback_block_allocator( fallback_block_allocator&& other ) noexcept = default;

      //----------------------------------------------------------------------

      fallback_block_allocator& operator=( const fallback_block_allocator& other ) = default;

      fallback_block_allocator& operator=( fallback_block_allocator&& other ) noexcept = default;

      //----------------------------------------------------------------------
      // Block Allocations
      //----------------------------------------------------------------------
    public:

      /// \brief Allocates a null memory_block
      ///
      /// \return a null memory_block
      owner<memory_block> allocate_block();

      /// \brief Deallocates a memory_block
      ///
      /// \param block the block to deallocate
      void deallocate_block( owner<memory_block> block );

      //----------------------------------------------------------------------
      // Private Member Types
      //----------------------------------------------------------------------
    private:

      using storage_type = std::tuple<BlockAllocator0*,BlockAllocatorN*...>;

      //----------------------------------------------------------------------
      // Private Members
      //----------------------------------------------------------------------
    private:

      storage_type m_storage;

      //----------------------------------------------------------------------
      // Private Allocations
      //----------------------------------------------------------------------
    private:

      template<std::size_t Idx, std::size_t...Idxs>
      owner<memory_block> allocate_block( std::index_sequence<Idx,Idxs...> );

      template<std::size_t Idx>
      owner<memory_block> allocate_block( std::index_sequence<Idx> );

      //----------------------------------------------------------------------
      // Private Deallocations
      //----------------------------------------------------------------------
    private:

      template<std::size_t Idx, std::size_t...Idxs>
      void deallocate_block( owner<memory_block> block, std::index_sequence<Idx,Idxs...> );

      template<std::size_t Idx>
      void deallocate_block( owner<memory_block> block, std::index_sequence<Idx> );

    };

    template<typename BlockAllocator0, typename...BlockAllocatorN>
    using cached_fallback_block_allocator =
      cached_block_allocator<fallback_block_allocator<BlockAllocator0,BlockAllocatorN...>>;

    template<typename BlockAllocator0, typename...BlockAllocatorN>
    using debug_fallback_block_allocator =
      debug_block_allocator<fallback_block_allocator<BlockAllocator0,BlockAllocatorN...>>;

    template<typename BlockAllocator0, typename...BlockAllocatorN>
    using cached_debug_fallback_block_allocator =
      cached_block_allocator<debug_block_allocator<fallback_block_allocator<BlockAllocator0,BlockAllocatorN...>>>;

  } // namespace memory
} // namespace bit

#include "detail/fallback_block_allocator.inl"

#endif /* BIT_MEMORY_FALLBACK_BLOCK_ALLOCATOR_HPP */
