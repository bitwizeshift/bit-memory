/**
 * \file debug_block_allocator.hpp
 *
 * \brief todo: fill in documentation
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */

#ifndef BIT_MEMORY_BLOCK_ALLOCATORS_DEBUG_BLOCK_ALLOCATOR_HPP
#define BIT_MEMORY_BLOCK_ALLOCATORS_DEBUG_BLOCK_ALLOCATOR_HPP

#include "../memory.hpp"
#include "../memory_block.hpp"
#include "../debugging.hpp"
#include "../errors.hpp"   // get_xyz_handler

#include <algorithm> // std::fill, std::for_each

namespace bit {
  namespace memory {

    //////////////////////////////////////////////////////////////////////////
    /// \brief A block allocator adapter that tags allocated and freed
    ///        memory blocks.
    ///
    /// All allocated blocks are tagged with the pattern
    /// \c debug_tag::allocated_block_byte, and deallocated blocks are tagged
    /// with the pattern \c debug_tag::freed_block_byte
    ///
    /// \satisfies BlockAllocator
    //////////////////////////////////////////////////////////////////////////
    template<typename BlockAllocator>
    class debug_block_allocator
    {
      //----------------------------------------------------------------------
      // Constructor
      //----------------------------------------------------------------------
    public:

      /// \brief Constructs a debug_block_allocator from another allocator
      ///
      /// \param alloc the underlying allocator to construct out of
      template<typename...Args, decltype(BlockAllocator( std::declval<Args>()... ), void())* = nullptr>
      explicit debug_block_allocator( Args&&...args );

      /// \brief Move-constructs a debug_block_allocator from another one
      ///
      /// \param other the other allocator to move
      debug_block_allocator( debug_block_allocator&& other ) noexcept = default;

      // Deleted copy constructor
      debug_block_allocator( const debug_block_allocator& other ) = delete;

      //----------------------------------------------------------------------

      /// \brief Destructs the block allocator
      ~debug_block_allocator();

      //----------------------------------------------------------------------

      // Deleted move assignment
      debug_block_allocator& operator=( debug_block_allocator&& other ) = delete;

      // Deleted copy assignment
      debug_block_allocator& operator=( const debug_block_allocator& other ) = delete;

      //----------------------------------------------------------------------
      // Block Allocations
      //----------------------------------------------------------------------
    public:

      /// \brief Allocates a block from the underlying allocator, tagging the
      ///        memory block with a specific pattern
      ///
      /// \return the memory block
      owner<memory_block> allocate_block();

      /// \brief Deallocates a block from the underlying container, tagging
      ///        the memroy block with a specified pattern first
      ///
      /// This function stores the memory block in the cache for later use
      ///
      /// \param block the block to deallocate
      void deallocate_block( owner<memory_block> block );

      //----------------------------------------------------------------------
      // Observers
      //----------------------------------------------------------------------
    public:

      /// \brief Returns the current size in bytes of allocated blocks
      ///
      /// \return the number of bytes allocated
      std::ptrdiff_t size() const noexcept;

      /// \brief Returns number of allocated blocks
      ///
      /// \return the number of allocated blocks
      std::ptrdiff_t allocations() const noexcept;

      //----------------------------------------------------------------------
      // Private Member Types
      //----------------------------------------------------------------------
    private:

      struct storage_type : BlockAllocator
      {
        template<typename...Args>
        storage_type(Args&&...args)
          : BlockAllocator( std::forward<Args>(args)... ),
            size(0),
            allocations(0)
        {

        }
        std::ptrdiff_t size;        ///< The total size during debugging
        std::ptrdiff_t allocations; ///< The number of allocations
      };

      //----------------------------------------------------------------------
      // Private Members
      //----------------------------------------------------------------------
    private:

      storage_type m_storage;
    };

  } // namespace memory
} // namespace bit

#include "detail/debug_block_allocator.inl"

#endif /* BIT_MEMORY_BLOCK_ALLOCATORS_DEBUG_BLOCK_ALLOCATOR_HPP */
