/**
 * \file new_block_allocator.hpp
 *
 * \brief This header contains an API for allocating memory blocks that use
 *        ::operator new as the underlying allocation scheme.
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */

#ifndef BIT_MEMORY_BLOCK_ALLOCATORS_NEW_BLOCK_ALLOCATOR_HPP
#define BIT_MEMORY_BLOCK_ALLOCATORS_NEW_BLOCK_ALLOCATOR_HPP

#include "../memory.hpp"       // owner
#include "../memory_block.hpp" // memory_block

#include "cached_block_allocator.hpp"
#include "debug_block_allocator.hpp"

namespace bit {
  namespace memory {

    //////////////////////////////////////////////////////////////////////////
    /// \brief A block allocator that uses ::operator new(...) to allocate
    ///        memory blocks
    ///
    /// \satisfies BlockAllocator
    //////////////////////////////////////////////////////////////////////////
    class new_block_allocator
    {
      //----------------------------------------------------------------------
      // Public Member Types
      //----------------------------------------------------------------------
    public:

      using block_alignment = std::integral_constant<std::size_t,alignof(std::max_align_t)>;

      //----------------------------------------------------------------------
      // Constructors
      //----------------------------------------------------------------------
    public:

      /// \brief Constructs a new_block_allocator that will distribute
      ///        blocks of the specified \p size
      ///
      /// \param size the size of each block allocation
      explicit new_block_allocator( std::size_t size ) noexcept;

      /// \brief Move-constructs a new_block_allocator from another allocator
      ///
      /// \param other the other new_block_allocator to move
      new_block_allocator( new_block_allocator&& other ) noexcept = default;

      // Deleted copy constructor
      new_block_allocator( const new_block_allocator& other ) = delete;

      //-----------------------------------------------------------------------

      /// \brief Move-assigns a new_block_allocator from another allocator
      ///
      /// \param other the other allocator to move_assign
      /// \return reference to \c (*this)
      new_block_allocator& operator=( new_block_allocator&& other ) noexcept = default;

      // Deleted copy assignment
      new_block_allocator& operator=( const new_block_allocator& other ) = delete;

      //----------------------------------------------------------------------
      // Block Allocations
      //----------------------------------------------------------------------
    public:

      /// \brief Allocates a new'ed memory_block
      ///
      /// \return a null memory_block
      owner<memory_block> allocate_block() noexcept;

      /// \brief Deallocates a new'ed memory_block
      ///
      /// \param block the block to deallocate
      void deallocate_block( owner<memory_block> block ) noexcept;

      //----------------------------------------------------------------------
      // Private Members
      //----------------------------------------------------------------------
    private:

      std::size_t m_size;

      friend bool operator==( const new_block_allocator&, const new_block_allocator& ) noexcept;
    };

    //-------------------------------------------------------------------------
    // Comparisons
    //-------------------------------------------------------------------------

    bool operator==( const new_block_allocator& lhs,
                     const new_block_allocator& rhs ) noexcept;
    bool operator!=( const new_block_allocator& lhs,
                     const new_block_allocator& rhs ) noexcept;

    //-------------------------------------------------------------------------
    // Utilities
    //-------------------------------------------------------------------------

    using cached_new_block_allocator = cached_block_allocator<new_block_allocator>;
    using debug_new_block_allocator  = debug_block_allocator<new_block_allocator>;
    using cached_debug_new_block_allocator = debug_block_allocator<cached_block_allocator<new_block_allocator>>;

  } // namespace memory
} // namespace bit

#include "detail/new_block_allocator.inl"

#endif /* BIT_MEMORY_BLOCK_ALLOCATORS_NEW_BLOCK_ALLOCATOR_HPP */
