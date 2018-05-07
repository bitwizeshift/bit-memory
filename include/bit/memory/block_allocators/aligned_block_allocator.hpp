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

#include "detail/enum_types.hpp"             // block_align_t, block_size_t
#include "detail/cached_block_allocator.hpp" // cached_block_allocator
#include "detail/named_block_allocator.hpp"  // detail::named_block_allocator

#include "../utilities/allocator_info.hpp"    // allocator_info
#include "../utilities/dynamic_size_type.hpp" // dynamic_size, etc
#include "../utilities/ebo_storage.hpp"       // ebo_storage
#include "../utilities/macros.hpp"            // BIT_MEMORY_UNLIKELY
#include "../utilities/memory_block.hpp"      // memory_block
#include "../utilities/owner.hpp"             // owner
#include "../utilities/pointer_utilities.hpp" // is_power_of_two

#include "../policies/growth_multipliers/no_growth.hpp"

// regions
#include "../regions/aligned_heap_memory.hpp" // aligned_malloc, aligned_free

#include <type_traits> // std::true_type, std::false_type, etc
#include <cstddef>     // std::size_t, std::max_align_t
#include <cassert>     // assert

namespace bit {
  namespace memory {

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
    template<std::size_t DefaultBlockSize,
             std::size_t BlockAlign,
             typename GrowthMultiplier=no_growth_multiplier>
    class aligned_block_allocator
        : ebo_storage<GrowthMultiplier>,
          dynamic_size_type<0,DefaultBlockSize>,
          dynamic_size_type<1,BlockAlign>
    {
      using base_type          = ebo_storage<GrowthMultiplier>;
      using block_size_member  = dynamic_size_type<0,DefaultBlockSize>;
      using block_align_member = dynamic_size_type<1,BlockAlign>;

      static_assert( is_power_of_two(BlockAlign) || BlockAlign == dynamic_size,
                     "Alignment must be a power of two!" );

      //-----------------------------------------------------------------------
      // Constructors / Allocators
      //-----------------------------------------------------------------------
    public:

      /// \brief Constructs a aligned_block_allocator that will distribute
      ///        blocks of the specified \p size
      aligned_block_allocator() = default;

      /// \brief Constructs an aligned_block_allocator with the specified
      ///        \p growth policy
      ///
      /// \param growth the growth multiplier policy
      template<std::size_t USize=DefaultBlockSize,
               std::size_t UAlign=BlockAlign,
               typename=std::enable_if_t<(USize!=dynamic_block_size) &&
                                         (UAlign!=dynamic_block_alignment)>>
      explicit aligned_block_allocator( GrowthMultiplier growth );

      /// \brief Constructs an aligned_block_allocator with the specified
      ///        \p growth policy
      ///
      /// \param block_size the default block size
      /// \param growth the growth multiplier policy
      template<std::size_t USize=DefaultBlockSize,
               typename=std::enable_if_t<USize==dynamic_block_size>>
      explicit aligned_block_allocator( block_size_t block_size,
                                        GrowthMultiplier growth = GrowthMultiplier{} );

      /// \brief Constructs an aligned_block_allocator with the specified
      ///        \p growth policy
      ///
      /// \param block_alignment the default block alignment
      /// \param growth the growth multiplier policy
      template<std::size_t UAlign=BlockAlign,
               typename=std::enable_if_t<UAlign==dynamic_block_alignment>>
      explicit aligned_block_allocator( block_alignment_t block_alignment,
                                        GrowthMultiplier growth = GrowthMultiplier{}  );

      /// \brief Constructs an aligned_block_allocator with the specified
      ///        \p growth policy
      ///
      /// \param block_size the default block size
      /// \param block_alignment the default block alignment
      /// \param growth the growth multiplier policy
      template<std::size_t USize=DefaultBlockSize,
               std::size_t UAlign=BlockAlign,
               typename=std::enable_if_t<(USize==dynamic_block_size) &&
                                         (UAlign==dynamic_block_alignment)>>
      aligned_block_allocator( block_size_t block_size,
                               block_alignment_t block_alignment,
                               GrowthMultiplier growth = GrowthMultiplier{}  );

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
    template<std::size_t DefaultBlockSize,std::size_t BlockAlign,typename GrowthMultiplier=no_growth_multiplier>
    using cached_aligned_block_allocator
      = detail::cached_block_allocator<aligned_block_allocator<DefaultBlockSize,BlockAlign,GrowthMultiplier>>;

    //-------------------------------------------------------------------------

    template<std::size_t DefaultBlockSize,std::size_t BlockAlign,typename GrowthMultiplier=no_growth_multiplier>
    using named_aligned_block_allocator
      = detail::named_block_allocator<aligned_block_allocator<DefaultBlockSize,BlockAlign,GrowthMultiplier>>;

    template<std::size_t DefaultBlockSize,std::size_t BlockAlign,typename GrowthMultiplier=no_growth_multiplier>
    using named_cached_aligned_block_allocator
      = detail::named_block_allocator<cached_aligned_block_allocator<DefaultBlockSize,BlockAlign,GrowthMultiplier>>;

  } // namespace memory
} // namespace bit

#include "detail/aligned_block_allocator.inl"

#endif /* BIT_MEMORY_BLOCK_ALLOCATORS_ALIGNED_BLOCK_ALLOCATOR_HPP */
