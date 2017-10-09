/**
 * \file BoundsChecker.hpp
 *
 * \brief This header defines the concept for a BoundsChecker
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */
#ifndef BIT_MEMORY_CONCEPTS_BOUNDSCHECKER_HPP
#define BIT_MEMORY_CONCEPTS_BOUNDSCHECKER_HPP

#include "detail/void_t.hpp" // detail::void_t

#include "../allocator_info.hpp" // allocator_info

#include <cstddef>     // std::size_t
#include <type_traits> // std::declval

namespace bit {
  namespace memory {

    ///////////////////////////////////////////////////////////////////////////
    /// \concept{BoundsChecker}
    ///
    /// \brief This concept defines a manner of injecting and checking memory
    ///        fences around allocations
    ///
    /// For type \c B to be a \c BoundsChecker, it must satisfy the following:
    ///
    /// **Provided**
    ///
    /// - \c B - a \c BoundsChecker type
    /// - \c b - an instance of type \c B
    /// - \c s - the size of an allocation
    /// - \c p - a void pointer
    /// - \p i - an allocator_info object
    ///
    /// The following expressions must be well-formed and with the following
    /// side-effects:
    ///
    /// \code
    /// b.prepare_front_fence( p, s );
    /// \endcode
    ///
    /// Prepares a front-fence at the given pointer \c p with the size \c s
    ///
    /// - - - - -
    ///
    /// \code
    /// b.prepare_back_fence( p, s );
    /// \endcode
    ///
    /// Prepares a back-fence at the given pointer \c p with the size \c s
    ///
    /// - - - - -
    ///
    /// \code
    /// b.check_front_fence( i, p, s );
    /// \endcode
    ///
    /// Checks a front-fence at the given pointer \c p with the size \c s, for
    /// allocator \c i.
    ///
    /// - - - - -
    ///
    /// \code
    /// b.check_back_fence( i, p, s );
    /// \endcode
    ///
    /// Checks a back-fence at the given pointer \c p with the size \c s, for
    /// allocator \c i.
    ///
    /// - - - - -
    ///
    /// \code
    /// std::size_t s = B::front_size
    /// \endcode
    ///
    /// Convertible to \c std::size_t -- the number of bytes to prepend to an
    /// allocation for a front memory fence
    ///
    /// - - - - -
    ///
    /// \code
    /// std::size_t s = B::back_size
    /// \endcode
    ///
    /// Convertible to \c std::size_t -- the number of bytes to append to an
    /// allocation for a back memory fence
    ///////////////////////////////////////////////////////////////////////////
#if __cplusplus >= 202000L
    // TODO(bitwize) replace 202000L with the correct __cplusplus when certified

    template<typename T>
    concept bool MemoryTagger = requires( T b,
                                          void* p,
                                          std::size_t s,
                                          allocator_info i )
    {
      { b.prepare_front_fence( p, s ) } -> void;
      { b.prepare_back_fence( p, s ) } -> void;
      { b.check_front_fence( i, p, s ) } -> void;
      { b.check_back_fence( i, p, s ) } -> void;
      { B::front_size } -> std::size_t;
      { B::back_size } -> std::size_t;
    };

#endif

    namespace detail {

      template<typename T, typename = void>
      struct bounds_checker_has_prepare_front_fence : std::false_type{};

      template<typename T>
      struct bounds_checker_has_prepare_front_fence<T,void_t<
        decltype( std::declval<T&>().prepare_front_fence( std::declval<void*&>(), std::declval<std::size_t>() ) )
      >> : std::true_type{};

      //-----------------------------------------------------------------------

      template<typename T, typename = void>
      struct bounds_checker_has_prepare_back_fence : std::false_type{};

      template<typename T>
      struct bounds_checker_has_prepare_back_fence<T,void_t<
        decltype( std::declval<T&>().prepare_back_fence( std::declval<void*&>(), std::declval<std::size_t>() ) )
      >> : std::true_type{};

      //-----------------------------------------------------------------------

      template<typename T, typename = void>
      struct bounds_checker_has_check_front_fence : std::false_type{};

      template<typename T>
      struct bounds_checker_has_check_front_fence<T,void_t<
        decltype( std::declval<T&>().check_front_fence( std::declval<allocator_info>(), std::declval<void*&>(), std::declval<std::size_t>() ) )
      >> : std::true_type{};

      //-----------------------------------------------------------------------

      template<typename T, typename = void>
      struct bounds_checker_has_check_back_fence : std::false_type{};

      template<typename T>
      struct bounds_checker_has_check_back_fence<T,void_t<
        decltype( std::declval<T&>().check_back_fence( std::declval<allocator_info>(), std::declval<void*&>(), std::declval<std::size_t>() ) )
      >> : std::true_type{};

      //-----------------------------------------------------------------------

      template<typename T, typename = void>
      struct bounds_checker_has_front_size : std::false_type{};

      template<typename T>
      struct bounds_checker_has_front_size<T,void_t<
        decltype( std::declval<std::size_t&>() = T::front_size )
      >> : std::true_type{};

      //-----------------------------------------------------------------------

      template<typename T, typename = void>
      struct bounds_checker_has_back_size : std::false_type{};

      template<typename T>
      struct bounds_checker_has_back_size<T,void_t<
        decltype( std::declval<std::size_t&>() = T::back_size )
      >> : std::true_type{};

    } // namespace detail

    /// \brief Type-trait determining whether \p T is a \c BoundsChecker
    ///
    /// The result is aliased as \c ::value
    ///
    /// \tparam T the type to check
    template<typename T>
    struct is_bounds_checker : std::integral_constant<bool,
      detail::bounds_checker_has_prepare_front_fence<T>::value &&
      detail::bounds_checker_has_prepare_back_fence<T>::value &&
      detail::bounds_checker_has_check_front_fence<T>::value &&
      detail::bounds_checker_has_check_back_fence<T>::value &&
      detail::bounds_checker_has_front_size<T>::value &&
      detail::bounds_checker_has_back_size<T>::value
     >{};

    /// \brief Convenience template variable for extracting the result from
    ///        \c is_memory_tracker<T>::value
    ///
    /// \tparam T the type to check
    template<typename T>
    constexpr bool is_bounds_checker_v = is_bounds_checker<T>::value;

  } // namespace memory
} // namespace bit

#endif /* BIT_MEMORY_CONCEPTS_BOUNDSCHECKER_HPP */
