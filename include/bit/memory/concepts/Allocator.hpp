/**
 * \file Allocator.hpp
 *
 * \brief This header defines the concepts and type-traits relating to the
 *        Allocator concept
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */
#ifndef BIT_MEMORY_CONCEPTS_ALLOCATOR_HPP
#define BIT_MEMORY_CONCEPTS_ALLOCATOR_HPP

#include "detail/identity.hpp"            // detail::identity
#include "detail/void_t.hpp"              // detail::void_t
#include "detail/allocator_utilities.hpp" // allocator_size_type

#include "../allocator_info.hpp" // allocator_info

#include <cstddef>     // std::maxalign_t
#include <type_traits> // std::declval

namespace bit {
  namespace memory {

#if __cplusplus >= 202000L
    template<typename T>
    concept Allocator = requires( T a,
                                  allocator_pointer_t<T> p,
                                  allocator_size_type_t<T> size,
                                  allocator_size_type_t<T> align )
    {
      { a.try_allocate( size, align ) } -> allocator_pointer_t<T>;
      { a.deallocate( p, size ) };
    };
#endif

    namespace detail {

      //----------------------------------------------------------------------
      // Allocator traits
      //----------------------------------------------------------------------

      template<typename T, typename = void>
      struct allocator_has_allocate_impl : std::false_type{};

      template<typename T>
      struct allocator_has_allocate_impl<T,
        void_t<decltype(std::declval<allocator_pointer_t<T>&>() = std::declval<T&>().allocate( std::declval<allocator_size_type_t<T>>(), std::declval<allocator_size_type_t<T>>() ) )>
      > : std::true_type{};

      //----------------------------------------------------------------------

      template<typename T, typename = void>
      struct allocator_has_try_allocate_impl : std::false_type{};

      template<typename T>
      struct allocator_has_try_allocate_impl<T,
        void_t<decltype(std::declval<allocator_pointer_t<T>&>() = std::declval<T&>().try_allocate( std::declval<allocator_size_type_t<T>>(), std::declval<allocator_size_type_t<T>>() ) )>
      > : std::true_type{};

      //----------------------------------------------------------------------

      template<typename T, typename = void>
      struct allocator_has_deallocate_impl : std::false_type{};

      template<typename T>
      struct allocator_has_deallocate_impl<T,
        void_t<decltype(std::declval<T&>().deallocate( std::declval<allocator_pointer_t<T>&>(), std::declval<allocator_size_type_t<T>>() ) )>
      > : std::true_type{};

      //----------------------------------------------------------------------

      template<typename T, typename = void>
      struct allocator_has_deallocate_all : std::false_type{};

      template<typename T>
      struct allocator_has_deallocate_all<T,
        void_t<decltype(std::declval<T&>().deallocate_all())>
      > : std::true_type{};

      //----------------------------------------------------------------------

      template<typename T, typename = void>
      struct allocator_has_owns_impl : std::false_type{};

      template<typename T>
      struct allocator_has_owns_impl<T,
        void_t<decltype(std::declval<bool&>() = std::declval<T&>().owns( std::declval<allocator_const_pointer_t<T>>() ))>
      > : std::true_type{};

      //----------------------------------------------------------------------

      template<typename T, typename = void>
      struct allocator_has_info_impl : std::false_type{};

      template<typename T>
      struct allocator_has_info_impl<T,
        void_t<decltype( std::declval<allocator_info>() = std::declval<const T&>().info() )>
      > : std::true_type{};

      //----------------------------------------------------------------------

      template<typename T, typename = void>
      struct allocator_has_max_size_impl : std::false_type{};

      template<typename T>
      struct allocator_has_max_size_impl<T,
        void_t<decltype( std::declval<allocator_size_type_t<T>&>() = std::declval<const T&>().max_size() )>
      > : std::true_type{};

      //----------------------------------------------------------------------

      template<typename T, typename = void>
      struct allocator_has_min_size_impl : std::false_type{};

      template<typename T>
      struct allocator_has_min_size_impl<T,
        void_t<decltype( std::declval<allocator_size_type_t<T>&>() = std::declval<const T&>().min_size() )>
      > : std::true_type{};

      //----------------------------------------------------------------------

      template<typename T, typename = void>
      struct allocator_is_always_equal_impl : std::false_type{};

      template<typename T>
      struct allocator_is_always_equal_impl<T,void_t<decltype(T::is_always_equal)>> : T::is_always_equal{};

      //----------------------------------------------------------------------

      template<typename T, typename = void>
      struct allocator_default_alignment_impl : std::integral_constant<allocator_size_type_t<T>,1>{};

      template<typename T>
      struct allocator_default_alignment_impl<T,void_t<decltype(T::default_alignment)>> : T::default_alignment{};

      //----------------------------------------------------------------------

      template<typename T, typename = void>
      struct allocator_max_alignment_impl : std::integral_constant<allocator_size_type_t<T>,alignof(std::max_align_t)>{};

      template<typename T>
      struct allocator_max_alignment_impl<T,void_t<decltype(T::max_alignment)>> : std::integral_constant<allocator_size_type_t<T>,alignof(std::max_align_t)>{};

      //----------------------------------------------------------------------

    } // namespace detail


    /// \brief Type-trait to determine whether \p T has an 'allocate' function
    ///
    /// The result is aliased as \c ::value
    ///
    /// \tparam T the type to check
    template<typename T>
    struct allocator_has_allocate
      : detail::allocator_has_allocate_impl<T>{};

