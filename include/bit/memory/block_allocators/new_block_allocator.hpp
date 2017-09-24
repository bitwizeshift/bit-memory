/**
 * \file new_block_allocator.hpp
 *
 * \brief This header contains an API for allocating memory blocks that use
 *        ::operator new as the underlying allocation scheme.
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */

#ifndef BIT_MEMORY_BLOCK_ALLOCATORS_NEW_BLOCK_ALLOCATOR_HPP
#define BIT_MEMORY_BLOCK_ALLOCATORS_NEW_BLOCK_ALLOCATOR_HPP

#include "../memory.hpp"
#include "../memory_block.hpp"
#include "cached_block_allocator.hpp"
#include "debug_block_allocator.hpp"

namespace bit {
  namespace memory {

    //////////////////////////////////////////////////////////////////////////
    /// \brief A block allocator that uses ::operator new(...) to allocate
    ///        memory blocks
    ///
    /// \satisfies BlockAllocator
    //////////////////////////////////////////////////////////////////////////
    class new_block_allocator
    {
      //----------------------------------------------------------------------
      // Constructors
      //----------------------------------------------------------------------
    public:

      /// \brief Constructs a new_block_allocator that will distribute
      ///        blocks of the specified \p size
      ///
      /// \param size the size of each block allocation
      explicit new_block_allocator( std::size_t size ) noexcept;

      //----------------------------------------------------------------------
      // Block Allocations
      //----------------------------------------------------------------------
    public:

      /// \brief Allocates a new'ed memory_block
      ///
      /// \return a null memory_block
      owner<memory_block> allocate_block();

      /// \brief Deallocates a new'ed memory_block
      ///
      /// \param block the block to deallocate
      void deallocate_block( owner<memory_block> block );

      //----------------------------------------------------------------------
      // Private Members
      //----------------------------------------------------------------------
    private:

      std::size_t m_size;
    };

    using cached_new_block_allocator = cached_block_allocator<new_block_allocator>;
    using debug_new_block_allocator  = debug_block_allocator<new_block_allocator>;
    using cached_debug_new_block_allocator = cached_block_allocator<debug_block_allocator<new_block_allocator>>;

  } // namespace memory
} // namespace bit

#endif /* BIT_MEMORY_BLOCK_ALLOCATORS_NEW_BLOCK_ALLOCATOR_HPP */
