/*****************************************************************************
 * \file
 * \brief This header contains various utilities for converting between
 *        different endiannesses
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
#ifndef BIT_MEMORY_UTILITIES_ENDIAN_HPP
#define BIT_MEMORY_UTILITIES_ENDIAN_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <algorithm>   // std::reverse
#include <cstdint>     // std::int8_t, std::uint8_t, etc
#include <type_traits> // std::is_copy_constructible, std::enable_if_t, etc

#ifdef _MSC_VER
#include <intrin.h> // _byteswap_ushort, _byteswap_ulong, _byteswap_uint64
#endif

namespace bit {
  namespace memory {

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Enum representing the endianness of the current system
    ///
    ///////////////////////////////////////////////////////////////////////////
    enum class endian
    {
#if defined(__ORDER_LITTLE_ENDIAN__) && \
    defined(__ORDER_BIG_ENDIAN__) && \
    defined(__BYTE_ORDER__)

      little = __ORDER_LITTLE_ENDIAN__,//!< little
      big    = __ORDER_BIG_ENDIAN__,   //!< big
      native = __BYTE_ORDER__,         //!< native

#elif defined(_WIN32)
    little = 0,
    big    = 1,
    native = little,
#elif defined(BIT_MEMORY_ENDIANNESS)
    little = 0,
    bit = 1,
    native = BIT_MEMORY_ENDIANNESS,
#else
#error endian could not be determined. Please define 'BIT_MEMORY_ENDIANNESS' to either 'little' or 'big'
#endif
    };

    static_assert( endian::native == endian::little ||
                   endian::native == endian::big, "Only big or little endianness is supported");

    //-------------------------------------------------------------------------
    // Swapping
    //-------------------------------------------------------------------------

    /// \{
    /// \brief Swaps the endianness of the given integral value \p val
    ///
    /// \param val the value to endian-swap
    /// \return the integral value with swapped endian
    char endian_swap( char val ) noexcept;
    std::int8_t endian_swap( std::int8_t val ) noexcept;
    std::uint8_t endian_swap( std::uint8_t val ) noexcept;
    std::int16_t endian_swap( std::int16_t val ) noexcept;
    std::uint16_t endian_swap( std::uint16_t val ) noexcept;
    std::int32_t endian_swap( std::int32_t val ) noexcept;
    std::uint32_t endian_swap( std::uint32_t val ) noexcept;
    std::int64_t endian_swap( std::int64_t val ) noexcept;
    std::uint64_t endian_swap( std::uint64_t val ) noexcept;
    float endian_swap( float val ) noexcept;
    double endian_swap( double val ) noexcept;
    /// \}

    /// \brief Swaps the endianness of the arbitrary standard layout type T
    ///
    /// \note that a copy is made before returning the result, so the type must
    ///       be copy-constructible
    ///
    /// \param val the value to endian swap
    template<typename T, typename = std::enable_if_t<!std::is_fundamental<T>::value>>
    T endian_swap( T val );

    //-------------------------------------------------------------------------
    // Buffers
    //-------------------------------------------------------------------------

    /// \brief Endian-swaps a buffer in-place, returning the pointer
    ///
    /// \note The buffer must be of a character type
    ///
    /// \param p the pointer to the buffer to swap
    /// \param n the size of the buffer
    /// \return pointer to \p p
    template<typename CharT, typename = std::enable_if<sizeof(CharT)==1>>
    CharT* endian_swap_buffer_in_place( CharT* p, std::size_t n ) noexcept;
    void* endian_swap_in_place( std::nullptr_t, std::size_t ) = delete;

    /// \brief Endian-swaps an arbitrary type in-place
    ///
    /// \note Care should be taken to avoid endian-swapping structures that
    ///       contain padding. In general, this utility should mostly just be
    ///       used for writing custom endian-swaps for
    ///
    /// \param p pointer to the type to swap.
    /// \return pointer to the swapped instance
    template<typename T>
    T* endian_swap_in_place( T* p ) noexcept;
    void* endian_swap_in_place( void* p ) = delete;
    void* endian_swap_in_place( std::nullptr_t ) = delete;

    inline namespace casts {

      /// \brief Endian-casts the specified type
      ///
      /// \note This uses an unqualified call to 'endian_swap'. ADL can be used
      ///       by defining custom 'endian_swap' calls in the same namespace as
      ///       the type being endian swapped
      ///
      /// \tparam Endianness the endianness to swap to
      /// \param val the value to endian-swap
      /// \return the value casted to the specified endianness
      template<endian Endianness, typename T>
      T endian_cast( T val ) noexcept;

    } // inline namespace casts
  } // namespace memory
  using namespace memory::casts;
} // namespace bit

#include "detail/endian.inl"

#endif /* BIT_MEMORY_UTILITIES_ENDIAN_HPP */
