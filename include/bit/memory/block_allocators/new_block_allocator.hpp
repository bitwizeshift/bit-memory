/**
 * \file new_block_allocator.hpp
 *
 * \brief This header contains an API for allocating memory blocks that use
 *        ::operator new as the underlying allocation scheme.
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */

#ifndef BIT_MEMORY_BLOCK_ALLOCATORS_NEW_BLOCK_ALLOCATOR_HPP
#define BIT_MEMORY_BLOCK_ALLOCATORS_NEW_BLOCK_ALLOCATOR_HPP

#include "../detail/dynamic_size_type.hpp" // detail::dynamic_size_type

#include "../macros.hpp"                   // BIT_MEMORY_UNLIKELY
#include "../memory.hpp"                   // owner
#include "../memory_block.hpp"             // memory_block

#include "cached_block_allocator.hpp"

#include <new>         // ::new
#include <type_traits> // std::true_type, std::false_type, etc

namespace bit {
  namespace memory {
    namespace detail {
      template<std::size_t Size>
      struct new_block_allocator_base
      {
        using block_size = std::integral_constant<std::size_t,Size>;
        using is_stateless = std::true_type;

        new_block_allocator_base() noexcept = default;
        new_block_allocator_base( new_block_allocator_base&& ) noexcept = default;
        new_block_allocator_base( const new_block_allocator_base& ) noexcept = default;
        new_block_allocator_base& operator=( new_block_allocator_base&& ) noexcept = default;
        new_block_allocator_base& operator=( const new_block_allocator_base& ) noexcept = default;
      };

      template<>
      struct new_block_allocator_base<dynamic_size>
      {
        using is_stateless = std::false_type;

        new_block_allocator_base() noexcept = default;
        new_block_allocator_base( new_block_allocator_base&& ) noexcept = default;
        new_block_allocator_base( const new_block_allocator_base& ) = delete;
        new_block_allocator_base& operator=( new_block_allocator_base&& ) = delete;
        new_block_allocator_base& operator=( const new_block_allocator_base& ) = delete;
      };

    } // namespace detail

    //////////////////////////////////////////////////////////////////////////
    /// \brief A block allocator that uses ::operator new(...) to allocate
    ///        memory blocks
    ///
    /// \satisfies BlockAllocator
    //////////////////////////////////////////////////////////////////////////
    template<std::size_t Size>
    class new_block_allocator
      : private detail::dynamic_size_type<0,Size>,
        private detail::new_block_allocator_base<Size>
    {
      using block_size_member = detail::dynamic_size_type<0,Size>;

      //----------------------------------------------------------------------
      // Public Member Types
      //----------------------------------------------------------------------
    public:

      using block_alignment = std::integral_constant<std::size_t,alignof(std::max_align_t)>;

      //----------------------------------------------------------------------
      // Constructors
      //----------------------------------------------------------------------
    public:

      /// \brief Default-constructs this new_block_allocator
      ///
      /// This is only enabled for non-dynamic new_block_allocators
      new_block_allocator() noexcept = default;

      /// \brief Constructs a new_block_allocator that will distribute
      ///        blocks of the specified \p size
      ///
      /// \param size the size of each block allocation
      template<std::size_t I = Size, typename = std::enable_if_t<!detail::dynamic_size_type<0,I>::is_stateless::value>>
      explicit new_block_allocator( std::size_t size ) noexcept;

      /// \brief Move-constructs a new_block_allocator from another allocator
      ///
      /// \param other the other new_block_allocator to move
      new_block_allocator( new_block_allocator&& other ) = default;

      /// \brief Copy-constructs a new_block_allocator from another allocator
      ///
      /// \note This is only an enabled overload for stateless (non-dynamic)
      ///       new block allocators
      ///
      /// \param other the other new_block_allocator to copy
      new_block_allocator( const new_block_allocator& other ) = default;

      //-----------------------------------------------------------------------

      /// \brief Move-assigns a new_block_allocator from another allocator
      ///
      /// \note This is only an enabled overload for stateless (non-dynamic)
      ///       new block allocators
      ///
      /// \param other the other new_block_allocator to move
      /// \return reference to \c (*this)
      // Deleted move assignment
      new_block_allocator& operator=( new_block_allocator&& other ) = default;

      /// \brief Copy-assigns a new_block_allocator from another allocator
      ///
      /// \note This is only an enabled overload for stateless (non-dynamic)
      ///       new block allocators
      ///
      /// \param other the other new_block_allocator to copy
      /// \return reference to \c (*this)
      new_block_allocator& operator=( const new_block_allocator& other ) = default;

      //----------------------------------------------------------------------
      // Block Allocations
      //----------------------------------------------------------------------
    public:

      /// \brief Allocates a new'ed memory_block
      ///
      /// \return a null memory_block
      owner<memory_block> allocate_block() noexcept;

      /// \brief Deallocates a new'ed memory_block
      ///
      /// \param block the block to deallocate
      void deallocate_block( owner<memory_block> block ) noexcept;

      //----------------------------------------------------------------------
      // Private Members
      //----------------------------------------------------------------------
    private:

      template<std::size_t S>
      friend bool operator==( const new_block_allocator<S>&,
                              const new_block_allocator<S>& ) noexcept;
    };

    //-------------------------------------------------------------------------
    // Comparisons
    //-------------------------------------------------------------------------

    template<std::size_t S>
    bool operator==( const new_block_allocator<S>& lhs,
                     const new_block_allocator<S>& rhs ) noexcept;

    template<std::size_t S>
    bool operator!=( const new_block_allocator<S>& lhs,
                     const new_block_allocator<S>& rhs ) noexcept;

    //-------------------------------------------------------------------------
    // Utilities
    //-------------------------------------------------------------------------

    template<std::size_t Size>
    using cached_new_block_allocator = cached_block_allocator<new_block_allocator<Size>>;

    using dynamic_new_block_allocator = new_block_allocator<dynamic_size>;

    using cached_dynamic_new_block_allocator = cached_block_allocator<dynamic_new_block_allocator>;

  } // namespace memory
} // namespace bit

#include "detail/new_block_allocator.inl"

#endif /* BIT_MEMORY_BLOCK_ALLOCATORS_NEW_BLOCK_ALLOCATOR_HPP */
