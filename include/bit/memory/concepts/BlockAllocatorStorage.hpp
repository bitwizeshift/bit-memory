/*****************************************************************************
 * \file
 * \brief This header defines the concept of an 'BlockAllocatorStorage', which
 *        is used to contain block allocator types within composites.
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
#ifndef BIT_MEMORY_CONCEPTS_BLOCK_ALLOCATOR_STORAGE_HPP
#define BIT_MEMORY_CONCEPTS_BLOCK_ALLOCATOR_STORAGE_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "detail/void_t.hpp" // detail::void_t

#include "BlockAllocator.hpp" // is_block_allocator

#include <type_traits> // std::declval, std::true_type

namespace bit {
  namespace memory {

    ///////////////////////////////////////////////////////////////////////////
    /// \concept{BlockAllocatorStorage}
    ///
    /// \brief The BlockAllocatorStorage concept defines a generalized way of
    ///        storing block allocators within composite types
    ///
    /// The requirements are minimal -- only requiring a 'get_block_allocator'
    /// function that returns a reference to the underlying block allocator,
    /// which must itself satisfy the \ref BlockAllocator concept
    ///
    /// For type \c S to be \c BlockAllocatorStorage, it must satisfy the
    /// following:
    ///
    /// **Provided**
    ///
    /// - \c S  - an BlockAllocatorStorage type
    /// - \c s  - an instance of type \c S
    /// - \c cs - an instance of type \c const S
    /// - \c A  - the type of the underlying stored block allocator
    /// - \c a  - an instance of type \c A
    ///
    /// the following expressions must be well-formed with the expected
    /// side-effects:
    ///
    /// \code
    /// S::block_allocator_type
    /// \endcode
    ///
    /// Retrieves the underlying bloc kallocator type. block_allocator_type
    /// must satisfy the \ref BlockAllocator concept.
    ///
    /// - - - - -
    ///
    /// \code
    /// A& a = s.get_block_allocator();
    /// \endcode
    ///
    /// Retrieves a reference to the underlying stored block allocator.
    ///
    /// - - - - -
    ///
    /// \code
    /// const A& a = s.get_block_allocator();
    /// \endcode
    ///
    /// Retrieves a reference to the underlying stored block allocator.
    ///
    ///////////////////////////////////////////////////////////////////////////
#if __cplusplus >= 202000L
    // TODO(bitwize) replace 202000L with the correct __cplusplus when certified

    template<typename T>
    concept bool BlockAllocatorStorage = requires( T& s, const T& cs )
    {
      { s.get_block_allocator() } -> typename T::block_allocator_type&;
      { cs.get_block_allocator() } -> const typename T::block_allocator_type&;
    } && std::is_nothrow_copy_constructible<T>::value &&
         std::is_nothrow_move_constructible<T>::value &&
         std::is_nothrow_copy_assignable<T>::value &&
         std::is_nothrow_move_assignable<T>::value;
#endif

    namespace detail {

      template<typename T, typename = void>
      struct is_block_allocator_storage_impl : std::false_type{};

      template<typename T>
      struct is_block_allocator_storage_impl<T,
      void_t<
        decltype( std::declval<T&>().get_block_allocator() ),
        decltype( std::declval<const T&>().get_block_allocator() )
      >
    > : std::integral_constant<bool,
      is_block_allocator<typename T::block_allocator_type>::value &&
      std::is_convertible<decltype(std::declval<T&>().get_block_allocator()),
                          typename T::block_allocator_type&>::value &&
      std::is_convertible<decltype(std::declval<const T&>().get_block_allocator()),
                          const typename T::block_allocator_type&>::value &&
      std::is_nothrow_copy_constructible<T>::value &&
      std::is_nothrow_move_constructible<T>::value &&
      std::is_nothrow_copy_assignable<T>::value &&
      std::is_nothrow_move_assignable<T>::value>{};
    } // namespace detail

    /// \brief Type trait to determine whether \c T is a BlockAllocatorStorage
    ///
    /// The result is aliased as \c ::value
    ///
    /// \tparam T the type to check
    template<typename T>
    struct is_block_allocator_storage : detail::is_block_allocator_storage_impl<T>{};

    /// \brief Convenience bool to retrieve the value of
    ///        \c is_block_allocator_storage<T>::value
    ///
    /// \tparam T the type to check
    template<typename T>
    constexpr bool is_block_allocator_storage_v = is_block_allocator_storage<T>::value;

  } // namespace memory
} // namespace bit

#endif /* BIT_MEMORY_CONCEPTS_BLOCK_ALLOCATOR_STORAGE_HPP */
