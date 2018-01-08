/**
 * \file Deleter.hpp
 *
 * \brief TODO: Add description
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */
#ifndef BIT_MEMORY_CONCEPTS_DELETER_HPP
#define BIT_MEMORY_CONCEPTS_DELETER_HPP

#include "detail/void_t.hpp" // detail::void_t

#include <type_traits> // std::integral_constants

namespace bit {
  namespace memory {

    ///////////////////////////////////////////////////////////////////////////
    /// \concept{Deleter}
    ///
    /// \brief A structure that deallocates and destroys the underlying object.
    ///
    /// The only requirement is that a deallocator must be invocable with an
    /// instance of the pointer type being deallocated. The pointer should
    /// call the underlying type's destructor, and deallocate the memory for
    /// the specified type
    ///
    /// **Requirements**
    ///
    /// The following state may be specified to override the
    /// determination of a concept.
    ///
    /// **Provided**
    ///
    /// - \c D - a \c Deleter type
    /// - \c d - an instance of \c D
    /// - \c p - the pointer to be deleted (type \c D::pointer)
    ///
    /// the following expressions must be well-formed with the expected
    /// reproduceable side-effects:
    ///
    /// \code
    /// typename D::pointer
    /// \endcode
    ///
    /// The type of the pointer to be deleted
    ///
    /// - - - - -
    ///
    /// \code
    /// d( p );
    /// \code
    ///
    /// \c p's destructor is called, and the storage used for the resources
    /// are released.
    ///
    ///////////////////////////////////////////////////////////////////////////
#if __cplusplus >= 202000L
    // TODO(bitwize) replace 202000L with the correct __cplusplus when certified

    template<typename D>
    concept bool Deleter = requires(D d, typename D::pointer p) {
        { d(p) };
    };

#endif

    namespace detail {

      template<typename T, typename = void>
      struct is_deleter_impl : std::false_type{};

      template<typename T>
      struct is_deleter_impl<T,
        void_t<
          decltype(std::declval<T&>()( std::declval<typename T::pointer>() ) )
        >
      > : std::true_type{};

    } // namespace detail

    /// \brief Type-trait to check for whether a given type \p T is
    ///        a \c Deleter
    ///
    /// The result is aliased as \c ::value
    ///
    /// \tparam T the type to check
    template<typename T>
    struct is_deleter : detail::is_deleter_impl<T>{};

    /// \brief Convenience template variable to extract the \c ::value member
    ///        of is_deleter
    ///
    /// \tparam T the type to check
    template<typename T>
    constexpr bool is_deleter_v = is_deleter<T>::value;

  } // namespace memory
} // namespace bit

#endif /* BIT_MEMORY_CONCEPTS_DELETER_HPP */
