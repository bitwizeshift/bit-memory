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

      struct any_block_allocator_vtable
      {
        using deallocate_block_fn_t = void(*)( void*, memory_block );
        using allocate_block_fn_t   = memory_block(*)( void* );
        using info_fn_t             = allocator_info(*)( const void* );
        using next_block_size_fn_t  = std::size_t(*)( const void* );

        deallocate_block_fn_t deallocate_fn;
        allocate_block_fn_t   allocate_fn;
        info_fn_t             info_fn;
        next_block_size_fn_t  next_block_fn;

        template<typename BlockAllocator>
        static any_block_allocator_vtable* get_vtable()
        {
          using traits_type = block_allocator_traits<BlockAllocator>;

          static auto s_vtable = []()
          {
            any_block_allocator_vtable table;

            table.allocate_fn = +[](void* p) -> void*
            {
              auto* instance = static_cast<BlockAllocator*>(p);

              return traits_type::allocate_block( *instance );
            };

            table.deallocate_fn = +[](void* p, memory_block block)
            {
              auto* instance = static_cast<BlockAllocator*>(p);

              traits_type::deallocate_block( *instance, block );
            };

            table.info_fn = +[]( const void* p ) -> allocator_info
            {
              auto* instance = static_cast<const BlockAllocator*>(p);

              return traits_type::info( *instance );
            };

            table.next_block_fn = +[]( const void* p ) -> std::size_t
            {
              auto* instance = static_cast<const BlockAllocator*>(p);

              return traits_type::next_block_size( *instance );
            };

            return table;
          }();

          return &s_vtable;
        }
      };

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
        is_block_allocator<A>::value && !std::is_same<any_block_allocator,A>::value
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
      any_block_allocator( BlockAllocator&& allocator ) noexcept;

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
