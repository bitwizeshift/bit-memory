/**
 * \file Stateless.hpp
 *
 * \brief This header contains the definitions for the 'Stateless' concepts and
 *        related type-traits
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */
#ifndef BIT_MEMORY_CONCEPTS_STATELESS_HPP
#define BIT_MEMORY_CONCEPTS_STATELESS_HPP

#include "detail/void_t.hpp" // detail::void_t

#include <type_traits> // std::integral_constants

namespace bit {
  namespace memory {

  // TODO(bitwize) replace 202000L with the correct __cplusplus when certified
#if __cplusplus >= 202000L

  template<typename T>
  concept bool Stateless = requires(T a, T b) {
      { a == b } -> bool;
      { a != b } -> bool;
  } && std::is_empty<T>::value
    && std::is_trivially_constructible<T>::value
    && std::is_trivially_destructible<T>::value
    && std::is_move_constructible<T>::value
    && std::is_copy_constructible<T>::value
    && std::is_move_assignable<T>::value
    && std::is_copy_assignable<T>::value;

#endif

    namespace detail {

      template<typename T, typename = void>
      struct is_equality_comparable : std::false_type{};

      template<typename T>
      struct is_equality_comparable<T,void_t<
        decltype(std::declval<bool&>() = (std::declval<T&>()==std::declval<T&>()))>
      > : std::true_type{};

      //-----------------------------------------------------------------------

      template<typename T, typename = void>
      struct is_inequality_comparable : std::false_type{};

      template<typename T>
      struct is_inequality_comparable<T,void_t<
        decltype(std::declval<bool&>() = (std::declval<T&>()!=std::declval<T&>()))>
      > : std::true_type{};

      //-----------------------------------------------------------------------

      template<typename T, typename = void>
      struct concept_is_stateless : std::false_type{};

      template<typename T>
      struct concept_is_stateless<T,void_t<decltype(T::is_stateless)>>
        : T::is_stateless{};

    } // namespace detail

    /// \brief Type-trait to check for whether a given type \p T is
    ///        \c Stateless
    ///
    /// The result is aliased as \c ::value
    ///
    /// \tparam T the type to check
    template<typename T>
    struct is_stateless : std::integral_constant<bool,
      detail::concept_is_stateless<T>::value &&
      detail::is_equality_comparable<T>::value &&
      detail::is_inequality_comparable<T>::value &&
      std::is_empty<T>::value &&
      std::is_trivially_constructible<T>::value &&
      std::is_trivially_destructible<T>::value &&
      std::is_move_constructible<T>::value &&
      std::is_copy_constructible<T>::value &&
      std::is_move_assignable<T>::value &&
      std::is_copy_assignable<T>::value
    >{};

    /// \brief Convenience template variable to extract the \c ::value member
    ///
    /// \tparam T the type to check
    template<typename T>
    constexpr bool is_stateless_v = is_stateless<T>::value;

  } // namespace memory
} // namespace bit

#endif /* BIT_MEMORY_CONCEPTS_STATELESS_HPP */