    /// \brief Convenience template bool for accessing
    ///        \c allocator_has_allocate<T>::value
    ///
    /// \tparam T the type to check
    template<typename T>
    constexpr bool allocator_has_allocate_v = allocator_has_allocate<T>::value;

    //-------------------------------------------------------------------------

    /// \brief Type-trait to determine whether \p T has a 'deallocate_all'
    ///        function
    ///
    /// The result is aliased as \c ::value
    ///
    /// \tparam T the type to check
    template<typename T>
    struct allocator_can_truncate_deallocations
      : detail::allocator_has_deallocate_all<T>{};

    /// \brief Convenience template bool for accessing
    ///        \c allocator_can_truncate_deallocations<T>::value
    ///
    /// \tparam T the type to check
    template<typename T>
    constexpr bool allocator_can_truncate_deallocations_v = allocator_can_truncate_deallocations<T>::value;

    //-------------------------------------------------------------------------

    /// \brief Type-trait to determine whether \p T has an 'owns' function
    ///
    /// The result is aliased as \c ::value
    ///
    /// \tparam T the type to check
    template<typename T>
    struct allocator_knows_ownership
      : detail::allocator_has_owns_impl<T>{};

    /// \brief Convenience template bool for accessing
    ///        \c allocator_knows_ownership<T>::value
    ///
    /// \tparam T the type to check
    template<typename T>
    constexpr bool allocator_knows_ownership_v = allocator_knows_ownership<T>::value;

    //-------------------------------------------------------------------------

    /// \brief Type-trait to determine whether \p T has an 'info' function
    ///
    /// The result is aliased as \c ::value
    ///
    /// \tparam T the type to check
    template<typename T>
    struct allocator_has_info
      : detail::allocator_has_info_impl<T>{};

    /// \brief Convenience template bool for accessing
    ///        \c allocator_has_info<T>::value
    ///
    /// \tparam T the type to check
    template<typename T>
    constexpr bool allocator_has_info_v = allocator_has_info<T>::value;

    //-------------------------------------------------------------------------

    /// \brief Type-trait to determine whether \p T has a 'max_size' function
    ///
    /// The result is aliased as \c ::value
    ///
    /// \tparam T the type to check
    template<typename T>
    struct allocator_has_max_size
      : detail::allocator_has_max_size_impl<T>{};

    /// \brief Convenience template bool for accessing
    ///        \c allocator_has_max_size<T>::value
    ///
    /// \tparam T the type to check
    template<typename T>
    constexpr bool allocator_has_max_size_v = allocator_has_max_size<T>::value;

    //-------------------------------------------------------------------------

    /// \brief Type-trait to determine whether \p T has a 'min_size' function
    ///
    /// The result is aliased as \c ::value
    ///
    /// \tparam T the type to check
    template<typename T>
    struct allocator_has_min_size
      : detail::allocator_has_min_size_impl<T>{};

    /// \brief Convenience template bool for accessing
    ///        \c allocator_has_min_size<T>::value
    ///
    /// \tparam T the type to check
    template<typename T>
    constexpr bool allocator_has_min_size_v = allocator_has_min_size<T>::value;

    //-------------------------------------------------------------------------

    /// \brief Type-trait to determine whether all Allocators of type \p T
    ///        are equal
    ///
    /// The result is aliased as \c ::value
    ///
    /// \tparam T the type to check
    template<typename T>
    struct allocator_is_always_equal
      : detail::allocator_is_always_equal_impl<T>{};

    /// \brief Convenience template bool for accessing
    ///        \c allocator_is_always_equal<T>::value
    ///
    /// \tparam T the type to check
    template<typename T>
    constexpr bool allocator_is_always_equal_v = allocator_is_always_equal<T>::value;

    //-------------------------------------------------------------------------

    /// \brief Type-trait to determine the default alignment of the given \p T
    ///
    /// The result is aliased as \c ::value
    ///
    /// \tparam T the type to check
    template<typename T>
    struct allocator_default_alignment
      : detail::allocator_default_alignment_impl<T>{};

    /// \brief Convenience template variable for accessing
    ///        \c allocator_max_alignment<T>::value
    ///
    /// \tparam T the type to check
    template<typename T>
    constexpr allocator_size_type_t<T> allocator_default_alignment_v = allocator_default_alignment<T>::value;

    //-------------------------------------------------------------------------

    /// \brief Type-trait to determine the maximum alignment of the given \p T
    ///
    /// The result is aliased as \c ::value
    ///
    /// \tparam T the type to check
    template<typename T>
    struct allocator_max_alignment
      : detail::allocator_max_alignment_impl<T>{};

    /// \brief Convenience template variable for accessing
    ///        \c allocator_max_alignment<T>::value
    ///
    /// \tparam T the type to check
    template<typename T>
    constexpr allocator_size_type_t<T> allocator_max_alignment_v = allocator_max_alignment<T>::value;

    //-------------------------------------------------------------------------

    /// \brief Type-trait to determine whether \p T satisfies the minimum
    ///        requirements to be an \c Allocator
    ///
    /// The result is aliased as \c ::value
    ///
    /// \tparam T the type to check
    template<typename T>
    struct is_allocator : std::integral_constant<bool,
      detail::allocator_has_try_allocate_impl<T>::value &&
      detail::allocator_has_deallocate_impl<T>::value
    >{};

    /// \brief Convenience template bool for accessing
    ///        \c is_allocator<T>::value
    ///
    /// \tparam T the type to check
    template<typename T>
    constexpr bool is_allocator_v = is_allocator<T>::value;

  } // namespace memory
} // namespace bit

#endif /* BIT_MEMORY_CONCEPTS_ALLOCATOR_HPP */
