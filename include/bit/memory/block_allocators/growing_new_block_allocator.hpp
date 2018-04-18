/*****************************************************************************
 * \file
 * \brief This header contains a block allocator that increasingly grows the
 *        block size until reaching a final threshhold
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
#ifndef BIT_MEMORY_BLOCK_ALLOCATORS_GROWING_NEW_BLOCK_ALLOCATOR_HPP
#define BIT_MEMORY_BLOCK_ALLOCATORS_GROWING_NEW_BLOCK_ALLOCATOR_HPP

#include "detail/cached_block_allocator.hpp" // detail::cached_block_allocator
#include "detail/named_block_allocator.hpp"  // detail::named_block_allocator

#include "../utilities/dynamic_size_type.hpp" // dynamic_size, etc
#include "../utilities/allocator_info.hpp"    // allocator_info
#include "../utilities/macros.hpp"            // BIT_MEMORY_UNLIKELY
#include "../utilities/memory_block.hpp"      // memory_block
#include "../utilities/owner.hpp"             // owner

#include <cstddef>     // std::size_t, std::ptrdiff_t
#include <new>         // std::malloc, std::free
#include <type_traits> // std::true_type, std::false_type, etc

namespace bit {
  namespace memory {
    namespace detail {

      template<std::size_t Size>
      struct growing_new_block_allocator_base
        : dynamic_size_type<0,Size>
      {
        using is_stateless = std::true_type;

        growing_new_block_allocator_base( std::size_t growths ) noexcept
          : m_growths_remaining(growths),
            m_multiplier(1)
        {

        }

        growing_new_block_allocator_base( growing_new_block_allocator_base&& ) noexcept = default;
        growing_new_block_allocator_base( const growing_new_block_allocator_base& ) noexcept = default;
        growing_new_block_allocator_base& operator=( growing_new_block_allocator_base&& ) noexcept = default;
        growing_new_block_allocator_base& operator=( const growing_new_block_allocator_base& ) noexcept = default;

        std::size_t m_growths_remaining;
        std::size_t m_multiplier;
      };

      template<>
      struct growing_new_block_allocator_base<dynamic_size>
        : dynamic_size_type<0,dynamic_size>
      {
        using is_stateless = std::false_type;

        explicit growing_new_block_allocator_base( std::size_t size,
                                                      std::size_t growths )
          : dynamic_size_type<0,dynamic_size>(size),
            m_growths_remaining(growths),
            m_multiplier(1)
        {

        }

        growing_new_block_allocator_base( growing_new_block_allocator_base&& ) noexcept = default;
        growing_new_block_allocator_base( const growing_new_block_allocator_base& ) = delete;
        growing_new_block_allocator_base& operator=( growing_new_block_allocator_base&& ) = delete;
        growing_new_block_allocator_base& operator=( const growing_new_block_allocator_base& ) = delete;

        std::size_t m_growths_remaining;
        std::size_t m_multiplier;
      };

    } // namespace detail

    //////////////////////////////////////////////////////////////////////////
    /// \brief A block allocator that wraps around raw calls to std::malloc
    ///        and std::free, where the block allocations double in size
    ///        until it reaches a final threshhold.
    ///
    /// The threshhold can be specified as a constructor argument, which
    /// indicates the amount of times the block allocator may grow before
    /// reaching the final size
    ///
    /// \satisfies{BlockAllocator}
    //////////////////////////////////////////////////////////////////////////
    template<std::size_t Size>
    class growing_new_block_allocator
      : protected detail::growing_new_block_allocator_base<Size>
    {
      using block_size_member = dynamic_size_type<0,Size>;
      using base_type = detail::growing_new_block_allocator_base<Size>;

      //----------------------------------------------------------------------
      // Public Member Types
      //----------------------------------------------------------------------
    public:

      using default_block_alignment = std::integral_constant<std::size_t,alignof(std::max_align_t)>;

      //----------------------------------------------------------------------
      // Constructors
      //----------------------------------------------------------------------
    public:

      // Inherit the dynamic constructor
      using base_type::base_type;

      /// \brief Move-constructs a growing_new_block_allocator from another allocator
      ///
      /// \param other the other growing_new_block_allocator to move
      growing_new_block_allocator( growing_new_block_allocator&& other ) = default;

      // Deleted copy constructor
      growing_new_block_allocator( const growing_new_block_allocator& other ) = delete;

      //-----------------------------------------------------------------------

      // Deleted move assignment
      growing_new_block_allocator& operator=( growing_new_block_allocator&& other ) = delete;

      // Deleted copy assignment
      growing_new_block_allocator& operator=( const growing_new_block_allocator& other ) = delete;

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
      /// This defaults to 'growing_new_block_allocator'.
      /// Use a named_growing_new_block_allocator to override this
      ///
      /// \return the info for this allocator
      allocator_info info() const noexcept;

      //----------------------------------------------------------------------
      // Private Members
      //----------------------------------------------------------------------
    private:

      /// \brief Grows the size of each block, if possible
      void grow();

      template<std::size_t S>
      friend bool operator==( const growing_new_block_allocator<S>&,
                              const growing_new_block_allocator<S>& rhs ) noexcept;
    };

    //-------------------------------------------------------------------------
    // Equality
    //-------------------------------------------------------------------------

    template<std::size_t Size>
    bool operator==( const growing_new_block_allocator<Size>& lhs,
                     const growing_new_block_allocator<Size>& rhs ) noexcept;

    template<std::size_t Size>
    bool operator!=( const growing_new_block_allocator<Size>& lhs,
                     const growing_new_block_allocator<Size>& rhs ) noexcept;

    //-------------------------------------------------------------------------
    // Utilities
    //-------------------------------------------------------------------------

    using dynamic_growing_new_block_allocator = growing_new_block_allocator<dynamic_size>;

    template<std::size_t Size>
    using cached_growing_new_block_allocator = detail::cached_block_allocator<growing_new_block_allocator<Size>>;

    using cached_dynamic_growing_new_block_allocator = detail::cached_block_allocator<dynamic_growing_new_block_allocator>;

    //-------------------------------------------------------------------------

    template<std::size_t Size>
    using named_growing_new_block_allocator = detail::named_block_allocator<growing_new_block_allocator<Size>>;

    template<std::size_t Size>
    using named_cached_growing_new_block_allocator = detail::named_block_allocator<cached_growing_new_block_allocator<Size>>;

    using named_dynamic_growing_new_block_allocator = detail::named_block_allocator<dynamic_growing_new_block_allocator>;

    using named_cached_dynamic_growing_new_block_allocator = detail::named_block_allocator<cached_dynamic_growing_new_block_allocator>;

  } // namespace memory
} // namespace bit

#include "detail/growing_new_block_allocator.inl"

#endif /* BIT_MEMORY_BLOCK_ALLOCATORS_GROWING_NEW_BLOCK_ALLOCATOR_HPP */
