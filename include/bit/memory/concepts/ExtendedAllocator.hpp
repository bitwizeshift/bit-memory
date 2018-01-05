/**
 * \file ExtendedAllocator.hpp
 *
 * \brief This header defines the concepts for the ExtendedAllocator
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */
#ifndef BIT_MEMORY_CONCEPTS_EXTENDEDALLOCATOR_HPP
#define BIT_MEMORY_CONCEPTS_EXTENDEDALLOCATOR_HPP

#include "detail/allocator_utilities.hpp" // allocator_pointer_t
#include "detail/identity.hpp"            // detail::identity
#include "detail/void_t.hpp"              // detail::void_t

#include "Allocator.hpp" // Allocator

#include <type_traits> // std::integral_constants

namespace bit {
  namespace memory {

    ///////////////////////////////////////////////////////////////////////////
    /// \concept{ExtendedAllocator,Allocator}
    ///
    /// \brief This concept defines the required interface and semantics
    ///        expected of an extended allocator
    ///
    /// An \c ExtendedAllocator is also an \c Allocator that provides extended
    /// functionality for tracking ownership, and providing the ability to
    /// offset the allocations alignment.
    ///
    /// **Requirements**
    ///
    /// - \ref Allocator
    ///
    /// For type \c A to be \c ExtendedAllocator, it must satisfy the above
    /// conditions as well as the following:
    ///
    /// **Provided**
    ///
    /// - \c A - an Allocator type
    /// - \c a - an instance of type \c A
    /// - \c s - the size of an allocation
    /// - \c n - the alignment of the allocation
    /// - \c o - the offset for the alignment
    /// - \c v - a void pointer
    ///
    /// the following expressions must be well-formed with the expected
    /// side-effects:
    ///
    /// \code
    /// v = a.try_allocate( s, n, o )
    /// \endcode
    ///
    /// \c a tries to allocate at least \c s bytes aligned to the boundary
    /// \c n, offset by \c o bytes.
    ///
    /// The expression \code a.try_allocate( s, n ) \endcode must be
    /// non-throwing, otherwise it is undefined behaviour.
    ///////////////////////////////////////////////////////////////////////////
#if __cplusplus >= 202000L
    // TODO(bitwize) replace 202000L with the correct __cplusplus when certified

    template<typename T>
    concept ExtendedAllocator = requires( T a,
                                          allocator_size_type_t<T> size,
                                          allocator_size_type_t<T> align,
                                          allocator_size_type_t<T> offset )
    {
      { a.try_allocate( size, align, offset ) } -> allocator_pointer_t<T>;
    } &&
    Allocator<T>;
#endif

    namespace detail {

      template<typename T, typename = void>
      struct allocator_has_extended_try_allocate_impl : std::false_type{};

      template<typename T>
      struct allocator_has_extended_try_allocate_impl<T,
        void_t<decltype(std::declval<allocator_pointer_t<T>&>()
          = std::declval<T&>().try_allocate( std::declval<allocator_size_type_t<T>>(),
                                             std::declval<allocator_size_type_t<T>>(),
                                             std::declval<allocator_size_type_t<T>>() ))
        >
      > : std::true_type{};

      //-----------------------------------------------------------------------

      template<typename T, typename = void>
      struct allocator_has_extended_try_allocate_hint_impl : std::false_type{};

      template<typename T>
      struct allocator_has_extended_try_allocate_hint_impl<T,
        void_t<decltype(std::declval<allocator_pointer_t<T>&>()
          = std::declval<T&>().try_allocate( std::declval<allocator_const_pointer_t<T>>(),
                                             std::declval<allocator_size_type_t<T>>(),
                                             std::declval<allocator_size_type_t<T>>(),
                                             std::declval<allocator_size_type_t<T>>() ))
        >
      > : std::true_type{};


      //-----------------------------------------------------------------------

      template<typename T, typename = void>
      struct allocator_has_extended_allocate_impl : std::false_type{};

      template<typename T>
      struct allocator_has_extended_allocate_impl<T,
        void_t<decltype(std::declval<allocator_pointer_t<T>&>()
          = std::declval<T&>().allocate( std::declval<allocator_size_type_t<T>>(),
                                         std::declval<allocator_size_type_t<T>>(),
                                         std::declval<allocator_size_type_t<T>>() ))
        >
      > : std::true_type{};

