/**
 * \file block_allocator_traits.hpp
 *
 * \brief This header defines traits for block_allocators
 *
 * Currently, this library does not contain any optional features
 * that a block_allocator may define; but is defined and used for
 * ensuring forward-compatibility as things change
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */
#ifndef BIT_MEMORY_BLOCK_ALLOCATORS_BLOCK_ALLOCATOR_TRAITS_HPP
#define BIT_MEMORY_BLOCK_ALLOCATORS_BLOCK_ALLOCATOR_TRAITS_HPP

#include "concepts/Stateless.hpp"      // is_stateless
#include "concepts/BlockAllocator.hpp" // is_block_allocator

#include "allocator_reference.hpp" // allocator_reference
#include "allocator_info.hpp"      // allocator_info
#include "macros.hpp"              // BIT_MEMORY_UNUSED
#include "memory_block.hpp"        // memory_block
#include "owner.hpp"               // owner

#include <type_traits> // std::true_type, std::false_type, etc
#include <cstddef>     // std::size_t, std::ptrdiff_t
#include <memory>      // std::addressof

namespace bit {
  namespace memory {

    //////////////////////////////////////////////////////////////////////////
    /// \brief The block_allocator_traits class template provides a
    ///        standardized way to access block allocator functionality
    ///
    /// This will infer defaults for any optional values that may not be
    /// present in the allocator
    ///
    /// \note In the present version of this library, BlockAllocator does not
    ///       define any optional variations. This is here for forward-
    ///       compatibility.
    ///
    /// \tparam BlockAllocator the block allocator type. Must satisfy
    ///         BlockAllocator concept, or be an allocator_reference of a
    ///         BlockAllocator
    //////////////////////////////////////////////////////////////////////////
    template<typename BlockAllocator>
    class block_allocator_traits
    {
      static_assert( is_block_allocator<BlockAllocator>::value, "BlockAllocator must be a BlockAllocator" );

      //----------------------------------------------------------------------
      // Public Member Types
      //----------------------------------------------------------------------
    public:

      using has_block_alignment = block_allocator_has_block_alignment<BlockAllocator>;
      using has_block_size      = block_allocator_has_block_size<BlockAllocator>;
      using is_stateless        = is_stateless<BlockAllocator>;

      //----------------------------------------------------------------------
      // Block Allocations
      //----------------------------------------------------------------------
    public:

      /// \brief Allocates a block from the underlying allocator
      ///
      /// \param alloc the allocator to allocate from
      /// \return the memory block
      static owner<memory_block> allocate_block( BlockAllocator& alloc );

      /// \brief Deallocates a block from the underlying container
      ///
      /// \param alloc the allocator to deallocate from
      /// \param block the block to deallocate
      static void deallocate_block( BlockAllocator& alloc,
                                    owner<memory_block> block );

      //----------------------------------------------------------------------
      // Observers
      //----------------------------------------------------------------------
    public:

      /// \brief Gets the name of the specified block allocator
      ///
      /// \note Not all allocators are nameable or have a name specified.
      ///       For these allocators, the string returned is "Unnamed"
      ///
      /// \note The lifetime of the pointer returned is unmanaged, and is NOT
      ///       the responsibility of the caller to free.
      ///
      /// \param alloc the block allocator to get the name of
      /// \return the name of the allocator
      static allocator_info info( const BlockAllocator& alloc ) noexcept;

      //----------------------------------------------------------------------
      // Capacity
      //----------------------------------------------------------------------
    public:

      /// \brief Returns the block allignment for each block from the given
      ///        block allocator
      ///
      /// \note This is only well-formed for block allocators where
      ///       \ref has_block_alignment is \c std::true_type
      ///
      /// \param alloc the block allocator
      /// \return the block alignment
      static constexpr std::size_t block_alignment( BlockAllocator& alloc ) noexcept;

      /// \brief Returns the block size for each block from a given block
      ///        allocator
      ///
      /// \note This is only well-formed for block allocators where
      ///       \ref has_block_alignment is \c std::true_type
      ///
      /// \param alloc the block allocator
      /// \return the block alignment
      static constexpr std::size_t block_size( BlockAllocator& alloc ) noexcept;


      //----------------------------------------------------------------------
      // Private Implementation
      //----------------------------------------------------------------------
    private:

      /// \{
      /// \brief Determines the info for the allocator, either by calling
      ///        \c BlockAllocator::info or by assuming "Unnamed" for the name
      ///
      /// \param alloc the allocator
      /// \return the name of the allocator
      static allocator_info do_info( std::true_type, const BlockAllocator& alloc );
      static allocator_info do_info( std::false_type, const BlockAllocator& alloc );
      /// \}
    };

    template<typename BlockAllocator>
    class block_allocator_traits<allocator_reference<BlockAllocator>>
      : public block_allocator_traits<BlockAllocator>
    {
    };

  } // namespace memory
} // namespace bit

#include "detail/block_allocator_traits.inl"

#endif /* BIT_MEMORY_BLOCK_ALLOCATORS_BLOCK_ALLOCATOR_TRAITS_HPP */
