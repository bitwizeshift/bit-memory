/**
 * \file malloc_block_allocator.hpp
 *
 * \brief This header contains an implementation of a non-aligned, malloc
 *        block allocator
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */
#ifndef BIT_MEMORY_BLOCK_ALLOCATORS_MALLOC_BLOCK_ALLOCATOR_HPP
#define BIT_MEMORY_BLOCK_ALLOCATORS_MALLOC_BLOCK_ALLOCATOR_HPP

#include "detail/cached_block_allocator.hpp" // detail::cached_block_allocator
#include "detail/named_block_allocator.hpp"  // detail::named_block_allocator
#include "../detail/dynamic_size_type.hpp"   // detail::dynamic_size_type

#include "../macros.hpp"                   // BIT_MEMORY_UNLIKELY
#include "../owner.hpp"                    // owner
#include "../memory_block.hpp"             // memory_block


#include <cstddef>     // std::size_t, std::ptrdiff_t
#include <cstdlib>     // std::malloc, std::free
#include <type_traits> // std::true_type, std::false_type, etc

namespace bit {
  namespace memory {
    namespace detail {
      template<std::size_t Size>
      struct malloc_block_allocator_base
      {
        using block_size   = std::integral_constant<std::size_t,Size>;
        using is_stateless = std::true_type;

        malloc_block_allocator_base() noexcept = default;
        malloc_block_allocator_base( malloc_block_allocator_base&& ) noexcept = default;
        malloc_block_allocator_base( const malloc_block_allocator_base& ) noexcept = default;
        malloc_block_allocator_base& operator=( malloc_block_allocator_base&& ) noexcept = default;
        malloc_block_allocator_base& operator=( const malloc_block_allocator_base& ) noexcept = default;
      };

      template<>
      struct malloc_block_allocator_base<dynamic_size>
      {
        using is_stateless = std::false_type;

        malloc_block_allocator_base() noexcept = default;
        malloc_block_allocator_base( malloc_block_allocator_base&& ) noexcept = default;
        malloc_block_allocator_base( const malloc_block_allocator_base& ) = delete;
        malloc_block_allocator_base& operator=( malloc_block_allocator_base&& ) = delete;
        malloc_block_allocator_base& operator=( const malloc_block_allocator_base& ) = delete;
      };
    } // namespace detail

    //////////////////////////////////////////////////////////////////////////
    /// \brief A block allocator that wraps around raw calls to std::malloc
    ///        and std::free
    ///
    /// \satisfies{BlockAllocator}
    /// \satisfies{Stateless}
    //////////////////////////////////////////////////////////////////////////
    template<std::size_t Size>
    class malloc_block_allocator
      : private detail::dynamic_size_type<0,Size>,
        private detail::malloc_block_allocator_base<(Size!=std::size_t(-1))>
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

      /// \brief Default-constructs this malloc_block_allocator
      ///
      /// This is only enabled for non-dynamic new_block_allocators
      malloc_block_allocator() noexcept = default;

      /// \brief Constructs a malloc_block_allocator that will distribute
      ///        blocks of the specified \p size
      ///
      /// \param size the size of each block allocation
      template<std::size_t I = Size, typename = std::enable_if_t<!detail::dynamic_size_type<0,I>::is_stateless::value>>
      explicit malloc_block_allocator( std::size_t size ) noexcept;

      /// \brief Move-constructs a malloc_block_allocator from another allocator
      ///
      /// \param other the other malloc_block_allocator to move
      malloc_block_allocator( malloc_block_allocator&& other ) = default;

      /// \brief Copy-constructs a malloc_block_allocator from another allocator
      ///
      /// \param other the other malloc_block_allocator to copy
      malloc_block_allocator( const malloc_block_allocator& other ) = default;

      //-----------------------------------------------------------------------

      /// \brief Move-assigns a malloc_block_allocator from another allocator
      ///
      /// \param other the other malloc_block_allocator to move
      /// \return reference to \c (*this)
      malloc_block_allocator& operator=( malloc_block_allocator&& other ) = default;

      /// \brief Copy-assigns a malloc_block_allocator from another allocator
      ///
      /// \param other the other malloc_block_allocator to copy
      /// \return reference to \c (*this)
      malloc_block_allocator& operator=( const malloc_block_allocator& other ) = default;

      //----------------------------------------------------------------------
      // Block Allocations
      //----------------------------------------------------------------------
    public:

      /// \brief Allocates a malloc'ed memory_block
      ///
      /// \return a null memory_block
      owner<memory_block> allocate_block() noexcept;

      /// \brief Deallocates a malloc'ed memory_block
      ///
      /// \param block the block to deallocate
      void deallocate_block( owner<memory_block> block ) noexcept;

      //----------------------------------------------------------------------
      // Private Members
      //----------------------------------------------------------------------
    private:

      template<std::size_t S>
      friend bool operator==( const malloc_block_allocator<S>&,
                              const malloc_block_allocator<S>& ) noexcept;
    };

    //-------------------------------------------------------------------------
    // Comparisons
    //-------------------------------------------------------------------------

    template<std::size_t Size>
    bool operator==( const malloc_block_allocator<Size>& lhs,
                     const malloc_block_allocator<Size>& rhs ) noexcept;

    template<std::size_t Size>
    bool operator!=( const malloc_block_allocator<Size>& lhs,
                     const malloc_block_allocator<Size>& rhs ) noexcept;

    //-------------------------------------------------------------------------
    // Utiltiies
    //-------------------------------------------------------------------------

    using dynamic_malloc_block_allocator = malloc_block_allocator<dynamic_size>;

    template<std::size_t Size>
    using cached_malloc_block_allocator = detail::cached_block_allocator<malloc_block_allocator<Size>>;

    using cached_dynamic_malloc_block_allocator = detail::cached_block_allocator<dynamic_malloc_block_allocator>;

    //-------------------------------------------------------------------------

    template<std::size_t Size>
    using named_malloc_block_allocator = detail::named_block_allocator<malloc_block_allocator<Size>>;

    template<std::size_t Size>
    using named_cached_malloc_block_allocator = detail::named_block_allocator<cached_malloc_block_allocator<Size>>;

    using named_dynamic_malloc_block_allocator = detail::named_block_allocator<dynamic_malloc_block_allocator>;

    using named_cached_dynamic_malloc_block_allocator = detail::named_block_allocator<cached_dynamic_malloc_block_allocator>;

  } // namespace memory
} // namespace bit

#include "detail/malloc_block_allocator.inl"

#endif /* BIT_MEMORY_BLOCK_ALLOCATORS_MALLOC_BLOCK_ALLOCATOR_HPP */
