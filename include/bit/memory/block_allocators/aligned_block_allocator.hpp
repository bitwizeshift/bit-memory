/**
 * \file aligned_block_allocator.hpp
 *
 * \brief This header contains the implementation of a block allocator that
 *        allocates aligned memory
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */

#ifndef BIT_MEMORY_BLOCK_ALLOCATORS_ALIGNED_BLOCK_ALLOCATOR_HPP
#define BIT_MEMORY_BLOCK_ALLOCATORS_ALIGNED_BLOCK_ALLOCATOR_HPP

#include "../memory.hpp"             // is_power_of_two
#include "../aligned_memory.hpp"     // aligned_allocate
#include "../memory_block.hpp"       // memory_block

#include "cached_block_allocator.hpp"
#include "debug_block_allocator.hpp"

namespace bit {
  namespace memory {

    //////////////////////////////////////////////////////////////////////////
    /// \brief A raw allocator that allocates over-aligned memory
    ///
    /// \tparam Size The size of the block
    /// \tparam Align The alignment of block
    ///
    /// \satisfies BlockAllocator
    /// \satisfies Stateless
    //////////////////////////////////////////////////////////////////////////
    template<std::size_t Size,std::size_t Align>
    class aligned_block_allocator
    {
      static_assert( is_power_of_two(Align), "Alignment must be a power of two!" );

      //----------------------------------------------------------------------
      // Public Member Types
      //----------------------------------------------------------------------
    public:

      using block_alignment = std::integral_constant<std::size_t,Align>;

      //----------------------------------------------------------------------
      // Constructors / Allocators
      //----------------------------------------------------------------------
    public:

      /// \brief Constructs a aligned_block_allocator that will distribute
      ///        blocks of the specified \p size
      ///
      /// \param size the size of each block allocation
      aligned_block_allocator() noexcept = default;

      /// \brief Move-constructs an aligned_block_allocator from another allocator
      ///
      /// \param other the other aligned_block_allocator to move
      aligned_block_allocator( aligned_block_allocator&& other ) noexcept = default;

      /// \brief Copy-constructs an aligned_block_allocator from another allocator
      ///
      /// \param other the other aligned_block_allocator to copy
      aligned_block_allocator( const aligned_block_allocator& other ) noexcept = default;

      //-----------------------------------------------------------------------

      /// \brief Move-assigns an aligned_block_allocator from another allocator
      ///
      /// \param other the other aligned_block_allocator to move
      /// \return reference to \c (*this)
      aligned_block_allocator& operator=( aligned_block_allocator&& other ) noexcept = default;

      /// \brief Copy-assigns an aligned_block_allocator from another allocator
      ///
      /// \param other the other aligned_block_allocator to copy
      /// \return reference to \c (*this)
      aligned_block_allocator& operator=( const aligned_block_allocator& other ) noexcept = default;

      //----------------------------------------------------------------------
      // Block Allocations
      //----------------------------------------------------------------------
    public:

      /// \brief Allocates a memory_block of size \ref block_size with
      ///        alignment \p Align
      ///
      /// \return a newly allocated memory_block
      owner<memory_block> allocate_block() noexcept;

      /// \brief Deallocates a given memory_block
      ///
      /// \param block the block to deallocate
      void deallocate_block( owner<memory_block> block ) noexcept;

    };

    //-------------------------------------------------------------------------
    // Utilities
    //-------------------------------------------------------------------------

    /// \brief A cached version of aligned_block_allocator
    ///
    /// \tparam Size The size of the block
    /// \tparam Align The alignment of block
    template<std::size_t Size,std::size_t Align>
    using cached_aligned_block_allocator = cached_block_allocator<aligned_block_allocator<Size,Align>>;

    /// \brief A debug version of aligned_block_allocator
    ///
    /// \tparam Size The size of the block
    /// \tparam Align The alignment of block
    template<std::size_t Size,std::size_t Align>
    using debug_aligned_block_allocator  = debug_block_allocator<aligned_block_allocator<Size,Align>>;

    /// \brief A cached/debug version of aligned_block_allocator
    ///
    /// \tparam Size The size of the block
    /// \tparam Align The alignment of block
    template<std::size_t Size,std::size_t Align>
    using cached_debug_aligned_block_allocator = cached_block_allocator<debug_block_allocator<aligned_block_allocator<Size,Align>>>;

  } // namespace memory
} // namespace bit

#include "detail/aligned_block_allocator.inl"

#endif /* BIT_MEMORY_BLOCK_ALLOCATORS_ALIGNED_BLOCK_ALLOCATOR_HPP */
