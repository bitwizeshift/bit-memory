/**
 * \file aligned_block_allocator.hpp
 *
 * \brief This header contains the implementation of a block allocator that
 *        allocates aligned memory
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */

#ifndef BIT_MEMORY_BLOCK_ALLOCATORS_ALIGNED_BLOCK_ALLOCATOR_HPP
#define BIT_MEMORY_BLOCK_ALLOCATORS_ALIGNED_BLOCK_ALLOCATOR_HPP

#include "detail/named_block_allocator.hpp" // detail::named_block_allocator
#include "../detail/dynamic_size_type.hpp"  // dynamic_size, detail::dynamic_size_type

#include "../owner.hpp"          // owner
#include "../memory.hpp"         // is_power_of_two
#include "../aligned_memory.hpp" // aligned_allocate
#include "../memory_block.hpp"   // memory_block

#include "cached_block_allocator.hpp" // cached_block_allocator

#include <type_traits> // std::true_type, std::false_type, etc
#include <cstddef>     // std::size_t
#include <cassert>     // assert

namespace bit {
  namespace memory {
    namespace detail {

      template<std::size_t Size, std::size_t Align>
      struct aligned_block_allocator_base
        : detail::dynamic_size_type<0,Size>,
          detail::dynamic_size_type<1,Align>
      {
        using block_size      = std::integral_constant<std::size_t,Size>;
        using block_alignment = std::integral_constant<std::size_t,Align>;
        using is_stateless    = std::true_type;

        aligned_block_allocator_base() noexcept = default;
        aligned_block_allocator_base( aligned_block_allocator_base&& ) noexcept = default;
        aligned_block_allocator_base( const aligned_block_allocator_base& ) noexcept = default;
        aligned_block_allocator_base& operator=( aligned_block_allocator_base&& ) noexcept = default;
        aligned_block_allocator_base& operator=( const aligned_block_allocator_base& ) noexcept = default;
      };

      //-----------------------------------------------------------------------

      template<std::size_t Size>
      struct aligned_block_allocator_base<Size,dynamic_size>
        : detail::dynamic_size_type<0,Size>,
          detail::dynamic_size_type<1,dynamic_size>
      {
        using block_size   = std::integral_constant<std::size_t,Size>;
        using is_stateless = std::false_type;

        aligned_block_allocator_base( std::size_t align ) noexcept
          : detail::dynamic_size_type<1,dynamic_size>( align )
        {

        }
        aligned_block_allocator_base( aligned_block_allocator_base&& ) noexcept = default;
        aligned_block_allocator_base( const aligned_block_allocator_base& ) = delete;
        aligned_block_allocator_base& operator=( aligned_block_allocator_base&& ) = delete;
        aligned_block_allocator_base& operator=( const aligned_block_allocator_base& ) = delete;
      };

      //-----------------------------------------------------------------------

      template<std::size_t Align>
      struct aligned_block_allocator_base<dynamic_size,Align>
        : detail::dynamic_size_type<0,dynamic_size>,
          detail::dynamic_size_type<1,Align>
      {
        using block_alignment = std::integral_constant<std::size_t,Align>;
        using is_stateless    = std::false_type;

        aligned_block_allocator_base( std::size_t size ) noexcept
          : detail::dynamic_size_type<0,dynamic_size>( size )
        {

        }
        aligned_block_allocator_base( aligned_block_allocator_base&& ) noexcept = default;
        aligned_block_allocator_base( const aligned_block_allocator_base& ) = delete;
        aligned_block_allocator_base& operator=( aligned_block_allocator_base&& ) = delete;
        aligned_block_allocator_base& operator=( const aligned_block_allocator_base& ) = delete;
      };

      //-----------------------------------------------------------------------

      template<>
      struct aligned_block_allocator_base<dynamic_size,dynamic_size>
        : detail::dynamic_size_type<0,dynamic_size>,
          detail::dynamic_size_type<1,dynamic_size>
      {
        using is_stateless = std::false_type;

        aligned_block_allocator_base( std::size_t size, std::size_t align ) noexcept
          : detail::dynamic_size_type<0,dynamic_size>( size ),
            detail::dynamic_size_type<1,dynamic_size>( align )
        {
          assert( is_power_of_two(align) && "Alignment must be a power of two!" );
        }

        aligned_block_allocator_base( aligned_block_allocator_base&& ) noexcept = default;
        aligned_block_allocator_base( const aligned_block_allocator_base& ) = delete;
        aligned_block_allocator_base& operator=( aligned_block_allocator_base&& ) = delete;
        aligned_block_allocator_base& operator=( const aligned_block_allocator_base& ) = delete;
      };

    } // namespace detail

