/**
 * \file memory_block_cache.hpp
 *
 * \brief This header contains definitions for an intrinsically linked list
 *        of available memory_blocks
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */

#ifndef BIT_MEMORY_MEMORY_BLOCK_CACHE_HPP
#define BIT_MEMORY_MEMORY_BLOCK_CACHE_HPP

#include "memory_block.hpp" // memory_block
#include "memory.hpp"       // owner

namespace bit {
  namespace memory {

    //////////////////////////////////////////////////////////////////////////
    /// \brief A cache containing an intrinsically linked list of
    ///        memory_blocks
    ///
    /// Blocks in this cache are not guaranteed to all be of the same size.
    /// Memory blocks may originate from different allocators, and represent
    /// different regions of memory -- however this is not the recommended
    /// practice.
    /////////////////////////////////////////////////////////////////////////
    class memory_block_cache
    {
      //----------------------------------------------------------------------
      // Constructor / Assignment
      //----------------------------------------------------------------------
    public:

      /// \brief Default constructs a block cache
      memory_block_cache() noexcept;

      /// \brief Move-constructs a memory_block_cache from an existing one
      ///
      /// \param other the other memory_block_cache to move
      memory_block_cache( memory_block_cache&& other ) noexcept = default;

      // Deleted copy constructor
      memory_block_cache( const memory_block_cache& other ) = delete;

      //----------------------------------------------------------------------

      // Deleted move assignment
      memory_block_cache& operator=( memory_block_cache&& other ) = delete;

      // Deleted copy assignment
      memory_block_cache& operator=( const memory_block_cache& other ) = delete;

      //----------------------------------------------------------------------
      // Observers
      //----------------------------------------------------------------------
    public:

      /// \brief Returns whether or not this memory_block_cache is empty
      ///
      /// \return \c true if this memory_block_cache is empty, \c false otherwise
      bool empty() const noexcept;

      /// \brief Returns the number of memory_blocks in this cache
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

      //----------------------------------------------------------------------
      // Caching
      //----------------------------------------------------------------------
    public:

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

#endif /* BIT_MEMORY_MEMORY_BLOCK_CACHE_HPP */
