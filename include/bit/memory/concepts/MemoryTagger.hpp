/*****************************************************************************
 * \file
 * \brief This header defines the concept for a MemoryTagger
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
#ifndef BIT_MEMORY_CONCEPTS_MEMORYTAGGER_HPP
#define BIT_MEMORY_CONCEPTS_MEMORYTAGGER_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "detail/void_t.hpp" // detail::void_t

#include <cstddef>     // std::size_t
#include <type_traits> // std::declval

namespace bit {
  namespace memory {

    ///////////////////////////////////////////////////////////////////////////
    /// \concept{MemoryTagger}
    ///
    /// \brief This concept defines a manner of tagging allocations for better
    ///        visibility in a debugger
    ///
    /// For type \c T to be \c MemoryTagger, it must satisfy the following:
    ///
    /// Provided
    ///
    /// - \c T - a \c MemoryTagger type
    /// - \c t - an instance of type \c T
    /// - \c s - the size of an allocation
    /// - \c p - a void pointer
    ///
    /// The following expressions must be well-formed and with the following
    /// side-effects:
    ///
    /// \code
    /// t.tag_allocation( p, s );
    /// \endcode
    ///
    /// \c T tags the allocation at address \c p with size \c s
    ///
    /// - - - - -
    ///
    /// \code
    /// t.tag_deallocation( p, s );
    /// \endcode
    ///
    /// \c T tags the deallocation at address \c p with size \c s
    ///////////////////////////////////////////////////////////////////////////
#if __cplusplus >= 202000L
    // TODO(bitwize) replace 202000L with the correct __cplusplus when certified

    template<typename T>
    concept bool MemoryTagger = requires( T t,
                                          void* p,
                                          std::size_t s )
    {
      { t.tag_allocation( p, s ) } -> void;
      { t.tag_deallocation( p, s ) } -> void;
    };
#endif

    namespace detail {

      template<typename T, typename = void>
      struct memory_tagger_has_tag_allocation : std::false_type{};

      template<typename T>
      struct memory_tagger_has_tag_allocation<T,void_t<
        decltype( std::declval<T&>().tag_allocation( std::declval<void*&>(), std::declval<std::size_t>() ) )
      >> : std::true_type{};

      //-----------------------------------------------------------------------

      template<typename T, typename = void>
      struct memory_tagger_has_tag_deallocation : std::false_type{};

      template<typename T>
      struct memory_tagger_has_tag_deallocation<T,void_t<
        decltype( std::declval<T&>().tag_deallocation( std::declval<void*&>(), std::declval<std::size_t>() ) )
      >> : std::true_type{};

    } // namespace detail

    /// \brief Type-trait determining whether \p T is a \c MemoryTagger
    ///
    /// The result is aliased as \c ::value
    ///
    /// \tparam T the type to check
    template<typename T>
    struct is_memory_tagger : std::integral_constant<bool,
      detail::memory_tagger_has_tag_allocation<T>::value &&
      detail::memory_tagger_has_tag_deallocation<T>::value &&
      std::is_default_constructible<T>::value
    >{};

    /// \brief Convenience template variable for extracting the result from
    ///        \c is_memory_tracker<T>::value
    ///
    /// \tparam T the type to check
    template<typename T>
    constexpr bool is_memory_tagger_v = is_memory_tagger<T>::value;

  } // namespace memory
} // namespace bit

#endif /* BIT_MEMORY_CONCEPTS_MEMORYTAGGER_HPP */
