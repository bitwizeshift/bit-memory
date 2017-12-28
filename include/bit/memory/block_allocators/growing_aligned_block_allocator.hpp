/**
 * \file growing_aligned_block_allocator.hpp
 *
 * \brief This header contains an aligned block allocator that grows in size
 *        with each subsequent block allocation
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */
#ifndef BIT_MEMORY_BLOCK_ALLOCATORS_GROWING_ALIGNED_BLOCK_ALLOCATOR_HPP
#define BIT_MEMORY_BLOCK_ALLOCATORS_GROWING_ALIGNED_BLOCK_ALLOCATOR_HPP

#include "detail/cached_block_allocator.hpp" // cached_block_allocator
#include "detail/named_block_allocator.hpp"  // detail::named_block_allocator

#include "../detail/dynamic_size_type.hpp" // dynamic_size, detail::dynamic_size_type
#include "../aligned_memory.hpp"    // aligned_allocate
#include "../allocator_info.hpp"    // allocator_info
#include "../memory_block.hpp"      // memory_block
#include "../macros.hpp"            // BIT_MEMORY_UNLIKELY
#include "../owner.hpp"             // owner
#include "../pointer_utilities.hpp" // is_power_of_two

#include <type_traits> // std::true_type, std::false_type, etc
#include <cstddef>     // std::size_t
#include <cassert>     // assert

namespace bit {
  namespace memory {
    namespace detail {

      template<std::size_t Size, std::size_t Align>
      struct growing_aligned_block_allocator_base
        : detail::dynamic_size_type<0,Size>,
          detail::dynamic_size_type<1,Align>
      {
        using default_block_alignment = std::integral_constant<std::size_t,Align>;

        growing_aligned_block_allocator_base( std::size_t growths ) noexcept
          : m_growths_remaining(growths),
            m_multiplier(1)
        {

        }
        growing_aligned_block_allocator_base( growing_aligned_block_allocator_base&& ) noexcept = default;
        growing_aligned_block_allocator_base( const growing_aligned_block_allocator_base& ) = delete;
        growing_aligned_block_allocator_base& operator=( growing_aligned_block_allocator_base&& ) = delete;
        growing_aligned_block_allocator_base& operator=( const growing_aligned_block_allocator_base& ) = delete;

        std::size_t m_growths_remaining;
        std::size_t m_multiplier;
      };

      //-----------------------------------------------------------------------

      template<std::size_t Size>
      struct growing_aligned_block_allocator_base<Size,dynamic_size>
        : detail::dynamic_size_type<0,Size>,
          detail::dynamic_size_type<1,dynamic_size>
      {
        growing_aligned_block_allocator_base( std::size_t align,
                                              std::size_t growths ) noexcept
          : detail::dynamic_size_type<1,dynamic_size>( align ),
            m_growths_remaining(growths),
            m_multiplier(1)
        {

        }
        growing_aligned_block_allocator_base( growing_aligned_block_allocator_base&& ) noexcept = default;
        growing_aligned_block_allocator_base( const growing_aligned_block_allocator_base& ) = delete;
        growing_aligned_block_allocator_base& operator=( growing_aligned_block_allocator_base&& ) = delete;
        growing_aligned_block_allocator_base& operator=( const growing_aligned_block_allocator_base& ) = delete;

        std::size_t m_growths_remaining;
        std::size_t m_multiplier;
      };

      //-----------------------------------------------------------------------

      template<std::size_t Align>
      struct growing_aligned_block_allocator_base<dynamic_size,Align>
        : detail::dynamic_size_type<0,dynamic_size>,
          detail::dynamic_size_type<1,Align>
      {
        using default_block_alignment = std::integral_constant<std::size_t,Align>;

        growing_aligned_block_allocator_base( std::size_t size,
                                              std::size_t growths ) noexcept
          : detail::dynamic_size_type<0,dynamic_size>( size ),
            m_growths_remaining(growths),
            m_multiplier(1)
        {

        }
        growing_aligned_block_allocator_base( growing_aligned_block_allocator_base&& ) noexcept = default;
        growing_aligned_block_allocator_base( const growing_aligned_block_allocator_base& ) = delete;
        growing_aligned_block_allocator_base& operator=( growing_aligned_block_allocator_base&& ) = delete;
        growing_aligned_block_allocator_base& operator=( const growing_aligned_block_allocator_base& ) = delete;

        std::size_t m_growths_remaining;
        std::size_t m_multiplier;
      };

      //-----------------------------------------------------------------------

