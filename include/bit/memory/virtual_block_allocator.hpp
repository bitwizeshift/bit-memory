/**
 * \file virtual_block_allocator.hpp
 *
 * \brief This header contains an implementation of a virtual-block allocator.
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */

#ifndef BIT_MEMORY_VIRTUAL_BLOCK_ALLOCATOR_HPP
#define BIT_MEMORY_VIRTUAL_BLOCK_ALLOCATOR_HPP

#include "virtual_memory.hpp"
#include "memory_block.hpp"
#include "memory_block_cache.hpp"
#include "debug_block_allocator.hpp"

#include <cstddef> // std::size_t

namespace bit {
  namespace memory {

    //////////////////////////////////////////////////////////////////////////
    /// \brief This block allocator is used for creating virtual memory blocks
    ///
    /// This allocator reserves virtual memory pages up front, and commits
    /// them as they get requested.
    ///
    /// \satisfies BlockAllocator
    //////////////////////////////////////////////////////////////////////////
    class virtual_block_allocator
    {
      //----------------------------------------------------------------------
      // Public Members
      //----------------------------------------------------------------------
    public:

      static const std::size_t block_size;
      static const std::size_t align_size;

      //----------------------------------------------------------------------
      // Constructors
      //----------------------------------------------------------------------
    public:

      /// \brief Constructs a virtual_block_allocator that reserves the
      ///        specified number of \p pages up front
      ///
      /// \param pages the number of pages to reserve
      explicit virtual_block_allocator( std::size_t pages );

      /// \brief Destructs this virtual_block_allocator, decommitting any
      ///        commit pages
      ~virtual_block_allocator();

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

      memory_block_cache m_cache; ///< The head of the current virtual memory page list
    };

    /// \brief A debug virtual block allocator.
    using debug_virtual_block_allocator = debug_block_allocator<virtual_block_allocator>;

  } // namespace memory
} // namespace bit

#endif /* BIT_MEMORY_VIRTUAL_BLOCK_ALLOCATOR_HPP */