      //-----------------------------------------------------------------------

      template<typename T, typename = void>
      struct allocator_has_extended_allocate_hint_impl : std::false_type{};

      template<typename T>
      struct allocator_has_extended_allocate_hint_impl<T,
        void_t<decltype(std::declval<allocator_pointer_t<T>&>()
          = std::declval<T&>().allocate( std::declval<allocator_const_pointer_t<T>>(),
                                         std::declval<allocator_size_type_t<T>>(),
                                         std::declval<allocator_size_type_t<T>>(),
                                         std::declval<allocator_size_type_t<T>>() ))
         >
       > : std::true_type{};

      //-----------------------------------------------------------------------

      template<typename T, typename = void>
      struct allocator_has_expand : std::false_type{};

      template<typename T>
      struct allocator_has_expand<T,
        void_t<decltype(std::declval<bool&>()
          = std::declval<T&>().expand( std::declval<allocator_pointer_t<T>&>(),
                                       std::declval<allocator_size_type_t<T>>() ))
        >
      > : std::true_type{};

    } // namespace detail

    /// \brief Type trait to determine whether the allocator has the extended
    ///        allocate function
    ///
    /// The result is aliased as \c ::value
    ///
    /// \tparam T the type to check
    template<typename T>
    struct allocator_has_extended_allocate
      : detail::allocator_has_extended_allocate_impl<T>{};

    /// \brief Convenience template bool for accessing
    ///        \c allocator_has_extended_allocate<T>::value
    ///
    /// \tparam T the type to check
    template<typename T>
    constexpr bool allocator_has_extended_allocate_v = allocator_has_extended_allocate<T>::value;

    //-------------------------------------------------------------------------

    /// \brief Type trait to determine whether the allocator has the extended
    ///        try allocate function with a hint
    ///
    /// The result is aliased as \c ::value
    ///
    /// \tparam T the type to check
    template<typename T>
    struct allocator_has_extended_try_allocate_hint
      : detail::allocator_has_extended_try_allocate_hint_impl<T>{};

    /// \brief Convenience template bool for accessing
    ///        \c allocator_has_extended_try_allocate_hint<T>::value
    ///
    /// \tparam T the type to check
    template<typename T>
    constexpr bool allocator_has_extended_try_allocate_hint_v
      = allocator_has_extended_try_allocate_hint<T>::value;

    //-------------------------------------------------------------------------

    /// \brief Type trait to determine whether the allocator has the extended
    ///        allocate function with a hint
    ///
    /// The result is aliased as \c ::value
    ///
    /// \tparam T the type to check
    template<typename T>
    struct allocator_has_extended_allocate_hint
      : detail::allocator_has_extended_allocate_hint_impl<T>{};

    /// \brief Convenience template bool for accessing
    ///        \c allocator_has_extended_allocate_hint<T>::value
    ///
    /// \tparam T the type to check
    template<typename T>
    constexpr bool allocator_has_extended_allocate_hint_v
      = allocator_has_extended_allocate_hint<T>::value;

    //-------------------------------------------------------------------------

    /// \brief Type trait to determine whether the allocator has the expand
    ///        function
    ///
    /// The result is aliased as \c ::value
    ///
    /// \tparam T the type to check
    template<typename T>
    struct allocator_has_expand
      : detail::allocator_has_expand<T>{};

    /// \brief Convenience template bool for accessing
    ///        \c allocator_has_expand<T>::value
    ///
    /// \tparam T the type to check
    template<typename T>
    constexpr bool allocator_has_expand_v = allocator_has_expand<T>::value;

    //-------------------------------------------------------------------------

    /// \brief Type trait to determine whether \p T is an ExtendedAllocator
    ///
    /// The result is aliased as \c ::value
    ///
    /// \tparam T the type to check
    template<typename T>
    struct is_extended_allocator : std::integral_constant<bool,
      is_allocator<T>::value &&
      detail::allocator_has_extended_try_allocate_impl<T>::value
    >{};

    /// \brief Convenience template bool for accessing
    ///        \c is_extended_allocator<T>::value
    ///
    /// \tparam T the type to check
    template<typename T>
    constexpr bool is_extended_allocator_v = is_extended_allocator<T>::value;

  } // namespace memory
} // namespace bit

#endif /* BIT_MEMORY_CONCEPTS_EXTENDEDALLOCATOR_HPP */
