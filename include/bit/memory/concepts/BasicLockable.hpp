/**
 * \file BasicLockable.hpp
 *
 * \brief This header defines the concept for a BasicLockable
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */
#ifndef BIT_MEMORY_CONCEPTS_BASICLOCKABLE_HPP
#define BIT_MEMORY_CONCEPTS_BASICLOCKABLE_HPP

#include "detail/void_t.hpp" // detail::void_t

#include <type_traits> // std::declval, std::true_type

namespace bit {
  namespace memory {

    ///////////////////////////////////////////////////////////////////////////
    /// \concept{BasicLockable}
    ///
    /// \brief The BasicLockable concept describes the minimal characteristics
    ///        of types that provide exclusive blocking semantics for execution
    ///        agents (i.e. threads).
    ///
    /// \note This concept is the same as the C++ standards definition.
    ///
    /// \see http://en.cppreference.com/w/cpp/concept/BasicLockable
    ///////////////////////////////////////////////////////////////////////////
#if __cplusplus >= 202000L
    // TODO(bitwize) replace 202000L with the correct __cplusplus when certified

    template<typename T>
    concept bool BasicLockable = requires( T l )
    {
      { l.lock() } -> void;
      { l.unlock() } -> void;
    };
#endif

    namespace detail {

      template<typename T, typename = void>
      struct basic_lockable_has_lock : std::false_type{};

      template<typename T>
      struct basic_lockable_has_lock<T,void_t<
        decltype( std::declval<T&>().lock() )
      >> : std::true_type{};

      //-----------------------------------------------------------------------

      template<typename T, typename = void>
      struct basic_lockable_has_unlock : std::false_type{};

      template<typename T>
      struct basic_lockable_has_unlock<T,void_t<
        decltype( std::declval<T&>().unlock() )
      >> : std::true_type{};

    } // namespace detail

    /// \brief Type trait to determine whether \c T is a BasicLockable
    ///
    /// The result is aliased as \c ::value
    ///
    /// \tparam T the type to check
    template<typename T>
    struct is_basic_lockable : std::integral_constant<bool,
      detail::basic_lockable_has_lock<T>::value &&
      detail::basic_lockable_has_unlock<T>::value
    >{};

    /// \brief Convenience bool to retrieve the value of
    ///        \c is_basic_lockable<T>::value
    ///
    /// \tparam T the type to check
    template<typename T>
    constexpr bool is_basic_lockable_v = is_basic_lockable<T>::value;

  } // namespace memory
} // namespace bit

#endif /* BIT_MEMORY_CONCEPTS_BASICLOCKABLE_HPP */
