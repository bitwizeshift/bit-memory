/**
 * \file BlockAllocator.hpp
 *
 * \brief This header contains the definitions for the 'BlockAllocator'
 *        concept and related type-traits
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */
#ifndef BIT_MEMORY_CONCEPTS_BLOCKALLOCATOR_HPP
#define BIT_MEMORY_CONCEPTS_BLOCKALLOCATOR_HPP

#include "detail/identity.hpp" // detail::identity
#include "detail/void_t.hpp"   // detail::void_t

#include "../allocator_info.hpp" // allocator_info
#include "../memory_block.hpp"   // memory_block

#include <type_traits> // std::integral_constants

namespace bit {
  namespace memory {

    ///////////////////////////////////////////////////////////////////////////
    /// \concept{BlockAllocator}
    ///
    /// \brief This concept defines the required interface and semantics
    ///        expected of a block allocator
    ///
    /// The \c bit::memory BlockAllocator concept is used to distribute
    /// memory blocks to allocators that can use them.
    ///
    /// BlockAllocators are also used as upstream resources for
    /// StreamingAllocators.
    ///
    /// The size of a memory_block returned by a BlockAllocator is fixed at
    /// construction time, and is not permitted to vary.
    ///
    /// Requirements
    ///
    /// - MoveConstructible
    ///
    /// For type \c A to be \c BlockAllocator, it must satisfy the above
    /// conditions as well as the following:
    ///
    /// Provided
    ///
    /// \c A - a BlockAllocator type
    /// \c a - an instance of type \c A
    /// \c b - a memory_block
    ///
    /// the following expressions must be well-formed with the expected
    /// side-effects:
    ///
    /// \code
    /// a.allocate_block()
    /// \endcode
    ///
    /// Allocates a \c memory_block of implementation-specific size
    ///
    /// \code
    /// a.deallocate_block( b );
    /// \endcode
    ///////////////////////////////////////////////////////////////////////////
#if __cplusplus >= 202000L
  // TODO(bitwize) replace 202000L with the correct __cplusplus when certified

    template<typename B>
    concept BlockAllocator = requires( B b, memory_block block )
    {
      { b.allocate_block() } -> memory_block;
      { b.deallocate_block( block ) } -> void;
    };
#endif

    namespace detail {

      template<typename T, typename = void>
      struct block_allocator_has_allocate_block_impl : std::false_type{};

      template<typename T>
      struct block_allocator_has_allocate_block_impl<T,void_t<
        decltype( std::declval<memory_block&>() = std::declval<T&>().allocate_block() )
      >> : std::true_type{};

      //-----------------------------------------------------------------------

      template<typename T, typename = void>
      struct block_allocator_has_deallocate_block_impl : std::false_type{};

      template<typename T>
      struct block_allocator_has_deallocate_block_impl<T,void_t<
        decltype( std::declval<T&>().deallocate_block( std::declval<memory_block&>() ) )
      >> : std::true_type{};

      //-----------------------------------------------------------------------

      template<typename T, typename = void>
      struct block_allocator_has_block_alignment_impl : std::false_type{};

      template<typename T>
      struct block_allocator_has_block_alignment_impl<T,void_t<
        decltype(T::block_alignment)>
      > : std::true_type{};

      //-----------------------------------------------------------------------

      template<typename T, typename = void>
      struct block_allocator_has_block_size_impl : std::false_type{};

      template<typename T>
      struct block_allocator_has_block_size_impl<T,void_t<
        decltype(T::block_size)>
      > : std::true_type{};

      //-----------------------------------------------------------------------

      template<typename T, typename = void>
      struct block_allocator_has_info_impl : std::false_type{};

      template<typename T>
      struct block_allocator_has_info_impl<T,
        void_t<decltype( std::declval<allocator_info>() = std::declval<const T&>().info() )>
      > : std::true_type{};

    } // namespace detail

    /// \brief Type-trait to determine whether \p T defines 'block_alignment'
    ///
    /// The result is aliased as \c ::value
    ///
    /// \tparam T the type to check
    template<typename T>
    struct block_allocator_has_block_alignment
      : detail::block_allocator_has_block_alignment_impl<T>{};

    /// \brief Convenience template to extract the result from
    ///        \c block_allocator_has_block_alignment<T>::value
    ///
    /// \tparam T the type to check
    template<typename T>
    constexpr std::size_t block_allocator_has_block_alignment_v = block_allocator_has_block_alignment<T>::value;

    //-------------------------------------------------------------------------

    /// \brief Type-trait to determine whether \p T defines 'block_size'
    ///
    /// The result is aliased as \c ::value
    ///
    /// \tparam T the type to check
    template<typename T>
    struct block_allocator_has_block_size
      : detail::block_allocator_has_block_size_impl<T>{};

    /// \brief Convenience template to extract the result from
    ///        \c block_allocator_has_block_size<T>::value
    ///
    /// \tparam T the type to check
    template<typename T>
    constexpr std::size_t block_allocator_has_block_size_v = block_allocator_has_block_size<T>::value;

    //-------------------------------------------------------------------------

    /// \brief Type-trait to determine whether \p T defines an 'info' function
    ///
    /// The result is aliased as \c ::value
    ///
    /// \tparam T the type to check
    template<typename T>
    struct block_allocator_has_info
      : detail::block_allocator_has_info_impl<T>{};

    /// \brief Convenience template to extract the result from
    ///        \c block_allocator_has_info<T>::value
    ///
    /// \tparam T the type to check
    template<typename T>
    constexpr bool block_allocator_has_info_v = block_allocator_has_info<T>::value;

    //-------------------------------------------------------------------------

    /// \brief Type-trait to determine whether \p T satisfies the minimum
    ///        requirements to be a \c BlockAllocator
    ///
    /// The result is aliased as \c ::value
    ///
    /// \tparam T the type to check
    template<typename T>
    struct is_block_allocator : std::integral_constant<bool,
      detail::block_allocator_has_allocate_block_impl<T>::value &&
      detail::block_allocator_has_deallocate_block_impl<T>::value
    >{};

    /// \brief Convenience template to extract the result from
    ///        \c is_block_allocator<T>::value
    ///
    /// \tparam T the type to check
    template<typename T>
    constexpr bool is_block_allocator_v = is_block_allocator<T>::value;

  } // namespace memory
} // namespace bit

#endif /* BIT_MEMORY_CONCEPTS_BLOCKALLOCATOR_HPP */
