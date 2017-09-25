/**
 * \file block_allocator_traits.hpp
 *
 * \brief This header defines traits for block_allocators
 *
 * Currently, this library does not contain any optional features
 * that a block_allocator may define; but is defined and used for
 * ensuring forward-compatibility as things change
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */
#ifndef BIT_MEMORY_BLOCK_ALLOCATORS_BLOCK_ALLOCATOR_TRAITS_HPP
#define BIT_MEMORY_BLOCK_ALLOCATORS_BLOCK_ALLOCATOR_TRAITS_HPP

#include "../detail/void_t.hpp" // void_t
#include "../memory_block.hpp"  // memory_block

#include <type_traits> // std::true_type, std::false_type, etc
#include <cstddef>     // std::size_t, std::ptrdiff_t

namespace bit {
  namespace memory {
    namespace detail {

      template<typename T, typename = void>
      struct is_block_allocator : std::false_type{};

      template<typename T>
      struct is_block_allocator<T,void_t<
        decltype( std::declval<T&>().allocate_block() ),
        decltype( std::declval<T&>().deallocate_block(std::declval<memory_block>() ) )>
      > : std::true_type{};

    } // namespace detail

    /// \brief Type-trait to determine whether \p T is a block allocator
    ///
    /// The result is \c std::true_type if \p T satisfies the BlockAllocator
    /// concept, which minimally requires the following to be well formed:
    ///
    /// \code
    /// std::declval<T&>().allocate_block();
    /// std::declval<T&>().deallocate_block( std::declval<memory_block>() );
    /// \endcode
    ///
    /// The result is accessible as \c ::value
    template<typename T>
    using is_block_allocator = detail::is_block_allocator<T>;

    /// \brief Convenience template variable to extract whether \p T is a
    ///        block allocator
    template<typename T>
    constexpr bool is_block_allocator_v = is_block_allocator<T>::value;

    //////////////////////////////////////////////////////////////////////////
    /// \brief The block_allocator_traits class template provides a
    ///        standardized way to access block allocator functionality
    ///
    /// This will infer defaults for any optional values that may not be
    /// present in the allocator
    ///
    /// \note In the present version of this library, BlockAllocator does not
    ///       define any optional variations. This is here for forward-
    ///       compatibility.
    ///
    /// \tparam BlockAllocator the block allocator type. Must satisfy
    ///         BlockAllocator concept
    //////////////////////////////////////////////////////////////////////////
    template<typename BlockAllocator>
    class block_allocator_traits
    {
      //----------------------------------------------------------------------
      // Block Allocations
      //----------------------------------------------------------------------
    public:

      /// \brief Allocates a block from the underlying allocator
      ///
      /// \param alloc the allocator to allocate from
      /// \return the memory block
      static owner<memory_block> allocate_block( BlockAllocator& alloc );

      /// \brief Deallocates a block from the underlying container
      ///
      /// \param alloc the allocator to deallocate from
      /// \param block the block to deallocate
      static void deallocate_block( BlockAllocator& alloc,
                                    owner<memory_block> block );
    };

  } // namespace memory
} // namespace bit

#endif /* BIT_MEMORY_BLOCK_ALLOCATORS_BLOCK_ALLOCATOR_TRAITS_HPP */
