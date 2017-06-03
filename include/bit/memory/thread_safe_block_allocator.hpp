/**
 * \file thread_safe_block_allocator.hpp
 *
 * \brief todo: fill in documentation
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */

#ifndef BIT_MEMORY_THREAD_SAFE_BLOCK_ALLOCATOR_HPP
#define BIT_MEMORY_THREAD_SAFE_BLOCK_ALLOCATOR_HPP

#include "memory.hpp"
#include "memory_block.hpp"

#include <mutex> // std::lock_guard

namespace bit {
  namespace memory {

    //////////////////////////////////////////////////////////////////////////
    /// \brief
    ///
    ///
    ///
    /// \tparam BlockAllocator the block allocator
    /// \tparam Lockable the underlying lockable type
    ///
    /// \satisfies BlockAllocator
    //////////////////////////////////////////////////////////////////////////
    template<typename BlockAllocator, typename Lockable>
    class thread_safe_block_allocator
    {
      //----------------------------------------------------------------------
      // thread_safe_block_allocator
      //----------------------------------------------------------------------
    public:

      /// \brief Constructs the thread_safe_block_allocator by forwarding the
      ///        arguments to the underlyng BlockAllocator
      ///
      /// \param args the args to forward to the underlying BlockAllocator
      template<typename...Args, decltype(BlockAllocator( std::declval<Args>()... ), void())* = nullptr>
      thread_safe_block_allocator( Args&&...args );

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

      //----------------------------------------------------------------------
      // Private Member Types
      //----------------------------------------------------------------------
    private:

      struct storage_type : BlockAllocator
      {
        template<typename...Args>
        storage_type( Args&&...args )
          : BlockAllocator( std::forward<Args>(args)... )
        {

        };

        Lockable lock;
      };

      //----------------------------------------------------------------------
      // Private Members
      //----------------------------------------------------------------------
    private:

      storage_type m_storage;
    };

  } // namespace memory
} // namespace bit

#include "detail/thread_safe_block_allocator.inl"

#endif /* BIT_MEMORY_THREAD_SAFE_BLOCK_ALLOCATOR_HPP */
