/*****************************************************************************
 * \file
 * \brief TODO: Add description
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

#ifndef BIT_MEMORY_UTILITIES_ADDRESS_HPP
#define BIT_MEMORY_UTILITIES_ADDRESS_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <cstdint>
#include <type_traits>

namespace bit {
  namespace memory {

    //=========================================================================
    // address
    //=========================================================================

    ///////////////////////////////////////////////////////////////////////////
    /// \brief An integral type representing a memory address.
    ///
    /// Addresses are comparable, and have restricted arithmetic operators
    /// to only support:
    ///
    /// - addition of integers
    /// - subtraction of integers
    /// - bitwise operations of integers or other addresses
    ///////////////////////////////////////////////////////////////////////////
    enum class address : std::uintptr_t{};

    //-------------------------------------------------------------------------
    // Compound Operators
    //-------------------------------------------------------------------------

    template<typename IntT,
             typename = std::enable_if_t<std::is_integral<IntT>::value>>
    address& operator+=( address& lhs, IntT rhs ) noexcept;
    template<typename IntT,
             typename = std::enable_if_t<std::is_integral<IntT>::value>>
    address& operator-=( address& lhs, IntT rhs ) noexcept;

    //-------------------------------------------------------------------------

    address& operator&=( address& lhs, address rhs ) noexcept;
    template<typename IntT,
             typename = std::enable_if_t<std::is_integral<IntT>::value>>
    address& operator&=( address& lhs, IntT rhs ) noexcept;

    address& operator|=( address& lhs, address rhs ) noexcept;
    template<typename IntT,
             typename = std::enable_if_t<std::is_integral<IntT>::value>>
    address& operator|=( address& lhs, IntT rhs ) noexcept;

    address& operator^=( address& lhs, address rhs ) noexcept;
    template<typename IntT,
             typename = std::enable_if_t<std::is_integral<IntT>::value>>
    address& operator^=( address& lhs, IntT rhs ) noexcept;

    //-------------------------------------------------------------------------
    // Binary Operators
    //-------------------------------------------------------------------------

    template<typename IntT,
             typename = std::enable_if_t<std::is_integral<IntT>::value>>
    constexpr address operator+( address lhs, IntT rhs ) noexcept;
    template<typename IntT,
             typename = std::enable_if_t<std::is_integral<IntT>::value>>
    constexpr address operator+( IntT lhs, address rhs ) noexcept;
    template<typename IntT,
             typename = std::enable_if_t<std::is_integral<IntT>::value>>
    constexpr address operator-( address lhs, IntT rhs ) noexcept;

    //-------------------------------------------------------------------------

    constexpr address operator~( address lhs ) noexcept;

    //-------------------------------------------------------------------------

    template<typename IntT,
             typename = std::enable_if_t<std::is_integral<IntT>::value>>
    constexpr address operator&( address lhs, IntT rhs ) noexcept;
    constexpr address operator&( address lhs, address rhs ) noexcept;

    template<typename IntT,
             typename = std::enable_if_t<std::is_integral<IntT>::value>>
    constexpr address operator|( address lhs, IntT rhs ) noexcept;
    constexpr address operator|( address lhs, address rhs ) noexcept;

    template<typename IntT,
             typename = std::enable_if_t<std::is_integral<IntT>::value>>
    constexpr address operator^( address lhs, IntT rhs ) noexcept;
    constexpr address operator^( address lhs, address rhs ) noexcept;

    //=========================================================================
    // X.Y.2 : enum class const_address
    //=========================================================================

    ///////////////////////////////////////////////////////////////////////////
    /// \brief An integral type representing a constant memory address.
    ///
    /// Like address, const_address objects are comparable, and have
    /// restricted arithmetic operators to only support:
    ///
    /// - addition of integers,
    /// - subtraction of integers, and
    /// - bitwise operations of integers or other addresses.
    ///
    /// const_address exists independent of address to ensure that the two
    /// are unable to be used in expressions together, except when used for
    /// comparisons.
    ///
    /// This distinction allows for better type safety when using addresses
    /// in expressions containing both const-qualified and const-unqualified
    /// objects.
    /// Otherwise,
    ///////////////////////////////////////////////////////////////////////////
    enum class const_address : std::uintptr_t{};

    //-------------------------------------------------------------------------
    // Compound Operators
    //-------------------------------------------------------------------------

    template<typename IntT,
             typename = std::enable_if_t<std::is_integral<IntT>::value>>
    const_address& operator+=( const_address& lhs, IntT rhs ) noexcept;
    template<typename IntT,
             typename = std::enable_if_t<std::is_integral<IntT>::value>>
    const_address& operator-=( const_address& lhs, IntT rhs ) noexcept;

    //-------------------------------------------------------------------------

    const_address& operator&=( const_address& lhs, const_address rhs ) noexcept;
    template<typename IntT,
             typename = std::enable_if_t<std::is_integral<IntT>::value>>
    const_address& operator&=( const_address& lhs, IntT rhs ) noexcept;

    const_address& operator|=( const_address& lhs, const_address rhs ) noexcept;
    template<typename IntT,
             typename = std::enable_if_t<std::is_integral<IntT>::value>>
    const_address& operator|=( const_address& lhs, IntT rhs ) noexcept;

    const_address& operator^=( const_address& lhs, const_address rhs ) noexcept;
    template<typename IntT,
             typename = std::enable_if_t<std::is_integral<IntT>::value>>
    const_address& operator^=( const_address& lhs, IntT rhs ) noexcept;

    //-------------------------------------------------------------------------
    // Binary Operators
    //-------------------------------------------------------------------------

    template<typename IntT,
             typename = std::enable_if_t<std::is_integral<IntT>::value>>
    constexpr const_address operator+( const_address lhs, IntT rhs ) noexcept;
    template<typename IntT,
             typename = std::enable_if_t<std::is_integral<IntT>::value>>
    constexpr const_address operator+( IntT lhs, const_address rhs ) noexcept;
    template<typename IntT,
             typename = std::enable_if_t<std::is_integral<IntT>::value>>
    constexpr const_address operator-( const_address lhs, IntT rhs ) noexcept;

    //-------------------------------------------------------------------------

    constexpr const_address operator~( const_address x ) noexcept;

    //-------------------------------------------------------------------------

    template<typename IntT,
             typename = std::enable_if_t<std::is_integral<IntT>::value>>
    constexpr const_address operator&( const_address lhs, IntT rhs ) noexcept;
    constexpr const_address operator&( const_address lhs,
                                       const_address rhs ) noexcept;

    template<typename IntT,
             typename = std::enable_if_t<std::is_integral<IntT>::value>>
    constexpr const_address operator|( const_address lhs, IntT rhs ) noexcept;
    constexpr const_address operator|( const_address lhs,
                                       const_address rhs ) noexcept;

    template<typename IntT,
             typename = std::enable_if_t<std::is_integral<IntT>::value>>
    constexpr const_address operator^( const_address lhs, IntT rhs ) noexcept;
    constexpr const_address operator^( const_address lhs,
                                       const_address rhs ) noexcept;

    //=========================================================================
    // X.Y.3 : address/const_address utilities
    //=========================================================================

    /// \{
    /// \brief Converts a pointer to an address
    ///
    /// \param p the pointer
    /// \return the converted address
    address to_address( void* p ) noexcept;
    const_address to_address( const void* p ) noexcept;
    /// \}

    /// \{
    /// \brief Converts an address to a pointer
    ///
    /// \param a the address
    /// \return the pointer for the address
    void* to_pointer( address a ) noexcept;
    const void* to_pointer( const_address a ) noexcept;
    /// \}

    /// \{
    /// \brief Converts an address into a pointer of type \p T
    ///
    /// \tparam T the type to convert the pointer into
    /// \param a the address
    /// \return the pointer for the address
    template<typename T>
    T* to_pointer( address a ) noexcept;
    template<typename T>
    const T* to_pointer( const_address a ) noexcept;
    /// \}

    //=========================================================================
    // X.Y.4 : address/const_address comparisons
    //=========================================================================

    constexpr bool operator==( address lhs, const_address rhs ) noexcept;
    constexpr bool operator==( const_address lhs, address rhs ) noexcept;
    constexpr bool operator!=( address lhs, const_address rhs ) noexcept;
    constexpr bool operator!=( const_address lhs, address rhs ) noexcept;
    constexpr bool operator<( address lhs, const_address rhs ) noexcept;
    constexpr bool operator<( const_address lhs, address rhs ) noexcept;
    constexpr bool operator>( address lhs, const_address rhs ) noexcept;
    constexpr bool operator>( const_address lhs, address rhs ) noexcept;
    constexpr bool operator<=( address lhs, const_address rhs ) noexcept;
    constexpr bool operator<=( const_address lhs, address rhs ) noexcept;
    constexpr bool operator>=( address lhs, const_address rhs ) noexcept;
    constexpr bool operator>=( const_address lhs, address rhs ) noexcept;

  } // namespace memory
} // namespace bit

#include "detail/address.inl"

#endif /* BIT_MEMORY_UTILITIES_ADDRESS_HPP */
