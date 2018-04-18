/*****************************************************************************
 * \file
 * \brief This header contains various utilities for loading unaligned memory
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
#ifndef BIT_MEMORY_UTILITIES_UNALIGNED_STORAGE_HPP
#define BIT_MEMORY_UTILITIES_UNALIGNED_STORAGE_HPP

#include <cstdint>     // std::uint8_t, std::uint16_t, etc
#include <cstring>     // std::memcpy
#include <memory>      // std::addressof
#include <type_traits> // std::is_trivially_copyable
#include <utility>     // std::aligned_storage

namespace bit {
  namespace memory {

    //-------------------------------------------------------------------------
    // Storing
    //-------------------------------------------------------------------------

    /// \brief Stores an arbitrary type \p T into unaligned memory
    ///
    /// \pre \c std::is_trivially_copyable<T>::value is \c true
    ///
    /// \param p the memory to store into
    /// \param val the value to store
    template<typename T>
    void store_unaligned( void* p, const T& val );

    //-------------------------------------------------------------------------
    // Loading
    //-------------------------------------------------------------------------

    /// \brief Loads an arbitrary type \p T from unaligned memory
    ///
    /// \note Only types that don't contain pointers that point inside the
    ///       loaded type can be safely loaded. Otherwise, the pointers will be
    ///       pointing to potentially unsafe memory. Ideally, this function
    ///       should only be used to load fundamental types, or simple
    ///       pod/aggregate types.
    ///
    /// \pre \c std::is_trivially_copyable<T>::value is \c true
    ///
    /// \param p pointer to the memory to load from
    /// \return the type \p T
    template<typename T>
    T load_unaligned( const void* p );

    //-------------------------------------------------------------------------

    /// \brief Loads an unaligned std::int8_t from the memory pointed at
    ///        by \p p
    ///
    /// \param p pointer to the memory to load from
    /// \return the loaded std::int8_t
    std::int8_t load_unaligned_int8( const void* p );

    /// \brief Loads an unaligned std::uint8_t from the memory pointed at
    ///        by \p p
    ///
    /// \param p pointer to the memory to load from
    /// \return the loaded std::uint8_t
    std::uint8_t load_unaligned_uint8( const void* p );

    //-------------------------------------------------------------------------

    /// \brief Loads an unaligned std::int16_t from the memory pointed at
    ///        by \p p
    ///
    /// \param p pointer to the memory to load from
    /// \return the loaded std::int16_t
    std::int16_t load_unaligned_int16( const void* p );

    /// \brief Loads an unaligned std::uint16_t from the memory pointed at
    ///        by \p p
    ///
    /// \param p pointer to the memory to load from
    /// \return the loaded std::uint16_t
    std::uint16_t load_unaligned_uint16( const void* p );

    //-------------------------------------------------------------------------

    /// \brief Loads an unaligned std::int32_t from the memory pointed at
    ///        by \p p
    ///
    /// \param p pointer to the memory to load from
    /// \return the loaded std::int32_t
    std::int32_t load_unaligned_int32( const void* p );

    /// \brief Loads an unaligned std::uint32_t from the memory pointed at
    ///        by \p p
    ///
    /// \param p pointer to the memory to load from
    /// \return the loaded std::uint32_t
    std::uint32_t load_unaligned_uint32( const void* p );

    //-------------------------------------------------------------------------

    /// \brief Loads an unaligned std::int64_t from the memory pointed at
    ///        by \p p
    ///
    /// \param p pointer to the memory to load from
    /// \return the loaded std::int64_t
    std::int64_t load_unaligned_int64( const void* p );

    /// \brief Loads an unaligned std::uint64_t from the memory pointed at
    ///        by \p p
    ///
    /// \param p pointer to the memory to load from
    /// \return the loaded std::uint64_t
    std::uint64_t load_unaligned_uint64( const void* p );

    //-------------------------------------------------------------------------

    /// \brief Loads an unaligned float from the memory pointed at
    ///        by \p p
    ///
    /// \param p pointer to the memory to load from
    /// \return the loaded float
    float load_unaligned_float( const void* p );

    /// \brief Loads an unaligned double from the memory pointed at
    ///        by \p p
    ///
    /// \param p pointer to the memory to load from
    /// \return the loaded double
    double load_unaligned_double( const void* p );

  } // namespace memory
} // namespace bit

#include "detail/unaligned_storage.inl"

#endif /* BIT_MEMORY_UTILITIES_UNALIGNED_STORAGE_HPP */
