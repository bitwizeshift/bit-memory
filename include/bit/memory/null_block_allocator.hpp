/**
 * \file null_block_allocator.hpp
 *
 * \brief todo: fill in documentation
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */

#ifndef BIT_MEMORY_NULL_BLOCK_ALLOCATOR_HPP
#define BIT_MEMORY_NULL_BLOCK_ALLOCATOR_HPP

#include "memory.hpp"
#include "memory_block.hpp"

namespace bit {
  namespace memory {

    //////////////////////////////////////////////////////////////////////////
    /// \brief A block allocator that only distributes null blocks
    ///
    /// \satisfies BlockAllocator
    //////////////////////////////////////////////////////////////////////////
    class null_block_allocator
    {
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

    };
  } // namespace memory
} // namespace bit

#include "detail/null_block_allocator.inl"

#endif /* BIT_MEMORY_NULL_BLOCK_ALLOCATOR_HPP */
