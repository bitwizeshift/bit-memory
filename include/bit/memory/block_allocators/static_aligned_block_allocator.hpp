/**
 * \file static_aligned_block_allocator.hpp
 *
 * \brief TODO: Add description
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */
#ifndef BIT_MEMORY_BLOCK_ALLOCATORS_STATIC_ALIGNED_BLOCK_ALLOCATOR_HPP
#define BIT_MEMORY_BLOCK_ALLOCATORS_STATIC_ALIGNED_BLOCK_ALLOCATOR_HPP

#include "../memory.hpp"
#include "../memory_block.hpp"
#include "debug_block_allocator.hpp"

#include <cstdlib> // std::size_t
#include <cassert> // assert
#include <type_traits> // std::aligned_storage_t

namespace bit {
  namespace memory {

    //////////////////////////////////////////////////////////////////////////
    /// \brief This is a block allocator that dolls out a single block of
    ///        static memory
    ///
    /// Repeated allocations will result in \c nullblock entries.
    ///
    /// This type is templated on the size of the static block, the alignment
    /// of that block, and an additional Tag type that may be used to create
    /// individual instantiations.
    ///
    /// \note Since this type creates static storage data, two instantiations
    ///       with the same size/align/tag will point to the same data -- hence
    ///       the ability to use type-tagging to produce unique instances.
    ///
    /// \tparam Size the size of the static allocation
    /// \tparam Align the alignment of the static allocation
    /// \tparam Tag the type used for a tag
    ///
    /// \satisfies BlockAllocator
    //////////////////////////////////////////////////////////////////////////
    template<std::size_t Size, std::size_t Align, typename Tag = void>
    class static_aligned_block_allocator
    {
      //----------------------------------------------------------------------
      // Public Members
      //----------------------------------------------------------------------
    public:

      static constexpr std::size_t block_size  = Size;
      static constexpr std::size_t block_align = Align;

      //----------------------------------------------------------------------
      // Constructor
      //----------------------------------------------------------------------
    public:

      /// \brief Constructs a static_block_allocator from another allocator
      static_aligned_block_allocator() noexcept;

      /// Deleted copy constructor
      static_aligned_block_allocator( const static_aligned_block_allocator& other ) = delete;

      /// \brief Move-constructs a static_block_allocator from another one
      ///
      /// \param other the other allocator to move
      static_aligned_block_allocator( static_aligned_block_allocator&& other ) noexcept = default;

      //----------------------------------------------------------------------

      /// Deleted copy assignment
      static_aligned_block_allocator& operator=( const static_aligned_block_allocator& other ) = delete;

      /// \brief Move-assigns a static_block_allocator from another one
      ///
      /// \param other the other allocator to move
      /// \return a reference to \c (*this)
      static_aligned_block_allocator& operator=( static_aligned_block_allocator&& other ) noexcept = default;

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

      using storage_type = std::aligned_storage_t<Size,Align>;

      static storage_type s_storage;
      static bool s_is_allocated; ///< is this block allocated?
    };

    template<std::size_t Size, std::size_t Align, typename Tag = void>
    using debug_static_aligned_block_allocator = debug_block_allocator<static_aligned_block_allocator<Size,Align,Tag>>;

  } // namespace memory
} // namespace bit

#include "detail/static_aligned_block_allocator.inl"

#endif /* BIT_MEMORY_BLOCK_ALLOCATORS_STATIC_ALIGNED_BLOCK_ALLOCATOR_HPP */
