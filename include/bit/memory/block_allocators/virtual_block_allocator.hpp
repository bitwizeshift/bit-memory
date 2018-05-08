/*****************************************************************************
 * \file
 * \brief This header contains an implementation of a virtual-block allocator.
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
#ifndef BIT_MEMORY_BLOCK_ALLOCATORS_VIRTUAL_BLOCK_ALLOCATOR_HPP
#define BIT_MEMORY_BLOCK_ALLOCATORS_VIRTUAL_BLOCK_ALLOCATOR_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "detail/named_block_allocator.hpp" // detail::named_block_allocator

#include "../regions/virtual_memory.hpp"

#include "../utilities/ebo_storage.hpp"        // ebo_storage
#include "../utilities/dynamic_size_type.hpp"  // dynamic_size, etc
#include "../utilities/allocator_info.hpp"     // allocator_info
#include "../utilities/memory_block_cache.hpp" // memory_block_cache
#include "../utilities/owner.hpp"              // owner

#include "../policies/growth_multipliers/no_growth.hpp" // no_growth

#include <cstddef> // std::size_t
#include <type_traits> // std::enable_if

namespace bit {
  namespace memory {

    ///////////////////////////////////////////////////////////////////////////
    /// \brief This block allocator is used for creating virtual memory blocks
    ///
    /// This allocator reserves virtual memory pages up front, and commits
    /// them as they get requested. Any blocks that get deleted are simply
    /// cached for later use, rather than being decommitted each time.
    ///
    /// \satisfies{BlockAllocator}
    ///////////////////////////////////////////////////////////////////////////
    template<std::size_t Pages, typename GrowthMultiplier=no_growth_multiplier>
    class virtual_block_allocator
      : ebo_storage<GrowthMultiplier>,
        dynamic_size_type<0,Pages>
    {
      using base_type    = ebo_storage<GrowthMultiplier>;
      using pages_member = dynamic_size_type<0,Pages>;

      //-----------------------------------------------------------------------
      // Constructors
      //-----------------------------------------------------------------------
    public:

      /// \brief Constructs a virtual_block_allocator that reserves the
      ///        specified number of \p pages up front
      ///
      /// \param growth the growth multiplier
      template<std::size_t UPages=Pages,
               typename=std::enable_if_t<UPages!=dynamic_size>>
      explicit virtual_block_allocator( GrowthMultiplier growth = GrowthMultiplier{} );

      /// \brief Constructs a virtual_block_allocator that reserves the
      ///        specified number of \p pages up front
      ///
      /// \param pages the number of pages to reserve
      /// \param growth the growth multiplier
      template<std::size_t UPages=Pages,
               typename=std::enable_if_t<UPages==dynamic_size>>
      explicit virtual_block_allocator( std::size_t pages,
                                        GrowthMultiplier growth = GrowthMultiplier{} );

      /// \brief Move-constructs a virtual_block_allocator from another one
      ///
      /// \param other the other virtual_block_allocator to move
      virtual_block_allocator( virtual_block_allocator&& other ) noexcept;

      // Deleted copy constructor
      virtual_block_allocator( const virtual_block_allocator& other ) = delete;

      //-----------------------------------------------------------------------

      /// \brief Destructs this virtual_block_allocator, decommitting any
      ///        commit pages
      ~virtual_block_allocator();

      //-----------------------------------------------------------------------

      // Deleted move-assignment
      virtual_block_allocator& operator=( virtual_block_allocator&& other ) = delete;

      // Deleted copy-assignment
      virtual_block_allocator& operator=( const virtual_block_allocator& other ) = delete;

      //-----------------------------------------------------------------------
      // Block Allocations
      //-----------------------------------------------------------------------
    public:

      /// \brief Allocates a memory_block of size block_size
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
      /// This defaults to 'virtual_block_allocator'.
      /// Use a named_virtual_block_allocator to override this
      ///
      /// \return the info for this allocator
      allocator_info info() const noexcept;

      //-----------------------------------------------------------------------
      // Private Members
      //-----------------------------------------------------------------------
    private:

      void*              m_memory;      ///< The virtual memory to access from
      std::ptrdiff_t     m_active_page; ///< The currently active page
      memory_block_cache m_cache;       ///< Cache of already committed pages
    };

    //-------------------------------------------------------------------------
    // Utilities
    //-------------------------------------------------------------------------

    template<std::size_t Pages,
             typename GrowthMultiplier=no_growth_multiplier>
    using named_virtual_block_allocator = detail::named_block_allocator<virtual_block_allocator<Pages,GrowthMultiplier>>;

  } // namespace memory
} // namespace bit

#include "detail/virtual_block_allocator.inl"

#endif /* BIT_MEMORY_BLOCK_ALLOCATORS_VIRTUAL_BLOCK_ALLOCATOR_HPP */
