/**
 * \file allocator_utilities.hpp
 *
 * \brief TODO: Add description
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */
#ifndef BIT_MEMORY_CONCEPTS_DETAIL_ALLOCATOR_UTILITIES_HPP
#define BIT_MEMORY_CONCEPTS_DETAIL_ALLOCATOR_UTILITIES_HPP

#include "identity.hpp" // detail::identity
#include "void_t.hpp"   // detail::void_t

#include <cstddef> // std::size_t, std::ptrdiff_t

namespace bit {
  namespace memory {
    namespace detail {

      template<typename T, typename = void>
      struct allocator_size_type_impl : identity<std::size_t>{};

      template<typename T>
      struct allocator_size_type_impl<T,void_t<decltype(T::size_type)>> : identity<typename T::size_type>{};

      //-----------------------------------------------------------------------

      template<typename T, typename = void>
      struct allocator_pointer_impl : identity<void*>{};

      template<typename T>
      struct allocator_pointer_impl<T,void_t<decltype(T::pointer)>> : identity<typename T::pointer>{};

      //-----------------------------------------------------------------------

      template<typename T, typename = void>
      struct allocator_difference_type_impl : identity<std::ptrdiff_t>{};

      template<typename T>
      struct allocator_difference_type_impl<T,void_t<decltype(T::difference_type)>> : identity<typename T::difference_type>{};

      //-----------------------------------------------------------------------

      template<typename T, typename = void>
      struct allocator_const_pointer_impl : identity<const void*>{};

      template<typename T>
      struct allocator_const_pointer_impl<T,void_t<decltype(T::const_pointer)>> : identity<typename T::const_pointer>{};
    } // namespace detail

    //-------------------------------------------------------------------------

    /// \brief Type-trait to determine the size type of a given \p T
    ///
    /// The result is aliased as \c ::type
    ///
    /// \param T the type to check
    template<typename T>
    struct allocator_size_type : detail::allocator_size_type_impl<T>{};

    /// \brief Convenience alias to extract \c allocator_size_type<T>::type
    ///
    /// \tparam T the type to check
    template<typename T>
    using allocator_size_type_t = typename allocator_size_type<T>::type;

    //-------------------------------------------------------------------------

    /// \brief Type-trait to determine the pointer type of a given \p T
    ///
    /// The result is aliased as \c ::type
    ///
    /// \param T the type to check
    template<typename T>
    struct allocator_pointer : detail::allocator_pointer_impl<T>{};

    /// \brief Convenience alias to extract \c allocator_pointer<T>::type
    ///
    /// \tparam T the type to check
    template<typename T>
    using allocator_pointer_t = typename allocator_pointer<T>::type;

    //-------------------------------------------------------------------------

    /// \brief Type-trait to determine the const pointer type of a given \p T
    ///
    /// The result is aliased as \c ::type
    ///
    /// \param T the type to check
    template<typename T>
    struct allocator_const_pointer : detail::allocator_const_pointer_impl<T>{};

    /// \brief Convenience alias to extract \c allocator_const_pointer<T>::type
    ///
    /// \tparam T the type to check
    template<typename T>
    using allocator_const_pointer_t = typename allocator_const_pointer<T>::type;

    //-------------------------------------------------------------------------

    /// \brief Type-trait to determine the difference type of a given \p T
    ///
    /// The result is aliased as \c ::type
    ///
    /// \param T the type to check
    template<typename T>
    struct allocator_difference_type : detail::allocator_difference_type_impl<T>{};

    /// \brief Convenience alias to extract \c allocator_difference_type<T>::type
    ///
    /// \tparam T the type to check
    template<typename T>
    using allocator_difference_type_t = typename allocator_difference_type<T>::type;

  } // namespace memory
} // namespace bit

#endif /* BIT_MEMORY_CONCEPTS_DETAIL_ALLOCATOR_UTILITIES_HPP */
