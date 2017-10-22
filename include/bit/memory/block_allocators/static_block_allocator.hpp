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

#include "detail/named_block_allocator.hpp" // detail::named_block_allocator
#include "../owner.hpp"              // owner
#include "../memory_block.hpp"       // memory_block
#include "../memory_block_cache.hpp" // memory_block_cache

#include <cstddef> // std::size_t, std::max_align_t
#include <cassert> // assert

namespace bit {
  namespace memory {

    //////////////////////////////////////////////////////////////////////////
    /// \brief This is a block allocator that distributes blocks of memory
    ///        stored statically.
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
    /// \satisfies{BlockAllocator}
    /// \satisfies{Stateless}
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

      using block_size      = std::integral_constant<std::size_t,BlockSize>;
      using block_alignment = std::integral_constant<std::size_t,Align>;

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

      /// \brief Copy-constructs a static_block_allocator from another one
      ///
      /// \param other the other allocator to copy
      static_block_allocator( const static_block_allocator& other ) noexcept = default;


      //----------------------------------------------------------------------

      /// \brief Move-assigns a static_block_allocator from another one
      ///
      /// \param other the other allocator to move
      /// \return reference to \c (*this)
      static_block_allocator& operator=( static_block_allocator&& other ) noexcept = default;

      /// \brief Move-assigns a static_block_allocator from another one
      ///
      /// \param other the other allocator to move
      /// \return reference to \c (*this)
      static_block_allocator& operator=( const static_block_allocator& other ) noexcept = default;

      //----------------------------------------------------------------------
      // Block Allocations
      //----------------------------------------------------------------------
    public:

      /// \brief Allocates a memory block of size \c Size
      ///
      /// \return a newly allocated memory_block
      owner<memory_block> allocate_block() noexcept;

      /// \brief Deallocates a given memory_block
      ///
      /// \param block the block to deallocate
      void deallocate_block( owner<memory_block> block ) noexcept;

      //-----------------------------------------------------------------------
      // Observers
      //-----------------------------------------------------------------------
    public:

      /// \brief Queries the next block size expected from this allocator
      ///
      /// \return the size of the next allocated block
      std::size_t next_block_size() const noexcept;

      /// \brief Gets the info about this allocator
      ///
      /// This defaults to 'static_block_allocator'.
      /// Use a named_static_block_allocator to override this
      ///
      /// \return the info for this allocator
      allocator_info info() const noexcept;

      //----------------------------------------------------------------------
      // Private Members
      //----------------------------------------------------------------------
    private:

      alignas(Align) static char s_storage[BlockSize * Blocks];

      /// \brief Gets the static memory_block_cache for this allocator
      ///
      /// \return the memory_block_cache
      static memory_block_cache& block_cache() noexcept;
    };

    //-------------------------------------------------------------------------

    template<std::size_t BlockSize,
             std::size_t Blocks = 1,
             std::size_t Align = alignof(std::max_align_t),
             typename Tag = void>
    using named_static_block_allocator = detail::named_block_allocator<static_block_allocator<BlockSize,Blocks,Align,Tag>>;

  } // namespace memory
} // namespace bit

#include "detail/static_block_allocator.inl"

#endif /* BIT_MEMORY_BLOCK_ALLOCATORS_STATIC_BLOCK_ALLOCATOR_HPP */
