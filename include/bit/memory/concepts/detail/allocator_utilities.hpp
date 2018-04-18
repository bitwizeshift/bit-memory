/*****************************************************************************
 * \file
 * \brief This internal header defines various utilities for extracting
 *        allocator-defined types (used for pretty-pointers)
 *
 * \note This is an internal header file, included by other library headers.
 *       Do not attempt to use it directly.
 *****************************************************************************/

/*
  The MIT License (MIT)

  Copyright (c) 2018 Matthew Rodusek

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/
#ifndef BIT_MEMORY_CONCEPTS_DETAIL_ALLOCATOR_UTILITIES_HPP
#define BIT_MEMORY_CONCEPTS_DETAIL_ALLOCATOR_UTILITIES_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

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
