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

    ///////////////////////////////////////////////////////////////////////////
    /// \concept{Allocator}
    ///
    /// \brief This concept defines the required interface and semantics
    ///        expected of an allocator
    ///
    /// The \c bit::memory Allocator concept varies from the C++ standard's
    /// definition of the concept. One major difference is that an Allocator
    /// is not templated on the type. Instead, an allocator simply must
    /// satisfy the requirements of minimally supplying a non-throwing
    ///  \c try_allocate and \c deallocate function with a specific signature.
    ///
    /// The rationale for this follows HP and SGI's rationale for removing
    /// the template type; it's easier to be dealing with raw bytes from a
    /// system, and giving it the single responsibility of allocating the
    /// data.
    ///
    /// For type \c A to be \c Allocator, it must satisfy the following:
    ///
    /// **Provided**
    ///
    /// - \c A - an Allocator type
    /// - \c a - an instance of type \c A
    /// - \c s - the size of an allocation
    /// - \c n - the alignment of the allocation
    /// - \c p - a void pointer
    ///
    /// the following expressions must be well-formed with the expected
    /// side-effects:
    ///
    /// \code
    /// p = a.try_allocate( s, n )
    /// \endcode
    /// \c a tries to allocate at least \c s bytes aligned to the boundary
    /// \c n. This function returns \c nullptr on failure to allocate,
    /// otherwise returns a pointer to data at least \c s bytes large, aligned
    /// to a \c n byte boundary.
    ///
    /// The expression \code a.try_allocate( s, n ) \endcode must be
    /// non-throwing, otherwise it is undefined behaviour.
    ///
    /// - - - - -
    ///
    /// \code
    /// a.deallocate( v, s )
    /// \endcode
    /// Deallocates the memory pointer to by \c v with the allocation size
    /// \c s. The size must match the original size requested by
    /// \c a.try_allocate, otherwise it is undefined behaviour.
    /// Any use of \c v after a call to \c a.deallocate is undefined
    /// behaviour.
    ///
    /// - - - - -
    ///
    /// \code
    /// a1 == a2
    /// \endcode
    ///
    /// returns true only if the storage allocated by the allocator a1 can be
    /// deallocated through a2. Establishes reflexive, symmetric, and
    /// transitive relationship. Does not throw exceptions.
    ///
    /// - - - - -
    ///
    /// \code
    /// a1 != a2
    /// \endcode
    ///
    /// same as !(a1==a2)
    ///
    /// - - - - -
    ///
    /// **Optionally**
    ///
    /// \code
    /// v = a.allocate( s, n )
    /// \endcode
    /// \c a allocates at least \c s bytes aligned to the boundary \c n.
    ///
    /// The behaviour of this function is implementation-defined on failure
    /// (may throw, may invoke out-of-memory handler, etc).
    ///
    /// The default for this is to invoke out-of-memory handler on \c nullptr
    /// if an implementation is not provided
    ///
    /// - - - - -
    ///
    /// \code
    /// a.deallocate_all()
    /// \endcode
    /// \c a deallocates all entries inside of the allocator. Any existing
    /// pointers are now invalidated
    ///
    /// Default asserts.
    ///
    /// - - - - -
    ///
    /// \code
    /// a.info()
    /// \endcode
    /// \c a returns an allocator_info object describing the allocator
    ///
    /// The default value is implementation-defined, but should somehow identify
    /// the allocator
    ///
    /// - - - - -
    ///
    /// \code
    /// bool b = a.owns( p );
    /// \endcode
    ///
    /// \c a checks whether it owns the pointer \c p, returning the result.
    ///
    /// - - - - -
    ///
    /// \code
    /// a.max_size()
    /// \endcode
    ///
    /// Returns the maximum size the allocator can support
    ///
    /// Default is \c std::numeric_limits<std::size_t>::max()
    ///
    /// - - - - -
    ///
    /// \code
    /// a.min_size()
    /// \endcode
    ///
    /// Returns the minimum size the allocator can support
    ///
    /// Default is \c 1
    ///
    /// - - - - -
    ///
    /// \code
    /// A::default_alignment::value
    /// \endcode
    /// Determines the default-alignment of allocations from the given
    /// allocator. This allows certain optimizations to be made by making
    /// assumptions about the allocated results (such as avoiding the need
    /// of storing padding-offset for alignment, if it's always guaranteed
    /// to align to the correct boundary).
    ///
    /// Default is \c 1.
    ///
    /// - - - - -
    ///
    /// \code
    /// A::max_alignment::value
    /// \endcode
    /// Determines the maximum-alignment for allocations from the given
    /// allocator. Any specified alignment argument that exceeds this value
    /// results in undefined behaviour.
    ///
    /// Default is \c alignof(std::max_align_t)
    ///////////////////////////////////////////////////////////////////////////
#if __cplusplus >= 202000L
    // TODO(bitwize) replace 202000L with the correct __cplusplus when certified
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
