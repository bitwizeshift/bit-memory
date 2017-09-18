/**
 * \file debug_block_allocator.hpp
 *
 * \brief todo: fill in documentation
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */

#ifndef BIT_MEMORY_DEBUG_BLOCK_ALLOCATOR_HPP
#define BIT_MEMORY_DEBUG_BLOCK_ALLOCATOR_HPP

#include "memory.hpp"
#include "memory_block.hpp"
#include "debugging.hpp"

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
      // Public Member Types
      //----------------------------------------------------------------------
    public:

      static constexpr std::size_t block_size = BlockAllocator::block_size;

      //----------------------------------------------------------------------
      // Constructor
      //----------------------------------------------------------------------
    public:

      /// \brief Constructs a debug_block_allocator from another allocator
      ///
      /// \param alloc the underlying allocator to construct out of
      template<typename...Args, decltype(BlockAllocator( std::declval<Args>()... ), void())* = nullptr>
      explicit debug_block_allocator( Args&&...args );

      /// \brief Copy-constructs a debug_block_allocator from another one
      ///
      /// \param other the other allocator to copy
      debug_block_allocator( const debug_block_allocator& other ) = default;

      /// \brief Move-constructs a debug_block_allocator from another one
      ///
      /// \param other the other allocator to move
      debug_block_allocator( debug_block_allocator&& other ) noexcept = default;

      //----------------------------------------------------------------------

      /// \brief Destructs the block allocator
      ~debug_block_allocator();

      //----------------------------------------------------------------------

      /// \brief Copy-assigns a debug_block_allocator from another one
      ///
      /// \param other the other allocator to copy
      /// \return a reference to \c (*this)
      debug_block_allocator& operator=( const debug_block_allocator& other ) = default;

      /// \brief Move-assigns a debug_block_allocator from another one
      ///
      /// \param other the other allocator to move
      /// \return a reference to \c (*this)
      debug_block_allocator& operator=( debug_block_allocator&& other ) noexcept = default;

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

#endif /* BIT_MEMORY_DEBUG_BLOCK_ALLOCATOR_HPP */
