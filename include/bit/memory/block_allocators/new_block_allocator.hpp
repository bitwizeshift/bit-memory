/*****************************************************************************
 * \file
 * \brief This header contains an API for allocating memory blocks that use
 *        ::operator new as the underlying allocation scheme.
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
#ifndef BIT_MEMORY_BLOCK_ALLOCATORS_NEW_BLOCK_ALLOCATOR_HPP
#define BIT_MEMORY_BLOCK_ALLOCATORS_NEW_BLOCK_ALLOCATOR_HPP

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

#include <new>         // ::new
#include <type_traits> // std::true_type, std::false_type, etc

namespace bit {
  namespace memory {
    namespace detail {

      template<std::size_t Size>
      struct new_block_allocator_base
        : dynamic_size_type<0,Size>
      {
        new_block_allocator_base() noexcept = default;
        new_block_allocator_base( new_block_allocator_base&& ) noexcept = default;
        new_block_allocator_base( const new_block_allocator_base& ) noexcept = default;
        new_block_allocator_base& operator=( new_block_allocator_base&& ) noexcept = default;
        new_block_allocator_base& operator=( const new_block_allocator_base& ) noexcept = default;
      };

      template<>
      struct new_block_allocator_base<dynamic_size>
        : dynamic_size_type<0,dynamic_size>
      {
        using is_stateless = std::false_type;

        explicit new_block_allocator_base( std::size_t size ) noexcept
          : dynamic_size_type<0,dynamic_size>( size )
        {

        }
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
    /// \satisfies{BlockAllocator}
    /// \satisfies{Stateless}
    //////////////////////////////////////////////////////////////////////////
    template<std::size_t Size>
    class new_block_allocator
      : private detail::new_block_allocator_base<Size>
    {
      using block_size_member = dynamic_size_type<0,Size>;
      using base_type = detail::new_block_allocator_base<Size>;

      //----------------------------------------------------------------------
      // Public Member Types
      //----------------------------------------------------------------------
    public:

      using default_block_alignment = std::integral_constant<std::size_t,alignof(std::max_align_t)>;

      //----------------------------------------------------------------------
      // Constructors
      //----------------------------------------------------------------------
    public:

      /// \brief Default-constructs this new_block_allocator
      ///
      /// This is only enabled for non-dynamic new_block_allocators
      new_block_allocator() noexcept = default;

      // Inherit the dynamic constructor
      using base_type::base_type;

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
      // Observers
      //----------------------------------------------------------------------
    public:

      /// \brief Queries the next block size expected from this allocator
      ///
      /// \return the size of the next allocated block
      std::size_t next_block_size() const noexcept;

      /// \brief Gets the info about this allocator
      ///
      /// This defaults to 'new_block_allocator'.
      /// Use a named_new_block_allocator to override this
      ///
      /// \return the info for this allocator
      allocator_info info() const noexcept;
    };

    //-------------------------------------------------------------------------
    // Equality
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
    using cached_new_block_allocator = detail::cached_block_allocator<new_block_allocator<Size>>;

    using dynamic_new_block_allocator = new_block_allocator<dynamic_size>;

    using cached_dynamic_new_block_allocator = detail::cached_block_allocator<dynamic_new_block_allocator>;

    //-------------------------------------------------------------------------

    template<std::size_t Size>
    using named_new_block_allocator = detail::named_block_allocator<new_block_allocator<Size>>;

    template<std::size_t Size>
    using named_cached_new_block_allocator = detail::named_block_allocator<cached_new_block_allocator<Size>>;

    using named_dynamic_new_block_allocator = detail::named_block_allocator<dynamic_new_block_allocator>;

    using named_cached_dynamic_new_block_allocator = detail::named_block_allocator<cached_dynamic_new_block_allocator>;

  } // namespace memory
} // namespace bit

#include "detail/new_block_allocator.inl"

#endif /* BIT_MEMORY_BLOCK_ALLOCATORS_NEW_BLOCK_ALLOCATOR_HPP */
