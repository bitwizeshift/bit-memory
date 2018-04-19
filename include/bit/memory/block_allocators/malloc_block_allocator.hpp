/*****************************************************************************
 * \file
 * \brief This header contains an implementation of a non-aligned, malloc
 *        block allocator
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
#ifndef BIT_MEMORY_BLOCK_ALLOCATORS_MALLOC_BLOCK_ALLOCATOR_HPP
#define BIT_MEMORY_BLOCK_ALLOCATORS_MALLOC_BLOCK_ALLOCATOR_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "detail/cached_block_allocator.hpp" // detail::cached_block_allocator
#include "detail/named_block_allocator.hpp"  // detail::named_block_allocator

#include "../utilities/dynamic_size_type.hpp" // dynamic_size, etc
#include "../utilities/allocator_info.hpp"    // allocator_info
#include "../utilities/macros.hpp"            // BIT_MEMORY_UNLIKELY
#include "../utilities/memory_block.hpp"      // memory_block
#include "../utilities/owner.hpp"             // owner

#include <cstddef>     // std::size_t, std::ptrdiff_t
#include <cstdlib>     // std::malloc, std::free
#include <type_traits> // std::true_type, std::false_type, etc

namespace bit {
  namespace memory {
    namespace detail {
      template<std::size_t Size>
      struct malloc_block_allocator_base
        : dynamic_size_type<0,Size>
      {
        malloc_block_allocator_base() noexcept = default;
        malloc_block_allocator_base( malloc_block_allocator_base&& ) noexcept = default;
        malloc_block_allocator_base( const malloc_block_allocator_base& ) noexcept = default;
        malloc_block_allocator_base& operator=( malloc_block_allocator_base&& ) noexcept = default;
        malloc_block_allocator_base& operator=( const malloc_block_allocator_base& ) noexcept = default;
      };

      template<>
      struct malloc_block_allocator_base<dynamic_size>
        : dynamic_size_type<0,dynamic_size>
      {
        explicit malloc_block_allocator_base( std::size_t size )
          : dynamic_size_type<0,dynamic_size>(size)
        {

        }

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
      : private detail::malloc_block_allocator_base<Size>
    {
      using block_size_member = dynamic_size_type<0,Size>;
      using base_type = detail::malloc_block_allocator_base<Size>;

      //----------------------------------------------------------------------
      // Public Member Types
      //----------------------------------------------------------------------
    public:

      using default_block_alignment = std::integral_constant<std::size_t,alignof(std::max_align_t)>;

      //----------------------------------------------------------------------
      // Constructors
      //----------------------------------------------------------------------
    public:

      /// \brief Default-constructs this malloc_block_allocator
      ///
      /// This is only enabled for non-dynamic new_block_allocators
      malloc_block_allocator() noexcept = default;

      // Inherit the dynamic constructor
      using base_type::base_type;

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
      // Observers
      //----------------------------------------------------------------------
    public:

      /// \brief Queries the next block size expected from this allocator
      ///
      /// \return the size of the next allocated block
      std::size_t next_block_size() const noexcept;

      /// \brief Gets the info about this allocator
      ///
      /// This defaults to 'malloc_block_allocator'.
      /// Use a named_malloc_block_allocator to override this
      ///
      /// \return the info for this allocator
      allocator_info info() const noexcept;
    };

    //-------------------------------------------------------------------------
    // Utiltiies
    //-------------------------------------------------------------------------

    using dynamic_malloc_block_allocator
      = malloc_block_allocator<dynamic_size>;

    template<std::size_t Size>
    using cached_malloc_block_allocator
      = detail::cached_block_allocator<malloc_block_allocator<Size>>;

    using cached_dynamic_malloc_block_allocator
      = detail::cached_block_allocator<dynamic_malloc_block_allocator>;

    //-------------------------------------------------------------------------

    template<std::size_t Size>
    using named_malloc_block_allocator
      = detail::named_block_allocator<malloc_block_allocator<Size>>;

    template<std::size_t Size>
    using named_cached_malloc_block_allocator
      = detail::named_block_allocator<cached_malloc_block_allocator<Size>>;

    using named_dynamic_malloc_block_allocator
      = detail::named_block_allocator<dynamic_malloc_block_allocator>;

    using named_cached_dynamic_malloc_block_allocator
      = detail::named_block_allocator<cached_dynamic_malloc_block_allocator>;

  } // namespace memory
} // namespace bit

#include "detail/malloc_block_allocator.inl"

#endif /* BIT_MEMORY_BLOCK_ALLOCATORS_MALLOC_BLOCK_ALLOCATOR_HPP */
