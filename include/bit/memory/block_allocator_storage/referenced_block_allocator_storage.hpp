/**
 * \file allocator_block_reference_storage.hpp
 *
 * \brief This header contains a storage policy using block allocators accessed
 *        by a reference
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */
#ifndef BIT_MEMORY_BLOCK_ALLOCATOR_STORAGE_REFERENCED_BLOCK_ALLOCATOR_STORAGE_HPP
#define BIT_MEMORY_BLOCK_ALLOCATOR_STORAGE_REFERENCED_BLOCK_ALLOCATOR_STORAGE_HPP

#include "../concepts/BlockAllocator.hpp"

#include <memory> // std::addressof

namespace bit {
  namespace memory {

    ///////////////////////////////////////////////////////////////////////////
    /// \brief A storage type for holding onto allocators by a reference.
    ///
    /// The lifetime of the underlying referenced allocator is the same as the
    /// original allocator being referenced; meaning that this storage type
    /// should be treated carefully to avoid dangling references.
    ///
    /// \satisfies{BlockAllocatorStorage}
    ///
    /// \tparam BlockAllocator an \ref BlockAllocator type
    ///////////////////////////////////////////////////////////////////////////
    template<typename BlockAllocator>
    class referenced_block_allocator_storage
    {
      static_assert( is_block_allocator<BlockAllocator>::value,
                     "referenced_block_allocator_storage can only be used with"
                     "types that satisfy the BlockAllocator concept");

      //-----------------------------------------------------------------------
      // Public Member Types
      //-----------------------------------------------------------------------
    public:

      using block_allocator_type = BlockAllocator;

      //-----------------------------------------------------------------------
      // Constructors / Assignment
      //-----------------------------------------------------------------------
    public:

      /// \brief Constructs a referenced_block_allocator_storage from an allocator
      ///
      /// \param alloc the allocator to reference
      explicit referenced_block_allocator_storage( BlockAllocator& alloc ) noexcept;

      /// \brief Move-constructs this referenced_block_allocator_storage from an
      ///        existing one
      ///
      /// \param other the other storage to move
      referenced_block_allocator_storage( referenced_block_allocator_storage&& other ) noexcept = default;

      /// \brief Copy-constructs this referenced_block_allocator_storage from an
      ///        existing one
      ///
      /// \param other the other storage to copy
      referenced_block_allocator_storage( const referenced_block_allocator_storage& other ) noexcept = default;

      //-----------------------------------------------------------------------

      /// \brief Move-assigns this referenced_block_allocator_storage from an
      ///        existing one
      ///
      /// \param other the other storage to move
      /// \return reference to \c (*this)
      referenced_block_allocator_storage& operator=( referenced_block_allocator_storage&& other ) noexcept = default;

      /// \brief Copy-assigns this referenced_block_allocator_storage from an
      ///        existing one
      ///
      /// \param other the other storage to copy
      /// \return reference to \c (*this)
      referenced_block_allocator_storage& operator=( const referenced_block_allocator_storage& other ) noexcept = default;

      //-----------------------------------------------------------------------
      // Observers
      //-----------------------------------------------------------------------
    public:

      /// \{
      /// \brief Gets the stored block allocator
      ///
      /// \return reference to the stored block allocator
      BlockAllocator& get_block_allocator() noexcept;
      const BlockAllocator& get_block_allocator() const noexcept;
      /// \}

      //-----------------------------------------------------------------------
      // Private Members
      //-----------------------------------------------------------------------
    private:

      BlockAllocator* m_allocator;
    };

    //-------------------------------------------------------------------------
    // Utilities
    //-------------------------------------------------------------------------

    /// \brief Makes a referenced_block_allocator_storage of the given
    ///        block allocator
    ///
    /// \param alloc the allocator to reference
    /// \return the stateless allocator storage
    template<typename BlockAllocator>
    referenced_block_allocator_storage<BlockAllocator>
      make_referenced_block_allocator_storage( BlockAllocator& alloc ) noexcept;

  } // namespace memory
} // namespace bit

#include "detail/referenced_block_allocator_storage.inl"

#endif /* BIT_MEMORY_BLOCK_ALLOCATOR_STORAGE_REFERENCED_BLOCK_ALLOCATOR_STORAGE_HPP */