    //////////////////////////////////////////////////////////////////////////
    /// \brief A raw allocator that allocates over-aligned memory
    ///
    /// \tparam Size The size of the block
    /// \tparam Align The alignment of block
    ///
    /// \satisfies BlockAllocator
    /// \satisfies Stateless
    //////////////////////////////////////////////////////////////////////////
    template<std::size_t Size,std::size_t Align>
    class aligned_block_allocator
        : private detail::aligned_block_allocator_base<Size,Align>
    {
      using base_type = detail::aligned_block_allocator_base<Size,Align>;
      using block_size_member  = detail::dynamic_size_type<0,Size>;
      using block_align_member = detail::dynamic_size_type<1,Align>;

      static_assert( is_power_of_two(Align) || Align == dynamic_size, "Alignment must be a power of two!" );

      //----------------------------------------------------------------------
      // Constructors / Allocators
      //----------------------------------------------------------------------
    public:

      /// \brief Constructs a aligned_block_allocator that will distribute
      ///        blocks of the specified \p size
      aligned_block_allocator() = default;

      // Inherit the constructors
      using base_type::base_type;

      /// \brief Move-constructs an aligned_block_allocator from another allocator
      ///
      /// \param other the other aligned_block_allocator to move
      aligned_block_allocator( aligned_block_allocator&& other ) = default;

      /// \brief Copy-constructs an aligned_block_allocator from another allocator
      ///
      /// \param other the other aligned_block_allocator to copy
      aligned_block_allocator( const aligned_block_allocator& other ) = default;

      //-----------------------------------------------------------------------

      /// \brief Move-assigns an aligned_block_allocator from another allocator
      ///
      /// \param other the other aligned_block_allocator to move
      /// \return reference to \c (*this)
      aligned_block_allocator& operator=( aligned_block_allocator&& other ) = default;

      /// \brief Copy-assigns an aligned_block_allocator from another allocator
      ///
      /// \param other the other aligned_block_allocator to copy
      /// \return reference to \c (*this)
      aligned_block_allocator& operator=( const aligned_block_allocator& other ) = default;

      //----------------------------------------------------------------------
      // Block Allocations
      //----------------------------------------------------------------------
    public:

      /// \brief Allocates a memory_block of size \ref block_size with
      ///        alignment \p Align
      ///
      /// \return a newly allocated memory_block
      owner<memory_block> allocate_block() noexcept;

      /// \brief Deallocates a given memory_block
      ///
      /// \param block the block to deallocate
      void deallocate_block( owner<memory_block> block ) noexcept;

    };

    //-------------------------------------------------------------------------
    // Utilities
    //-------------------------------------------------------------------------

    /// \brief A cached version of aligned_block_allocator
    ///
    /// \tparam Size The size of the block
    /// \tparam Align The alignment of block
    template<std::size_t Size,std::size_t Align>
    using cached_aligned_block_allocator = cached_block_allocator<aligned_block_allocator<Size,Align>>;

    using dynamic_aligned_block_allocator = aligned_block_allocator<dynamic_size,dynamic_size>;

    using cached_dynamic_aligned_block_allocator = cached_block_allocator<dynamic_aligned_block_allocator>;

    //-------------------------------------------------------------------------

    template<std::size_t Size,std::size_t Align>
    using named_aligned_block_allocator = detail::named_block_allocator<aligned_block_allocator<Size,Align>>;

    template<std::size_t Size,std::size_t Align>
    using named_cached_aligned_block_allocator = detail::named_block_allocator<cached_aligned_block_allocator<Size,Align>>;

    using named_dynamic_aligned_block_allocator = detail::named_block_allocator<dynamic_aligned_block_allocator>;

    using named_cached_dynamic_aligned_block_allocator = detail::named_block_allocator<cached_dynamic_aligned_block_allocator>;

  } // namespace memory
} // namespace bit

#include "detail/aligned_block_allocator.inl"

#endif /* BIT_MEMORY_BLOCK_ALLOCATORS_ALIGNED_BLOCK_ALLOCATOR_HPP */
