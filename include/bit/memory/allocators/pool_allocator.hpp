/**
 * \file pool_allocator.hpp
 *
 * \brief This header contains the definition for an allocator that creates
 *        fixed-sized allocations from a reused pool
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */
#ifndef BIT_MEMORY_ALLOCATORS_POOL_ALLOCATOR_HPP
#define BIT_MEMORY_ALLOCATORS_POOL_ALLOCATOR_HPP

#include "detail/named_allocator.hpp" // detail::named_allocator

#include "../macros.hpp"            // BIT_MEMORY_ASSUME
#include "../memory_block.hpp"      // memory_block
#include "../pointer_utilities.hpp" // is_power_of_two

#include <cassert>

namespace bit {
  namespace memory {

    ///////////////////////////////////////////////////////////////////////////
    /// \brief This allocator creates a pool of fixed-sized chunk entries for
    ///        allocations
    ///
    /// \satisfies ExtendedAllocator
    ///////////////////////////////////////////////////////////////////////////
    class pool_allocator
    {
      // TODO: Add template-argument for chunk-size

      //-----------------------------------------------------------------------
      // Public Member Types
      //-----------------------------------------------------------------------
    public:

      /// The max alignment is limited to 128 bytes due to an internal
      /// requirement that it stores the offset information
      using max_alignment = std::integral_constant<std::size_t,128>;

      //-----------------------------------------------------------------------
      // Constructors / Assignment
      //-----------------------------------------------------------------------
    public:

      /// \brief Constructs a pool allocator with chunk sizes of \p chunk_size,
      ///        in the arena indicated by \p block
      ///
      /// \param chunk_size the size of each entry in the pool allocator
      /// \param block the block to allocate from
      pool_allocator( std::size_t chunk_size, memory_block block );

      /// \brief Move-constructs the pool_allocator from another allocator
      ///
      /// \param other the other allocator to move
      pool_allocator( pool_allocator&& other ) noexcept = default;

      // Deleted copy construction
      pool_allocator( const pool_allocator& other ) = delete;

      //-----------------------------------------------------------------------

      // Deleted move assignment
      pool_allocator& operator=( pool_allocator&& other ) = delete;

      // Deleted copy assignment
      pool_allocator& operator=( const pool_allocator& other ) = delete;

      //-----------------------------------------------------------------------
      // Allocations / Deallocations
      //-----------------------------------------------------------------------
    public:

      /// \brief Tries to allocate \p size bytes with the alignment of \p align,
      ///        offset by \p offset
      ///
      /// \param size the requested size of the allocation
      /// \param align the requested alignment of the allocation
      /// \param offset the amount to offset the alignment
      /// \return pointer to the allocated memory, or \c nullptr on failure
      void* try_allocate( std::size_t size,
                          std::size_t align,
                          std::size_t offset = 0 ) noexcept;

      /// \brief Deallocates memory previously allocated from a call to
      ///        \c try_allocate
      ///
      /// \param p the pointer to the memory to deallocate
      /// \param size the size of the memory previously provided to try_allocate
      void deallocate( void* p, std::size_t size );

      //-----------------------------------------------------------------------
      // Observers
      //-----------------------------------------------------------------------
    public:

      /// \brief Determines whether the pointer \p p is owned by this allocator
      ///
      /// \return \c true if the pointer \p p is originally from this allocator
      bool owns( const void* p ) const noexcept;

      /// \brief Determines the max size that this allocator can allocate
      ///
      /// \return the max size
      std::size_t max_size() const noexcept;

      //-----------------------------------------------------------------------
      // Private Members
      //-----------------------------------------------------------------------
    private:

      void*        m_head;
      memory_block m_block;
      std::size_t  m_chunk_size;

      //-----------------------------------------------------------------------
      // Private Member Functions
      //-----------------------------------------------------------------------
    private:

      /// \brief Pushes the pointer \p p into the freelist
      ///
      /// \param p the pointer to push
      void push_freelist_entry( void* p ) noexcept;

      /// \brief Pops an entry from the freelist
      ///
      /// \return the freelist entry
      void* pop_freelist_entry() noexcept;

      friend bool operator==( const pool_allocator&, const pool_allocator&) noexcept;
    };

    //-------------------------------------------------------------------------
    // Comparison
    //-------------------------------------------------------------------------

    bool operator==( const pool_allocator& lhs, const pool_allocator& rhs ) noexcept;
    bool operator!=( const pool_allocator& lhs, const pool_allocator& rhs ) noexcept;

    //-------------------------------------------------------------------------
    // Utilities
    //-------------------------------------------------------------------------

    using named_pool_allocator = detail::named_allocator<pool_allocator>;

  } // namespace memory
} // namespace bit

#include "detail/pool_allocator.inl"

#endif /* BIT_MEMORY_ALLOCATORS_POOL_ALLOCATOR_HPP */
