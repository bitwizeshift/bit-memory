/**
 * \file growing_virtual_block_allocator.hpp
 *
 * \brief This header contains the implementation of a growing block allocator
 *        that allocates virtual memory
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */
#ifndef BIT_MEMORY_BLOCK_ALLOCATORS_GROWING_VIRTUAL_BLOCK_ALLOCATOR_HPP
#define BIT_MEMORY_BLOCK_ALLOCATORS_GROWING_VIRTUAL_BLOCK_ALLOCATOR_HPP

#include "detail/named_block_allocator.hpp" // detail::named_block_allocator

#include "../utilities/allocator_info.hpp"     // allocator_info
#include "../utilities/memory_block.hpp"       // memory_block
#include "../utilities/memory_block_cache.hpp" // memory_block_cache
#include "../utilities/owner.hpp"              // owner

#include <cstddef> // std::size_t

namespace bit {
  namespace memory {

    //////////////////////////////////////////////////////////////////////////
    /// \brief This block allocator is used for creating virtual memory blocks
    ///        that double in size with each subsequent block
    ///
    /// This allocator reserves virtual memory pages up front, and commits
    /// them as they get requested. Any blocks that get deleted are simply
    /// cached for later use, rather than being decommitted each time.
    ///
    /// \satisfies{BlockAllocator}
    //////////////////////////////////////////////////////////////////////////
    class growing_virtual_block_allocator
    {
      //-----------------------------------------------------------------------
      // Constructors
      //-----------------------------------------------------------------------
    public:

      /// \brief Constructs a growing_virtual_block_allocator that reserves the
      ///        specified number of \p pages up front
      ///
      /// \param pages the number of pages to reserve
      explicit growing_virtual_block_allocator( std::size_t pages );

      /// \brief Move-constructs a growing_virtual_block_allocator from another one
      ///
      /// \param other the other growing_virtual_block_allocator to move
      growing_virtual_block_allocator( growing_virtual_block_allocator&& other ) noexcept;

      // Deleted copy constructor
      growing_virtual_block_allocator( const growing_virtual_block_allocator& other ) = delete;

      //-----------------------------------------------------------------------

      /// \brief Destructs this growing_virtual_block_allocator, decommitting
      ///        any commit pages
      ~growing_virtual_block_allocator();

      //-----------------------------------------------------------------------

      // Deleted move-assignment
      growing_virtual_block_allocator& operator=( growing_virtual_block_allocator&& other ) = delete;

      // Deleted copy-assignment
      growing_virtual_block_allocator& operator=( const growing_virtual_block_allocator& other ) = delete;

      //-----------------------------------------------------------------------
      // Block Allocations
      //-----------------------------------------------------------------------
    public:

      /// \brief Allocates a memory_block of size block_size
      ///
      /// \return a newly allocated memory_block
      owner<memory_block> allocate_block() noexcept;

      /// \brief Deallocates a given memory_block
      ///
      /// \param block the block to deallocate
      void deallocate_block( owner<memory_block> block ) noexcept;

      //-----------------------------------------------------------------------
      // Observers
      //-----------------------------------------------------------------------
    public:

      /// \brief Queries the next block size expected from this allocator
      ///
      /// \return the size of the next allocated block
      std::size_t next_block_size() const noexcept;

      /// \brief Gets the info about this allocator
      ///
      /// This defaults to 'growing_virtual_block_allocator'.
      /// Use a named_growing_virtual_block_allocator to override this
      ///
      /// \return the info for this allocator
      allocator_info info() const noexcept;

      //-----------------------------------------------------------------------
      // Private Members
      //-----------------------------------------------------------------------
    private:

      void*              m_memory;      ///< The virtual memory to access from
      const std::size_t  m_pages;       ///< The total number of pages
      std::ptrdiff_t     m_active_page; ///< The currently active page
      std::size_t        m_multiplier;  ///< The current multiplier
      memory_block_cache m_cache;       ///< Cache of already committed pages
    };

    //-------------------------------------------------------------------------
    // Utilities
    //-------------------------------------------------------------------------

    using named_growing_virtual_block_allocator = detail::named_block_allocator<growing_virtual_block_allocator>;

  } // namespace memory
} // namespace bit

#endif /* BIT_MEMORY_BLOCK_ALLOCATORS_GROWING_VIRTUAL_BLOCK_ALLOCATOR_HPP */
