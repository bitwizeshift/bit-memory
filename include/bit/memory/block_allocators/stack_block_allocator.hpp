/**
 * \file stack_block_allocator.hpp
 *
 * \brief This header contains the definition of a block allocator that uses
 *        the stack for temporary, automatic cleanup
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */
#ifndef BIT_MEMORY_BLOCK_ALLOCATORS_STACK_BLOCK_ALLOCATOR_HPP
#define BIT_MEMORY_BLOCK_ALLOCATORS_STACK_BLOCK_ALLOCATOR_HPP

#include "detail/named_block_allocator.hpp" // detail::named_block_allocator
#include "../macros.hpp"       // BIT_MEMORY_UNUSED
#include "../memory.hpp"       // memory, is_power_of_two
#include "../memory_block.hpp" // memory_block

#include "cached_block_allocator.hpp" // cached_block_allocator

#include <cstddef> // std::max_align_t

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
    /// \satisfies BlockAllocator
    ///////////////////////////////////////////////////////////////////////////
    template<std::size_t BlockSize,
             std::size_t Blocks=1u,
             std::size_t Align=alignof(std::max_align_t)>
    class stack_block_allocator
    {
      static_assert( Blocks > 0,"Must have at least one block" );
      static_assert( is_power_of_two(Align), "Alignment must be a power of two" );
      static_assert( BlockSize % Align == 0, "Block size must must be an increment of the block size" );

      //-----------------------------------------------------------------------
      // Public Member Types
      //-----------------------------------------------------------------------
    public:

      using block_size      = std::integral_constant<std::size_t,BlockSize>;
      using block_alignment = std::integral_constant<std::size_t,Align>;

      //----------------------------------------------------------------------
      // Constructors
      //----------------------------------------------------------------------
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

      //----------------------------------------------------------------------
      // Block Allocations
      //----------------------------------------------------------------------
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
      // Private Members
      //-----------------------------------------------------------------------
    private:

      static constexpr auto size = BlockSize * Blocks;

      alignas(Align) char m_storage[size];
      std::size_t         m_index;
    };

    //-------------------------------------------------------------------------
    // Utilities
    //-------------------------------------------------------------------------

    template<std::size_t BlockSize, std::size_t Blocks=1, std::size_t Align=alignof(std::max_align_t)>
    using cached_stack_block_allocator = cached_block_allocator<stack_block_allocator<BlockSize,Blocks,Align>>;

    //-------------------------------------------------------------------------

    template<std::size_t BlockSize, std::size_t Blocks=1, std::size_t Align=alignof(std::max_align_t)>
    using named_stack_block_allocator = detail::named_block_allocator<stack_block_allocator<BlockSize,Blocks,Align>>;

    template<std::size_t BlockSize, std::size_t Blocks=1, std::size_t Align=alignof(std::max_align_t)>
    using named_cached_stack_block_allocator = detail::named_block_allocator<cached_stack_block_allocator<BlockSize,Blocks,Align>>;

  } // namespace memory
} // namespace bit

#include "detail/stack_block_allocator.inl"

#endif /* BIT_MEMORY_BLOCK_ALLOCATORS_STACK_BLOCK_ALLOCATOR_HPP */