      template<>
      struct growing_aligned_block_allocator_base<dynamic_size,dynamic_size>
        : detail::dynamic_size_type<0,dynamic_size>,
          detail::dynamic_size_type<1,dynamic_size>
      {
        growing_aligned_block_allocator_base( std::size_t size,
                                              std::size_t align,
                                              std::size_t growths ) noexcept
          : detail::dynamic_size_type<0,dynamic_size>( size ),
            detail::dynamic_size_type<1,dynamic_size>( align ),
            m_growths_remaining(growths),
            m_multiplier(1)
        {
          assert( is_power_of_two(align) && "Alignment must be a power of two!" );
        }

        growing_aligned_block_allocator_base( growing_aligned_block_allocator_base&& ) noexcept = default;
        growing_aligned_block_allocator_base( const growing_aligned_block_allocator_base& ) = delete;
        growing_aligned_block_allocator_base& operator=( growing_aligned_block_allocator_base&& ) = delete;
        growing_aligned_block_allocator_base& operator=( const growing_aligned_block_allocator_base& ) = delete;

        std::size_t m_growths_remaining;
        std::size_t m_multiplier;
      };

    } // namespace detail

    ///////////////////////////////////////////////////////////////////////////
    /// \brief An allocator that allocates over-aligned memory that grows with
    ///        each allocation
    ///
    /// \tparam Size The size of the block
    /// \tparam Align The alignment of block
    ///
    /// \satisfies{BlockAllocator}
    ///////////////////////////////////////////////////////////////////////////
    template<std::size_t Size,std::size_t Align>
    class growing_aligned_block_allocator
        : private detail::growing_aligned_block_allocator_base<Size,Align>
    {
      using base_type = detail::growing_aligned_block_allocator_base<Size,Align>;
      using block_size_member  = detail::dynamic_size_type<0,Size>;
      using block_align_member = detail::dynamic_size_type<1,Align>;

      static_assert( is_power_of_two(Align) || Align == dynamic_size, "Alignment must be a power of two!" );

      //-----------------------------------------------------------------------
      // Constructors / Allocators
      //-----------------------------------------------------------------------
    public:

      // Inherit the constructors
      using base_type::base_type;

      /// \brief Move-constructs an growing_aligned_block_allocator from another allocator
      ///
      /// \param other the other growing_aligned_block_allocator to move
      growing_aligned_block_allocator( growing_aligned_block_allocator&& other ) = default;

      // Deleted copy constructor
      growing_aligned_block_allocator( const growing_aligned_block_allocator& other ) = delete;

      //-----------------------------------------------------------------------

      // Deleted copy assignment
      growing_aligned_block_allocator& operator=( growing_aligned_block_allocator&& other ) = delete;

      // Deleted move assignment
      growing_aligned_block_allocator& operator=( const growing_aligned_block_allocator& other ) = delete;

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
      /// This defaults to 'growing_aligned_block_allocator'.
      /// Use a named_growing_aligned_block_allocator to override this
      ///
      /// \return the info for this allocator
      allocator_info info() const noexcept;

      //-----------------------------------------------------------------------
      // Private Member Functions
      //-----------------------------------------------------------------------
    private:

      /// \brief Grows the size of each block, if possible
      void grow();
    };

    //-------------------------------------------------------------------------
    // Equality
    //-------------------------------------------------------------------------

    template<std::size_t Size, std::size_t Align>
    bool operator==( const growing_aligned_block_allocator<Size,Align>& lhs,
                     const growing_aligned_block_allocator<Size,Align>& rhs ) noexcept;

    template<std::size_t Size, std::size_t Align>
    bool operator!=( const growing_aligned_block_allocator<Size,Align>& lhs,
                     const growing_aligned_block_allocator<Size,Align>& rhs ) noexcept;

    //-------------------------------------------------------------------------
    // Utilities
    //-------------------------------------------------------------------------

    /// \brief A cached version of growing_aligned_block_allocator
    ///
    /// \tparam Size The size of the block
    /// \tparam Align The alignment of block
    template<std::size_t Size,std::size_t Align>
    using cached_growing_aligned_block_allocator = detail::cached_block_allocator<growing_aligned_block_allocator<Size,Align>>;

    using dynamic_growing_aligned_block_allocator = growing_aligned_block_allocator<dynamic_size,dynamic_size>;

    using cached_dynamic_growing_aligned_block_allocator = detail::cached_block_allocator<dynamic_growing_aligned_block_allocator>;

    //-------------------------------------------------------------------------

    template<std::size_t Size,std::size_t Align>
    using named_growing_aligned_block_allocator = detail::named_block_allocator<growing_aligned_block_allocator<Size,Align>>;

    template<std::size_t Size,std::size_t Align>
    using named_cached_growing_aligned_block_allocator = detail::named_block_allocator<cached_growing_aligned_block_allocator<Size,Align>>;

    using named_dynamic_growing_aligned_block_allocator = detail::named_block_allocator<dynamic_growing_aligned_block_allocator>;

    using named_cached_dynamic_growing_aligned_block_allocator = detail::named_block_allocator<cached_dynamic_growing_aligned_block_allocator>;

  } // namespace memory
} // namespace bit

#include "detail/growing_aligned_block_allocator.inl"

#endif /* BIT_MEMORY_BLOCK_ALLOCATORS_GROWING_ALIGNED_BLOCK_ALLOCATOR_HPP */
