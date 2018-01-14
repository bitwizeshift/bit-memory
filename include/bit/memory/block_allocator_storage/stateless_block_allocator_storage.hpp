/**
 * \file stateless_block_allocator_storage.hpp
 *
 * \brief This header contains a storage policy for stateless block allocators
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */
#ifndef BIT_MEMORY_BLOCK_ALLOCATOR_STORAGE_STATELESS_BLOCK_ALLOCATOR_STORAGE_HPP
#define BIT_MEMORY_BLOCK_ALLOCATOR_STORAGE_STATELESS_BLOCK_ALLOCATOR_STORAGE_HPP

#include "../utilities/ebo_storage.hpp"

#include "../concepts/Stateless.hpp"
#include "../concepts/Allocator.hpp"

namespace bit {
  namespace memory {

    ///////////////////////////////////////////////////////////////////////////
    /// \brief A storage type for holding onto stateless block allocators
    ///
    /// \satisfies{AllocatorStorage}
    ///
    /// \tparam StatelessBlockAllocator an \ref BlockAllocator type that
    ///         additionally satisfies \ref Stateless
    ///////////////////////////////////////////////////////////////////////////
    template<typename StatelessBlockAllocator>
    class stateless_block_allocator_storage
      : private ebo_storage<StatelessBlockAllocator>
    {
      using base_type = ebo_storage<StatelessBlockAllocator>;

      static_assert( is_stateless<StatelessBlockAllocator>::value &&
                     is_allocator<StatelessBlockAllocator>::value,
                     "stateless_allocator_storage can only be used with"
                     "Allocator types that satisfy Stateless and Allocator");

      //-----------------------------------------------------------------------
      // Public Member Types
      //-----------------------------------------------------------------------
    public:

      using block_allocator_type = StatelessBlockAllocator;

      //-----------------------------------------------------------------------
      // Constructors / Assignment
      //-----------------------------------------------------------------------
    public:

      /// \brief Default-constructs the stateless_allocator_storage
      stateless_block_allocator_storage() = default;

      /// \brief Move-constructs this shared_allocator_storage from an
      ///        existing one
      ///
      /// \param other the other storage to move
      stateless_block_allocator_storage( stateless_block_allocator_storage&& other ) noexcept = default;

      /// \brief Copy-constructs this shared_allocator_storage from an
      ///        existing one
      ///
      /// \param other the other storage to copy
      stateless_block_allocator_storage( const stateless_block_allocator_storage& other ) noexcept = default;

      //-----------------------------------------------------------------------

      /// \brief Move-assigns this stateless_allocator_storage from an
      ///        existing one
      ///
      /// \param other the other storage to move
      /// \return reference to \c (*this)
      stateless_block_allocator_storage& operator=( stateless_block_allocator_storage&& other ) noexcept = default;

      /// \brief Copy-assigns this stateless_allocator_storage from an
      ///        existing one
      ///
      /// \param other the other storage to copy
      /// \return reference to \c (*this)
      stateless_block_allocator_storage& operator=( const stateless_block_allocator_storage& other ) noexcept = default;

      //-----------------------------------------------------------------------
      // Observers
      //-----------------------------------------------------------------------
    public:

      /// \{
      /// \brief Gets the stored allocator
      ///
      /// \return reference to the stored allocator
      StatelessBlockAllocator& get_block_allocator() noexcept;
      const StatelessBlockAllocator& get_block_allocator() const noexcept;
      /// \}
    };

    //-------------------------------------------------------------------------
    // Utilities
    //-------------------------------------------------------------------------

    /// \brief Makes a stateless_block_allocator_storage of the given
    ///        block allocator type
    ///
    /// \note This function is mostly superflous, since the constructor of
    ///       stateless_block_allocator_storage does the same functionality.
    ///       This exists for symmetry with the other storage 'make' functions
    ///
    /// \return the stateless allocator storage
    template<typename Allocator>
    stateless_block_allocator_storage<Allocator>
      make_stateless_block_allocator_storage();

  } // namespace memory
} // namespace bit

#include "detail/stateless_block_allocator_storage.inl"

#endif /* BIT_MEMORY_BLOCK_ALLOCATOR_STORAGE_STATELESS_BLOCK_ALLOCATOR_STORAGE_HPP */
