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

#include "memory_block.hpp"
#include "memory.hpp"
#include <iterator>

namespace bit {
  namespace memory {

    //////////////////////////////////////////////////////////////////////////
    /// \brief An iterator for the memory_block_cache
    //////////////////////////////////////////////////////////////////////////
    class memory_block_cache_iterator
    {
      //----------------------------------------------------------------------
      // Public Member Types
      //----------------------------------------------------------------------
    public:

      using iterator_category = std::forward_iterator_tag;
      using value_type        = std::add_const_t<memory_block>;
      using pointer           = value_type*;
      using reference         = value_type&;
      using difference_type   = std::ptrdiff_t;

      //----------------------------------------------------------------------
      // Constructor
      //----------------------------------------------------------------------
    public:

      /// \brief Default-constructs an end-iterator
      explicit memory_block_cache_iterator();

      /// \brief Constructs an iterator for this memory_block_cache
      ///
      /// \param block a pointer to the first block
      explicit memory_block_cache_iterator( memory_block block );

      //----------------------------------------------------------------------
      // Member functions
      //----------------------------------------------------------------------
    public:

      reference operator*() noexcept;
      pointer operator->() noexcept;

      memory_block_cache_iterator& operator++() noexcept;
      memory_block_cache_iterator operator++(int) noexcept;

      //----------------------------------------------------------------------
      // Private Members
      //----------------------------------------------------------------------
    private:

      memory_block m_block; /// A memory block to use as the pointer

      friend bool operator==( const memory_block_cache_iterator&,
                              const memory_block_cache_iterator& ) noexcept;
    };

    //------------------------------------------------------------------------
    // Comparison Operators
    //------------------------------------------------------------------------

    bool operator==( const memory_block_cache_iterator& lhs,
                     const memory_block_cache_iterator& rhs ) noexcept;

    bool operator!=( const memory_block_cache_iterator& lhs,
                     const memory_block_cache_iterator& rhs ) noexcept;

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
      // Public Member Types
      //----------------------------------------------------------------------
    public:

      using iterator        = memory_block_cache_iterator;
      using const_iterator  = memory_block_cache_iterator;

      //----------------------------------------------------------------------
      // Constructor
      //----------------------------------------------------------------------
    public:

      /// \brief Default constructs a block cache
      memory_block_cache() noexcept;

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
      // Iteration
      //----------------------------------------------------------------------
    public:

      /// \brief Gets the begin iterator for this cache
      ///
      /// \return the begin iterator
      iterator begin() noexcept;

      /// \copydoc memory_block_cache::begin()
      const_iterator begin() const noexcept;

      /// \copydoc memory_block_cache::begin()
      const_iterator cbegin() const noexcept;

      //----------------------------------------------------------------------

      /// \brief Gets the end iterator for this cache
      ///
      /// \return the end iterator
      iterator end() noexcept;

      /// \copydoc memory_block_cache::end()
      const_iterator end() const noexcept;

      /// \copydoc memory_block_cache::end()
      const_iterator cend() const noexcept;

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
