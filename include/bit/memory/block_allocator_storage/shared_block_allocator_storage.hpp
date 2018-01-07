/**
 * \file shared_block_allocator_storage.hpp
 *
 * \brief This header contains a BlockAllocatorStorage type that has
 *        shared-ownership semantics of the underlying block allocator
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */
#ifndef BIT_MEMORY_BLOCK_ALLOCATOR_STORAGE_SHARED_BLOCK_ALLOCATOR_STORAGE_HPP
#define BIT_MEMORY_BLOCK_ALLOCATOR_STORAGE_SHARED_BLOCK_ALLOCATOR_STORAGE_HPP

#include "../concepts/BlockAllocator.hpp"

#include <memory> // std::shared_ptr

namespace bit {
  namespace memory {

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Shared-ownership storage of a heap-allocated allocator
    ///
    /// \satisfies{AllocatorStorage}
    ///
    /// \tparam BlockAllocator a \ref BlockAllocator type
    ///////////////////////////////////////////////////////////////////////////
    template<typename BlockAllocator>
    class shared_block_allocator_storage
    {
      static_assert( is_block_allocator<BlockAllocator>::value,
                     "shared_block_allocator_storage can only be used with"
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

      /// \brief Constructs a shared_block_allocator_storage from a shared pointer
      ///
      /// \param alloc the shared pointer to the allocator
      explicit shared_block_allocator_storage( std::shared_ptr<BlockAllocator> alloc ) noexcept;

      /// \brief Move-constructs this shared_block_allocator_storage from an
      ///        existing one
      ///
      /// \param other the other storage to move
      shared_block_allocator_storage( shared_block_allocator_storage&& other ) = default;

      /// \brief Copy-constructs this shared_block_allocator_storage from an
      ///        existing one
      ///
      /// \param other the other storage to copy
      shared_block_allocator_storage( const shared_block_allocator_storage& other ) = default;

      //-----------------------------------------------------------------------

      /// \brief Move-assigns this shared_block_allocator_storage from an
      ///        existing one
      ///
      /// \param other the other storage to move
      /// \return reference to \c (*this)
      shared_block_allocator_storage& operator=( shared_block_allocator_storage&& ) = default;

      /// \brief Copy-assigns this shared_block_allocator_storage from an
      ///        existing one
      ///
      /// \param other the other storage to copy
      /// \return reference to \c (*this)
      shared_block_allocator_storage& operator=( const shared_block_allocator_storage& ) = default;

      //-----------------------------------------------------------------------
      // Observers
      //-----------------------------------------------------------------------
    public:

      /// \{
      /// \brief Retrieves a reference to the underlying block allocator
      ///
      /// \return the reference to the allocator
      BlockAllocator& get_block_allocator() noexcept;
      const BlockAllocator& get_block_allocator() const noexcept;
      /// \}

      //-----------------------------------------------------------------------
      // Private Members
      //-----------------------------------------------------------------------
    private:

      std::shared_ptr<BlockAllocator> m_allocator;
    };

    //-------------------------------------------------------------------------
    // Utilities
    //-------------------------------------------------------------------------

    template<typename Allocator, typename...Args>
    shared_block_allocator_storage<Allocator>
      make_shared_block_allocator_storage( Args&&...args );

    template<typename Allocator, typename UAllocator, typename...Args>
    shared_block_allocator_storage<Allocator>
      allocate_shared_block_allocator_storage( UAllocator& alloc, Args&&...args );

  } // namespace memory
} // namespace bit

#include "detail/shared_block_allocator_storage.inl"

#endif /* BIT_MEMORY_BLOCK_ALLOCATOR_STORAGE_SHARED_BLOCK_ALLOCATOR_STORAGE_HPP */
