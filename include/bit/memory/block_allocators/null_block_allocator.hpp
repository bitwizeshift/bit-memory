/**
 * \file null_block_allocator.hpp
 *
 * \brief todo: fill in documentation
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */

#ifndef BIT_MEMORY_BLOCK_ALLOCATORS_NULL_BLOCK_ALLOCATOR_HPP
#define BIT_MEMORY_BLOCK_ALLOCATORS_NULL_BLOCK_ALLOCATOR_HPP

#include "detail/named_block_allocator.hpp" // detail::named_block_allocator

#include "../owner.hpp"         // owner
#include "../memory_block.hpp"  // memory_block

#include <type_traits> // std::integral_constant, std::true_false
#include <cstddef>     // std::max_align_t

namespace bit {
  namespace memory {

    //////////////////////////////////////////////////////////////////////////
    /// \brief A block allocator that only distributes null blocks
    ///
    /// \satisfies{BlockAllocator}
    /// \satisfies{Stateless}
    //////////////////////////////////////////////////////////////////////////
    class null_block_allocator
    {
      //----------------------------------------------------------------------
      // Public Member Types
      //----------------------------------------------------------------------
    public:

      using block_size      = std::integral_constant<std::size_t,0>;
      using block_alignment = std::integral_constant<std::size_t,1>;
      using is_stateless    = std::true_type;

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

      /// \brief Copy-constructs a null_block_allocator from another allocator
      ///
      /// \param other the other null_block_allocator to copy
      null_block_allocator( const null_block_allocator& other ) noexcept = default;

      //-----------------------------------------------------------------------

      /// \brief Move-assigns a null_block_allocator from another allocator
      ///
      /// \param other the other null_block_allocator to move
      /// \return reference to \c (*this)
      null_block_allocator& operator=( null_block_allocator&& other ) noexcept = default;

      /// \brief Copy-assigns a null_block_allocator from another allocator
      ///
      /// \param other the other null_block_allocator to copy
      /// \return reference to \c (*this)
      null_block_allocator& operator=( const null_block_allocator& other ) noexcept = default;

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

    using named_null_block_allocator = detail::named_block_allocator<null_block_allocator>;

  } // namespace memory
} // namespace bit

#include "detail/null_block_allocator.inl"

#endif /* BIT_MEMORY_BLOCK_ALLOCATORS_NULL_BLOCK_ALLOCATOR_HPP */
