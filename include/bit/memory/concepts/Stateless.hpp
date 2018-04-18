/*****************************************************************************
 * \file
 * \brief This header contains the definitions for the 'Stateless' concepts and
 *        related type-traits
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
#ifndef BIT_MEMORY_CONCEPTS_STATELESS_HPP
#define BIT_MEMORY_CONCEPTS_STATELESS_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "detail/void_t.hpp" // detail::void_t

#include <type_traits> // std::integral_constants

namespace bit {
  namespace memory {

    ///////////////////////////////////////////////////////////////////////////
    /// \concept{Stateless}
    ///
    /// \brief A structure that encompasses functionality, but contains no
    ///        internal nor external state (i.e. no data members).
    ///
    /// Two instances of the same type must always compare equal, and
    /// constructing/copying/moving instances must not affect or be affected by
    /// any external state.
    ///
    /// Semantically, a single instance should be identical in every respect to
    /// any duplicates; which may allow a composition of multiple of the same
    /// Stateless object to be compressed into a single instance (either EBO
    /// inheritance or data member).
    ///
    /// **Requirements**
    ///
    /// - Empty
    /// - TriviallyDefaultConstructible
    /// - TriviallyCopyConstructible
    /// - TriviallyMoveConstructible
    /// - TriviallyCopyAssignable
    /// - TriviallyMoveASsignable
    ///
    /// Alternatively, the following state may be specified to override the
    /// determination of a concept.
    ///
    /// **Provided**
    ///
    /// - \c S - a Stateless type
    /// - \c s - an instance of \c S
    ///
    /// the following expressions must be well-formed with the expected
    /// reproduceable side-effects:
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
    ///////////////////////////////////////////////////////////////////////////
#if __cplusplus >= 202000L
    // TODO(bitwize) replace 202000L with the correct __cplusplus when certified

    template<typename T>
    concept bool Stateless = std::is_empty<T>::value
      && std::is_trivially_constructible<T>::value
      && std::is_trivially_destructible<T>::value
      && std::is_trivially_move_constructible<T>::value
      && std::is_trivially_copy_constructible<T>::value
      && std::is_trivially_copy_assignable<T>::value
      && std::is_trivially_move_assignable<T>::value
      && std::is_trivial<T>::value

#endif

    /// \brief Type-trait to check for whether a given type \p T is
    ///        \c Stateless
    ///
    /// The result is aliased as \c ::value
    ///
    /// \tparam T the type to check
    template<typename T>
    struct is_stateless : std::integral_constant<bool,
      std::is_empty<T>::value &&
      std::is_trivially_constructible<T>::value &&
      std::is_trivially_destructible<T>::value &&
      std::is_trivially_move_constructible<T>::value &&
      std::is_trivially_copy_constructible<T>::value &&
      std::is_trivially_copy_assignable<T>::value &&
      std::is_trivially_move_assignable<T>::value &&
      std::is_trivial<T>::value
    >{};

    /// \brief Convenience template variable to extract the \c ::value member
    ///
    /// \tparam T the type to check
    template<typename T>
    constexpr bool is_stateless_v = is_stateless<T>::value;

  } // namespace memory
} // namespace bit

#endif /* BIT_MEMORY_CONCEPTS_STATELESS_HPP */
