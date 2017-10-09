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

  ///////////////////////////////////////////////////////////////////////////////
  /// \concept{Stateless}
  ///
  /// \brief A structure that encompasses functionality, but contains no internal
  ///        nor external state (i.e. no data members).
  ///
  /// Two instances of the same type must always compare equal, and constructing/
  /// copying/moving instances must not affect or be affected by any external
  /// state.
  ///
  /// Semantically, a single instance should be identical in every respect to any
  /// duplicates; which may allow a composition of multiple of the same Stateless
  /// object to be compressed into a single instance (either EBO inheritance or
  /// data member).
  ///
  /// **Requirements**
  ///
  /// - DefaultConstructible
  /// - CopyConstructible
  /// - MoveConstructible
  /// - CopyAssignable
  /// - MoveASsignable
  /// - EqualityComparable
  ///
  /// Additionally, it must satisfy the followowing:
  ///
  /// **Provided**
  ///
  /// - \c S - a Stateless type
  /// - \c s - an instance of type \c S
  ///
  /// the following expressions must be well-formed with the expected
  /// reproduceable side-effects:
  ///
  /// \code
  /// S() == S()
  /// \endcode
  /// returns true
  ///
  /// - - - - -
  ///
  /// \code
  /// S() != S()
  /// \endcode
  /// returns false
  ///
  /// - - - - -
  ///
  /// \code
  /// S s1{};              // default ctor
  /// S s2(s);             // copy ctor
  /// S s3(std::move(s1)); // move ctor
  ///
  /// s1 = s2;             // copy assignment
  /// s2 = std::move(2);   // move assignment
  /// \code
  ///
  /// \code
  /// S::is_stateless
  /// \endcode
  /// Type must be std::true_type
  ///
  ///////////////////////////////////////////////////////////////////////////////
#if __cplusplus >= 202000L
    // TODO(bitwize) replace 202000L with the correct __cplusplus when certified

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
