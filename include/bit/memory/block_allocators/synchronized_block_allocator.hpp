/**
 * \file synchronized_block_allocator
 *
 * \brief This header contains a BlockAllocator wrapper that adds thread-safety
 *        to block allocators
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */

#ifndef BIT_MEMORY_SYNCHRONIZED_BLOCK_ALLOCATOR_HPP
#define BIT_MEMORY_SYNCHRONIZED_BLOCK_ALLOCATOR_HPP

#include "../memory.hpp"
#include "../memory_block.hpp"

#include "../detail/ebo_storage.hpp"

#include <mutex> // std::lock_guard

namespace bit {
  namespace memory {

    //////////////////////////////////////////////////////////////////////////
    /// \brief A wrapper for BlockAllocators that adds thread-safety through
    ///        allowing a locking primitive type
    ///
    /// \tparam BlockAllocator the block allocator
    /// \tparam BasicLockable the underlying lockable type
    ///
    /// \satisfies BlockAllocator
    //////////////////////////////////////////////////////////////////////////
    template<typename BlockAllocator, typename BasicLockable>
    class synchronized_block_allocator
      : private detail::ebo_storage<BlockAllocator,BasicLockable>
    {
      using base_type = detail::ebo_storage<BlockAllocator,BasicLockable>;

      //----------------------------------------------------------------------
      // thread_safe_block_allocator
      //----------------------------------------------------------------------
    public:

      synchronized_block_allocator() = default;

      template<typename Tuple0, typename Tuple1>
      synchronized_block_allocator( std::piecewise_construct_t,
                                    Tuple0&& allocator_args,
                                    Tuple1&& lock_args );

      //----------------------------------------------------------------------
      // Block Allocations
      //----------------------------------------------------------------------
    public:

      /// \brief Allocates a block from the underlying BlockAllocator
      ///
      /// \return an allocated memory_block
      owner<memory_block> allocate_block();

      /// \brief Deallocates a memory_block previously allocated by this
      ///        thread_safe_block_allocator
      ///
      /// \param block the block to deallocate
      void deallocate_block( owner<memory_block> block );
    };

  } // namespace memory
} // namespace bit

#include "detail/synchronized_block_allocator.inl"

#endif /* BIT_MEMORY_SYNCHRONIZED_BLOCK_ALLOCATOR_HPP */
