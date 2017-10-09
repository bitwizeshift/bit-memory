/**
 * \file virtual_block_allocator.hpp
 *
 * \brief This header contains an implementation of a virtual-block allocator.
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */

#ifndef BIT_MEMORY_BLOCK_ALLOCATORS_VIRTUAL_BLOCK_ALLOCATOR_HPP
#define BIT_MEMORY_BLOCK_ALLOCATORS_VIRTUAL_BLOCK_ALLOCATOR_HPP

#include "detail/named_block_allocator.hpp" // detail::named_block_allocator

#include "../owner.hpp"              // owner
#include "../memory_block_cache.hpp" // memory_block_cache
#include "../virtual_memory.hpp"     // virtual_alloc, etc

#include <cstddef> // std::size_t

namespace bit {
  namespace memory {

    //////////////////////////////////////////////////////////////////////////
    /// \brief This block allocator is used for creating virtual memory blocks
    ///
    /// This allocator reserves virtual memory pages up front, and commits
    /// them as they get requested. Any blocks that get deleted are simply
    /// cached for later use, rather than being decommitted each time.
    ///
    /// \satisfies{BlockAllocator}
    //////////////////////////////////////////////////////////////////////////
    class virtual_block_allocator
    {
      //----------------------------------------------------------------------
      // Constructors
      //----------------------------------------------------------------------
    public:

      /// \brief Constructs a virtual_block_allocator that reserves the
      ///        specified number of \p pages up front
      ///
      /// \param pages the number of pages to reserve
      explicit virtual_block_allocator( std::size_t pages );

      /// \brief Move-constructs a virtual_block_allocator from another one
      ///
      /// \param other the other virtual_block_allocator to move
      virtual_block_allocator( virtual_block_allocator&& other ) noexcept;

      // Deleted copy constructor
      virtual_block_allocator( const virtual_block_allocator& other ) = delete;

      //----------------------------------------------------------------------

      /// \brief Destructs this virtual_block_allocator, decommitting any
      ///        commit pages
      ~virtual_block_allocator();

      //----------------------------------------------------------------------

      // Deleted move-assignment
      virtual_block_allocator& operator=( virtual_block_allocator&& other ) = delete;

      // Deleted copy-assignment
      virtual_block_allocator& operator=( const virtual_block_allocator& other ) = delete;

      //----------------------------------------------------------------------
      // Block Allocations
      //----------------------------------------------------------------------
    public:

      /// \brief Allocates a memory_block of size block_size
      ///
      /// \return a newly allocated memory_block
      owner<memory_block> allocate_block() noexcept;

      /// \brief Deallocates a given memory_block
      ///
      /// \param block the block to deallocate
      void deallocate_block( owner<memory_block> block ) noexcept;

      //----------------------------------------------------------------------
      // Private Members
      //----------------------------------------------------------------------
    private:

      void*              m_memory;      ///< The virtual memory to access from
      const std::size_t  m_pages;       ///< The total number of pages
      std::ptrdiff_t     m_active_page; ///< The currently active page
      memory_block_cache m_cache;       ///< Cache of already committed pages
    };

    //-------------------------------------------------------------------------
    // Utilities
    //-------------------------------------------------------------------------

    using named_virtual_block_allocator = detail::named_block_allocator<virtual_block_allocator>;

  } // namespace memory
} // namespace bit

#endif /* BIT_MEMORY_BLOCK_ALLOCATORS_VIRTUAL_BLOCK_ALLOCATOR_HPP */
