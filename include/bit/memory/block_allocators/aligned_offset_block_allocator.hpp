/**
 * \file aligned_offset_block_allocator.hpp
 *
 * \brief This header contains the implementation of a block allocator that
 *        allocates aligned memory with a given offset
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */
#ifndef BIT_MEMORY_BLOCK_ALLOCATORS_ALIGNED_OFFSET_BLOCK_ALLOCATOR_HPP
#define BIT_MEMORY_BLOCK_ALLOCATORS_ALIGNED_OFFSET_BLOCK_ALLOCATOR_HPP

#include "../aligned_memory.hpp"
#include "../memory_block.hpp"
#include "cached_block_allocator.hpp"
#include "debug_block_allocator.hpp"

namespace bit {
  namespace memory {

    //////////////////////////////////////////////////////////////////////////
    /// \brief A raw allocator that allocates over-aligned memory with a
    ///        given \p Offset
    ///
    /// \tparam Size The size of the block
    /// \tparam Align The alignment of block
    /// \tparam Offset The offset from the alignment
    ///
    /// \satisfies BlockAllocator
    //////////////////////////////////////////////////////////////////////////
    template<std::size_t Size,std::size_t Align, std::size_t Offset>
    class aligned_offset_block_allocator
    {
      //----------------------------------------------------------------------
      // Public Members
      //----------------------------------------------------------------------
    public:

      static constexpr std::size_t block_size  = Size;
      static constexpr std::size_t align_size  = Align;
      static constexpr std::size_t offset_size = Offset;

      //----------------------------------------------------------------------
      // Block Allocations
      //----------------------------------------------------------------------
    public:

      /// \brief Allocates a memory_block of size \ref block_size with
      ///        alignment \p Align with an offset of \ref Offset
      ///
      /// \return a newly allocated memory_block
      owner<memory_block> allocate_block() noexcept;

      /// \brief Deallocates a given memory_block
      ///
      /// \param block the block to deallocate
      void deallocate_block( owner<memory_block> block ) noexcept;

    };

    /// \brief A cached version of aligned_offset_block_allocator
    ///
    /// \tparam Size The size of the block
    /// \tparam Align The alignment of block
    /// \tparam Offset The offset from the alignment
    template<std::size_t Size,std::size_t Align,std::size_t Offset>
    using cached_aligned_offset_block_allocator = cached_block_allocator<aligned_offset_block_allocator<Size,Align,Offset>>;

    /// \brief A debug version of aligned_offset_block_allocator
    ///
    /// \tparam Size The size of the block
    /// \tparam Align The alignment of block
    /// \tparam Offset The offset from the alignment
    template<std::size_t Size,std::size_t Align,std::size_t Offset>
    using debug_aligned_offset_block_allocator  = debug_block_allocator<aligned_offset_block_allocator<Size,Align,Offset>>;

    /// \brief A cached/debug version of aligned_offset_block_allocator
    ///
    /// \tparam Size The size of the block
    /// \tparam Align The alignment of block
    /// \tparam Offset The offset from the alignment
    template<std::size_t Size,std::size_t Align,std::size_t Offset>
    using cached_debug_aligned_offset_block_allocator = cached_block_allocator<debug_block_allocator<aligned_offset_block_allocator<Size,Align,Offset>>>;

  } // namespace memory
} // namespace bit

#include "detail/aligned_offset_block_allocator.inl"

#endif /* BIT_MEMORY_BLOCK_ALLOCATORS_ALIGNED_OFFSET_BLOCK_ALLOCATOR_HPP */
