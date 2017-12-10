/**
 * \file any_block_allocator.hpp
 *
 * \brief This header contains a type-erased view of a BlockAllocator concept
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */
#ifndef BIT_MEMORY_BLOCK_ALLOCATORS_ANY_BLOCK_ALLOCATOR_HPP
#define BIT_MEMORY_BLOCK_ALLOCATORS_ANY_BLOCK_ALLOCATOR_HPP

#include "../owner.hpp"          // owner
#include "../memory_block.hpp"   // memory_block
#include "../allocator_info.hpp" // allocator_info

#include "../block_allocator_traits.hpp"

namespace bit {
  namespace memory {
    namespace detail {
      struct any_block_allocator_vtable;
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
    class any_block_allocator final
    {
      template<typename A>
      using is_enabled = std::integral_constant<bool,
        is_block_allocator<std::decay_t<A>>::value &&
        !std::is_same<any_block_allocator,std::decay_t<A>>::value
      >;

      //----------------------------------------------------------------------
      // Constructor / Assignment
      //----------------------------------------------------------------------
    public:

      /// \brief Constructs a block allocator by type erasing the underlying
      ///        allocator
      ///
      /// \param allocator the block allocator to type-erase
      template<typename BlockAllocator, typename = std::enable_if_t<is_enabled<BlockAllocator>::value>>
      any_block_allocator( BlockAllocator& allocator ) noexcept;

      /// \brief Move-constructs a block_allocator from an existing one
      ///
      /// \param other the other allocator to move
      any_block_allocator( any_block_allocator&& other ) noexcept = default;

      /// \brief Copy-constructs a block_allocator from an existing one
      ///
      /// \param other the other allocator to copy
      any_block_allocator( const any_block_allocator& other ) noexcept = default;

      //----------------------------------------------------------------------

      /// \brief Move-assigns a block_allocator from an existing one
      ///
      /// \param other the other allocator to move
      /// \return reference to \c (*this)
      any_block_allocator& operator = ( any_block_allocator&& other ) noexcept = default;

      /// \brief Copy-assigns a block_allocator from an existing one
      ///
      /// \param other the other allocator to copy
      /// \return reference to \c (*this)
      any_block_allocator& operator = ( const any_block_allocator& other ) noexcept = default;

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

      using vtable_type = detail::any_block_allocator_vtable;

      //----------------------------------------------------------------------
      // Private Members
      //----------------------------------------------------------------------
    private:

      void*        m_ptr;
      vtable_type* m_vtable;
    };

  } // namespace memory
} // namespace bit

#include "detail/any_block_allocator.inl"

#endif /* BIT_MEMORY_BLOCK_ALLOCATORS_BLOCK_ANY_ALLOCATOR_HPP */
