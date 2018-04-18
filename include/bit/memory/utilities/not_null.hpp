/*****************************************************************************
 * \file
 * \brief This header contains a utility for guaranteeing non-null pointers
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
#ifndef BIT_MEMORY_UTILITIES_NOT_NULL_HPP
#define BIT_MEMORY_UTILITIES_NOT_NULL_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <cassert>     // assert
#include <cstddef>     // std::nullptr_t
#include <type_traits> // std::is_assignable
#include <utility>     // std::forward

namespace bit {
  namespace memory {

    //////////////////////////////////////////////////////////////////////////
    /// \brief A wrapper type around a pointer to disallow null assignments
    ///
    /// \tparam Ptr the underlying pointer type
    //////////////////////////////////////////////////////////////////////////
    template<typename Ptr>
    class not_null
    {

      static_assert(std::is_assignable<Ptr&, std::nullptr_t>::value, "Ptr cannot be assigned nullptr.");

      //----------------------------------------------------------------------
      // Public Member Types
      //----------------------------------------------------------------------
    public:

      using pointer      = Ptr;
      using reference    = decltype(*std::declval<Ptr>())&;
      using element_type = std::decay_t<reference>;

      //----------------------------------------------------------------------
      // Constructor / Assignment
      //----------------------------------------------------------------------
    public:

      /// Deleted nullptr assignment
      not_null( std::nullptr_t ) = delete;

      /// \brief Constructs a not_null from any type convertible to the pointer
      ///        type
      ///
      /// \param u the type to instantiate the not_null from
      template<typename U, std::enable_if_t<std::is_convertible<U, Ptr>::value>* = nullptr>
      constexpr not_null(U&& u);

      /// \brief Copy-constructs a not_null from another one
      ///
      /// \param other the other not_null to copy
      template<typename U, std::enable_if_t<std::is_convertible<U, Ptr>::value>* = nullptr>
      constexpr not_null(const not_null<U>& other);

      /// \brief Move-constructs a not_null from another one
      ///
      /// \param other the other not_null to move
      template<typename U, std::enable_if_t<std::is_convertible<U, Ptr>::value>* = nullptr>
      constexpr not_null(not_null<U>&& other) noexcept;

      /// \brief Default copy-construction
      ///
      /// \param other the other not_null to copy
      not_null(const not_null& other) = default;

      //----------------------------------------------------------------------

      /// Disallow assignment to nullptr
      not_null& operator=(std::nullptr_t) = delete;

      /// \brief Default copy-assignment
      ///
      /// \param other the other not_null to copy
      /// \return reference to \c (*this)
      not_null& operator=(const not_null& other) = default;

      //----------------------------------------------------------------------
      // Observers
      //----------------------------------------------------------------------
    public:

      /// \brief Operator to get the underlying pointer
      ///
      /// \return the underlying pointer
      constexpr pointer get() const noexcept;

      /// \brief Implicitly converts this to the underying pointer type
      constexpr operator Ptr() const noexcept;

      /// \brief Dereferences the underlying pointer
      ///
      /// \return the underlying pointer
      constexpr pointer operator->() const noexcept;

      /// \brief Dereferences the underlying pointer
      ///
      /// \return reference to the underlying pointer
      constexpr reference operator*() const noexcept;

      //----------------------------------------------------------------------
      // Deleted Operators
      //----------------------------------------------------------------------
    private:

      not_null& operator++() = delete;
      not_null& operator--() = delete;
      not_null operator++(int) = delete;
      not_null operator--(int) = delete;
      not_null& operator+=(std::ptrdiff_t) = delete;
      not_null& operator-=(std::ptrdiff_t) = delete;
      void operator[](std::ptrdiff_t) const = delete;

      //----------------------------------------------------------------------
      // Private Members
      //----------------------------------------------------------------------
    private:

      pointer m_pointer;
    };

    template<typename Ptr>
    constexpr not_null<Ptr> make_not_null(Ptr&& ptr);

  } // namespace memory
} // namespace bit

#include "detail/not_null.inl"

#endif /* BIT_MEMORY_UTILITIES_NOT_NULL_HPP */
