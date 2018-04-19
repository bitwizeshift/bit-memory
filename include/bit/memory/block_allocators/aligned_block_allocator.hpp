/*****************************************************************************
 * \file
 * \brief This header contains the implementation of a block allocator that
 *        allocates aligned memory
 *****************************************************************************/

/*
  The MIT License (MIT)

  Copyright (c) 2018 Matthew Rodusek

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/
#ifndef BIT_MEMORY_BLOCK_ALLOCATORS_ALIGNED_BLOCK_ALLOCATOR_HPP
#define BIT_MEMORY_BLOCK_ALLOCATORS_ALIGNED_BLOCK_ALLOCATOR_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "detail/cached_block_allocator.hpp" // cached_block_allocator
#include "detail/named_block_allocator.hpp"  // detail::named_block_allocator

#include "../utilities/dynamic_size_type.hpp" // dynamic_size, etc
#include "../utilities/allocator_info.hpp"    // allocator_info
#include "../utilities/macros.hpp"            // BIT_MEMORY_UNLIKELY
#include "../utilities/memory_block.hpp"      // memory_block
#include "../utilities/owner.hpp"             // owner
#include "../utilities/pointer_utilities.hpp" // is_power_of_two

// regions
#include "../regions/aligned_heap_memory.hpp" // aligned_malloc, aligned_free

#include <type_traits> // std::true_type, std::false_type, etc
#include <cstddef>     // std::size_t, std::max_align_t
#include <cassert>     // assert

namespace bit {
  namespace memory {
    namespace detail {

      template<std::size_t Size, std::size_t Align>
      struct aligned_block_allocator_base
        : dynamic_size_type<0,Size>,
          dynamic_size_type<1,Align>
      {
        using default_block_alignment = std::integral_constant<std::size_t,Align>;

        aligned_block_allocator_base() noexcept = default;
        aligned_block_allocator_base( aligned_block_allocator_base&& ) noexcept = default;
        aligned_block_allocator_base( const aligned_block_allocator_base& ) noexcept = default;
        aligned_block_allocator_base& operator=( aligned_block_allocator_base&& ) noexcept = default;
        aligned_block_allocator_base& operator=( const aligned_block_allocator_base& ) noexcept = default;
      };

      //-----------------------------------------------------------------------

      template<std::size_t Size>
      struct aligned_block_allocator_base<Size,dynamic_size>
        : dynamic_size_type<0,Size>,
          dynamic_size_type<1,dynamic_size>
      {

        aligned_block_allocator_base( std::size_t align ) noexcept
          : dynamic_size_type<1,dynamic_size>( align )
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
        : dynamic_size_type<0,dynamic_size>,
          dynamic_size_type<1,Align>
      {
        using default_block_alignment = std::integral_constant<std::size_t,Align>;

        aligned_block_allocator_base( std::size_t size ) noexcept
          : dynamic_size_type<0,dynamic_size>( size )
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
        : dynamic_size_type<0,dynamic_size>,
          dynamic_size_type<1,dynamic_size>
      {
        aligned_block_allocator_base( std::size_t size, std::size_t align ) noexcept
          : dynamic_size_type<0,dynamic_size>( size ),
            dynamic_size_type<1,dynamic_size>( align )
        {
          assert( is_power_of_two(align) && "Alignment must be a power of two!" );
        }

        aligned_block_allocator_base( aligned_block_allocator_base&& ) noexcept = default;
        aligned_block_allocator_base( const aligned_block_allocator_base& ) = delete;
        aligned_block_allocator_base& operator=( aligned_block_allocator_base&& ) = delete;
        aligned_block_allocator_base& operator=( const aligned_block_allocator_base& ) = delete;
      };

    } // namespace detail

    ///////////////////////////////////////////////////////////////////////////
    /// \brief An allocator that allocates over-aligned memory
    ///
    /// \note This allocator should only be used for over-aligned memory blocks
    ///       where required. It does not make sense to be used if the required
    ///       alignment of each block is less than or equal to
    ///       \c alignof(std::max_align_t) -- as this fundamental alignment is
    ///       required of both the 'new_block_allocator' and
    ///       'malloc_block_allocator' without requiring any additional
    ///       overhead
    ///
    /// \tparam Size The size of the block
    /// \tparam Align The alignment of block
    ///
    /// \satisfies{BlockAllocator}
    /// \satisfies{Stateless}
    ///////////////////////////////////////////////////////////////////////////
    template<std::size_t Size,std::size_t Align>
    class aligned_block_allocator
        : private detail::aligned_block_allocator_base<Size,Align>
    {
      using base_type = detail::aligned_block_allocator_base<Size,Align>;
      using block_size_member  = dynamic_size_type<0,Size>;
      using block_align_member = dynamic_size_type<1,Align>;

      static_assert( is_power_of_two(Align) || Align == dynamic_size,
                     "Alignment must be a power of two!" );

      //-----------------------------------------------------------------------
      // Constructors / Allocators
      //-----------------------------------------------------------------------
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

      //-----------------------------------------------------------------------
      // Block Allocations
      //-----------------------------------------------------------------------
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

      //-----------------------------------------------------------------------
      // Observers
      //-----------------------------------------------------------------------
    public:

      /// \brief Queries the next block size expected from this allocator
      ///
      /// \return the size of the next allocated block
      std::size_t next_block_size() const noexcept;

      /// \brief Gets the info about this allocator
      ///
      /// This defaults to 'aligned_block_allocator'.
      /// Use a named_aligned_block_allocator to override this
      ///
      /// \return the info for this allocator
      allocator_info info() const noexcept;
    };

    //-------------------------------------------------------------------------
    // Utilities
    //-------------------------------------------------------------------------

    /// \brief A cached version of aligned_block_allocator
    ///
    /// \tparam Size The size of the block
    /// \tparam Align The alignment of block
    template<std::size_t Size,std::size_t Align>
    using cached_aligned_block_allocator
      = detail::cached_block_allocator<aligned_block_allocator<Size,Align>>;

    using dynamic_aligned_block_allocator
      = aligned_block_allocator<dynamic_size,dynamic_size>;

    using cached_dynamic_aligned_block_allocator
      = detail::cached_block_allocator<dynamic_aligned_block_allocator>;

    //-------------------------------------------------------------------------

    template<std::size_t Size,std::size_t Align>
    using named_aligned_block_allocator
      = detail::named_block_allocator<aligned_block_allocator<Size,Align>>;

    template<std::size_t Size,std::size_t Align>
    using named_cached_aligned_block_allocator
      = detail::named_block_allocator<cached_aligned_block_allocator<Size,Align>>;

    using named_dynamic_aligned_block_allocator
      = detail::named_block_allocator<dynamic_aligned_block_allocator>;

    using named_cached_dynamic_aligned_block_allocator
      = detail::named_block_allocator<cached_dynamic_aligned_block_allocator>;

  } // namespace memory
} // namespace bit

#include "detail/aligned_block_allocator.inl"

#endif /* BIT_MEMORY_BLOCK_ALLOCATORS_ALIGNED_BLOCK_ALLOCATOR_HPP */
