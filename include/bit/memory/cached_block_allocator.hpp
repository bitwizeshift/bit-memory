/**
 * \file cached_block_allocator.hpp
 *
 * \brief This header contains an allocator adapter that caches previously
 *        allocated blocks
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */

#ifndef BIT_MEMORY_CACHED_BLOCK_ALLOCATOR_HPP
#define BIT_MEMORY_CACHED_BLOCK_ALLOCATOR_HPP

#include "memory.hpp"             // owner
#include "memory_block.hpp"       // memory_block
#include "memory_block_cache.hpp" // memory_block_cache

#include <utility> // std::forward

namespace bit {
  namespace memory {

    //////////////////////////////////////////////////////////////////////////
    /// \brief This class is an adapter around a given block allocator that
    ///        caches any allocated blocks for reuse.
    ///
    /// Any constructors for the underlying BlockAllocator are propagated
    /// to the cache_block_allocator constructor -- allowing it to be
    /// constructed as though it is the underlying type.
    ///
    /// \tparam BlockAllocator the block allocator to adapt around
    //////////////////////////////////////////////////////////////////////////
    template<typename BlockAllocator>
    class cached_block_allocator
    {
      //----------------------------------------------------------------------
      // Constructor
      //----------------------------------------------------------------------
    public:

      /// \brief Constructs a cached_block_allocator from another allocator
      ///
      /// \param alloc the underlying allocator to construct out of
      template<typename...Args>
      explicit cached_block_allocator( Args&&...args );

      /// Deleted copy constructor
      cached_block_allocator( const cached_block_allocator& other ) = delete;

      /// \brief Move-constructs a cached_block_allocator from another one
      ///
      /// \param other the other allocator to move
      cached_block_allocator( cached_block_allocator&& other ) noexcept = default;

      /// \brief Destructs this cached_block_allocator and destroys all
      ///        cached entries
      ~cached_block_allocator();

      /// Deleted copy assignment operator
      cached_block_allocator& operator=( const cached_block_allocator& other ) = delete;

      /// \brief Move-assigns a cached_block_allocator from another one
      ///
      /// \param other the other allocator to move
      /// \return a reference to \c (*this)
      cached_block_allocator& operator=( cached_block_allocator&& other ) noexcept = default;

      //----------------------------------------------------------------------
      // Block Allocations
      //----------------------------------------------------------------------
    public:

      /// \brief Allocates a block, drawing from a cache if a cached block
      ///        exists
      ///
      /// \return the memory block
      owner<memory_block> allocate_block();

      /// \brief Deallocates a specified memory block
      ///
      /// This function stores the memory block in the cache for later use
      ///
      /// \param block the block to deallocate
      void deallocate_block( owner<memory_block> block );

      //----------------------------------------------------------------------
      // Private Member Types
      //----------------------------------------------------------------------
    private:

      // Inherit from BlockAllocator to leverage EBO
      struct storage_type : BlockAllocator
      {
        template<typename...Args>
        storage_type( Args&&...args )
          : BlockAllocator( std::forward<Args>(args)... )
        {}

        memory_block_cache cache;
      };

      //----------------------------------------------------------------------
      // Private Members
      //----------------------------------------------------------------------
    private:

      storage_type m_storage; ///< The underlying storage type
    };

  } // namespace memory
} // namespace bit

#include "detail/cached_block_allocator.inl"

#endif /* BIT_MEMORY_CACHED_BLOCK_ALLOCATOR_HPP */
