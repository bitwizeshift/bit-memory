/**
 * \file block_allocator_reference.hpp
 *
 * \brief This header contains a type-erased view of a BlockAllocator concept
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */
#ifndef BIT_MEMORY_BLOCK_ALLOCATORS_BLOCK_ALLOCATOR_REFERENCE_HPP
#define BIT_MEMORY_BLOCK_ALLOCATORS_BLOCK_ALLOCATOR_REFERENCE_HPP

#include "../utilities/owner.hpp"                  // owner
#include "../utilities/allocator_info.hpp"         // allocator_info
#include "../utilities/memory_block.hpp"           // memory_block
#include "../utilities/macros.hpp"                 // BIT_MEMORY_UNUSED

#include "../concepts/BlockAllocator.hpp" // is_block_allocator
#include "../concepts/Stateless.hpp"      // is_stateless

#include "../traits/block_allocator_traits.hpp" // block_allocator_traits

#include <cstddef>     // std::size_t, std::ptrdiff_t
#include <type_traits> // std::integral_constant, std::is_same, etc
#include <memory>      // std::addressof

namespace bit {
  namespace memory {
    namespace detail {
      struct block_allocator_reference_vtable;
    } // namespace detail

    //////////////////////////////////////////////////////////////////////////
    /// \brief A type erased view of allocators that satisfy the BlockAllocator
    ///        concept.
    ///
    /// This view is non-owning, and thus does not participate in the lifetime
    /// of a given allocator.
    ///
    /// \satisfies{BlockAllocator}
    //////////////////////////////////////////////////////////////////////////
    class block_allocator_reference final
    {
      //----------------------------------------------------------------------
      // Constructor / Assignment
      //----------------------------------------------------------------------
    public:

      /// \brief Constructs a block allocator by type erasing the underlying
      ///        allocator
      ///
      /// \param allocator the block allocator to type-erase
      template<typename BlockAllocator,
              typename = std::enable_if_t<is_block_allocator<BlockAllocator>::value &&
                                         !std::is_same<block_allocator_reference,BlockAllocator>::value>>
      block_allocator_reference( BlockAllocator& allocator ) noexcept;

      /// \brief Move-constructs a block_allocator from an existing one
      ///
      /// \param other the other allocator to move
      block_allocator_reference( block_allocator_reference&& other ) noexcept = default;

      /// \brief Copy-constructs a block_allocator from an existing one
      ///
      /// \param other the other allocator to copy
      block_allocator_reference( const block_allocator_reference& other ) noexcept = default;

      //----------------------------------------------------------------------

      /// \brief Move-assigns a block_allocator from an existing one
      ///
      /// \param other the other allocator to move
      /// \return reference to \c (*this)
      block_allocator_reference& operator = ( block_allocator_reference&& other ) noexcept = default;

      /// \brief Copy-assigns a block_allocator from an existing one
      ///
      /// \param other the other allocator to copy
      /// \return reference to \c (*this)
      block_allocator_reference& operator = ( const block_allocator_reference& other ) noexcept = default;

      //----------------------------------------------------------------------
      // Block Allocations
      //----------------------------------------------------------------------
    public:

      /// \brief Allocates a block from the underlying block allocator
      ///
      /// \return the allocated memory block
      owner<memory_block> allocate_block();

      /// \brief Deallocates a block from the underlying block allocatore
      ///
      /// \param block the block to deallocate
      void deallocate_block( owner<memory_block> block );

      //----------------------------------------------------------------------
      // Observers
      //----------------------------------------------------------------------
    public:

      /// \brief Gets information about this allocator
      ///
      /// \return the allocator information
      allocator_info info() const noexcept;

      /// \brief Gets the next available block size for this allocator
      ///
      /// \return the next block size
      std::size_t next_block_size() const noexcept;

      //----------------------------------------------------------------------
      // Private Member Types
      //----------------------------------------------------------------------
    private:

      using vtable_type = detail::block_allocator_reference_vtable;

      template<typename Allocator>
      struct stateless_type;

      //-----------------------------------------------------------------------
      // Private Constructor
      //-----------------------------------------------------------------------
    private:

      /// \brief Constructs a block allocator reference to a stateless
      ///        block allocator
      ///
      /// This is an implementation function of
      /// \ref make_stateless_block_allocator_reference.
      template<typename BlockAllocator>
      explicit block_allocator_reference( stateless_type<BlockAllocator> ) noexcept;

      //----------------------------------------------------------------------
      // Private Members
      //----------------------------------------------------------------------
    private:

      void*              m_ptr;
      const vtable_type* m_vtable;

      template<typename>
      friend block_allocator_reference make_stateless_block_allocator_reference()
        noexcept;
    };

    //-------------------------------------------------------------------------
    // Utility
    //-------------------------------------------------------------------------

    /// \brief Makes a block_allocator_reference that refers to a stateless
    ///        block allocator
    ///
    /// This creates a reference to any stateless block allocator, even though
    /// different instances may have different underlying addresses.
    ///
    /// \tparam StatelessBlockAllocator the type of a stateless block allocator
    template<typename StatelessBlockAllocator>
    block_allocator_reference make_stateless_block_allocator_reference() noexcept;

  } // namespace memory
} // namespace bit

#include "detail/block_allocator_reference.inl"

#endif /* BIT_MEMORY_BLOCK_ALLOCATORS_BLOCK_ALLOCATOR_REFERENCE_HPP */
