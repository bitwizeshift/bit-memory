/**
 * \file static_block_allocator.hpp
 *
 * \brief This header contains the definition for an allocator that can only
 *        allocate a single static block
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */
#ifndef BIT_MEMORY_BLOCK_ALLOCATORS_STATIC_BLOCK_ALLOCATOR_HPP
#define BIT_MEMORY_BLOCK_ALLOCATORS_STATIC_BLOCK_ALLOCATOR_HPP

#include "../memory.hpp"             // owner
#include "../memory_block.hpp"       // memory_block
#include "../memory_block_cache.hpp" // memory_block_cache
#include "debug_block_allocator.hpp"

#include <cstddef> // std::size_t, std::max_align_t
#include <cassert> // assert

namespace bit {
  namespace memory {

    //////////////////////////////////////////////////////////////////////////
    /// \brief This is a block allocator that dolls out a single block of
    ///        static memory
    ///
    /// Repeated allocations will result in \c nullblock entries.
    ///
    /// This type is templated on both the size of the static block, and an
    /// additional Tag type that may be used to create individual instantiations
    ///
    /// \note Since this type creates static storage data, two instantiations
    ///       with the same size/tag will point to the same data -- hence the
    ///       ability to use type-tagging to produce unique instances.
    ///
    /// \tparam BlockSize the size of the static allocation
    /// \tparam Blocks the number of blocks
    /// \tparam Align the alignment of the blocks
    /// \tparam Tag the type used for a tag
    ///
    /// \satisfies BlockAllocator
    //////////////////////////////////////////////////////////////////////////
    template<std::size_t BlockSize,
             std::size_t Blocks = 1,
             std::size_t Align = alignof(std::max_align_t),
             typename Tag = void>
    class static_block_allocator
    {
      static_assert( Blocks > 0,"Must have at least one block" );
      static_assert( is_power_of_two(Align), "Alignment must be a power of two" );
      static_assert( BlockSize % Align == 0, "Block size must must be an increment of the block size" );

      //----------------------------------------------------------------------
      // Public Members
      //----------------------------------------------------------------------
    public:

      static constexpr std::size_t block_size = BlockSize;

      //----------------------------------------------------------------------
      // Constructor
      //----------------------------------------------------------------------
    public:

      /// \brief Default constructs a static_block_allocator
      static_block_allocator() noexcept = default;

      /// \brief Move-constructs a static_block_allocator from another one
      ///
      /// \param other the other allocator to move
      static_block_allocator( static_block_allocator&& other ) noexcept = default;

      // Deleted copy constructor
      static_block_allocator( const static_block_allocator& other ) = delete;


      //----------------------------------------------------------------------

      // Deleted move assignment
      static_block_allocator& operator=( static_block_allocator&& other ) = delete;

      // Deleted copy assignment
      static_block_allocator& operator=( const static_block_allocator& other ) = delete;

      //----------------------------------------------------------------------
      // Block Allocations
      //----------------------------------------------------------------------
    public:

      /// \brief Allocates a single memory block of size \c Size
      ///
      /// This function can only be called once, since it allocates from the
      /// static storage
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

      alignas(Align) static byte s_storage[BlockSize * Blocks];

      /// \brief Gets the static memory_block_cache for this allocator
      ///
      /// \return the memory_block_cache
      static memory_block_cache& block_cache() noexcept;
    };

    template<std::size_t BlockSize, std::size_t Blocks = 1, std::size_t Align = alignof(std::max_align_t), typename Tag = void>
    using debug_static_block_allocator = debug_block_allocator<static_block_allocator<BlockSize,Blocks,Align,Tag>>;

  } // namespace memory
} // namespace bit

#include "detail/static_block_allocator.inl"

#endif /* BIT_MEMORY_BLOCK_ALLOCATORS_STATIC_BLOCK_ALLOCATOR_HPP */
