/**
 * \file null_block_allocator.hpp
 *
 * \brief todo: fill in documentation
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */

#ifndef BIT_MEMORY_BLOCK_ALLOCATORS_NULL_BLOCK_ALLOCATOR_HPP
#define BIT_MEMORY_BLOCK_ALLOCATORS_NULL_BLOCK_ALLOCATOR_HPP

#include "../memory.hpp"
#include "../memory_block.hpp"
#include "cached_block_allocator.hpp"

#include <type_traits> // std::integral_constant, std::true_false
#include <cstddef>     // std::max_align_t

namespace bit {
  namespace memory {

    //////////////////////////////////////////////////////////////////////////
    /// \brief A block allocator that only distributes null blocks
    ///
    /// \satisfies BlockAllocator
    //////////////////////////////////////////////////////////////////////////
    class null_block_allocator
    {
      //----------------------------------------------------------------------
      // Public Member Types
      //----------------------------------------------------------------------
    public:

      using is_always_equal = std::true_type;
      using is_stateless    = std::true_type;
      using block_alignment = std::integral_constant<std::size_t,1>;

      //-----------------------------------------------------------------------
      // Constructor / Assignment
      //-----------------------------------------------------------------------
    public:

      /// \brief Default-constructs a null_block_allocator
      null_block_allocator() = default;

      /// \brief Move-constructs a null_block_allocator from another allocator
      ///
      /// \param other the other null_block_allocator to move
      null_block_allocator( null_block_allocator&& other ) noexcept = default;

      // Deleted copy constructor
      null_block_allocator( const null_block_allocator& other ) = delete;

      //-----------------------------------------------------------------------

      // Deleted move assignment
      null_block_allocator& operator=( null_block_allocator&& other ) = delete;

      // Deleted copy assignment
      null_block_allocator& operator=( const null_block_allocator& other ) = delete;

      //----------------------------------------------------------------------
      // Block Allocations
      //----------------------------------------------------------------------
    public:

      /// \brief Allocates a null memory_block
      ///
      /// \return a null memory_block
      owner<memory_block> allocate_block() noexcept;

      /// \brief Deallocates a memory_block
      ///
      /// \param block the block to deallocate
      void deallocate_block( owner<memory_block> block ) noexcept;
    };

    //-------------------------------------------------------------------------
    // Comparisons
    //-------------------------------------------------------------------------

    bool operator==( const null_block_allocator& lhs,
                     const null_block_allocator& rhs ) noexcept;
    bool operator!=( const null_block_allocator& lhs,
                     const null_block_allocator& rhs ) noexcept;

    //-------------------------------------------------------------------------
    // Utilities
    //-------------------------------------------------------------------------

    using cached_null_block_allocator = cached_block_allocator<null_block_allocator>;

  } // namespace memory
} // namespace bit

#include "detail/null_block_allocator.inl"

#endif /* BIT_MEMORY_BLOCK_ALLOCATORS_NULL_BLOCK_ALLOCATOR_HPP */
