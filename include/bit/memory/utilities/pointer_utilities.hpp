/*****************************************************************************
 * \file
 * \brief This header defines various pointer utilities to simplify deriving
 *        pointers
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
#ifndef BIT_MEMORY_UTILITIES_POINTER_UTILITIES_HPP
#define BIT_MEMORY_UTILITIES_POINTER_UTILITIES_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <cstdint> // std::uintptr_t
#include <cstddef> // std::size_t, std::ptrdiff_t, std::nullptr_t
#include <cassert> // assert

namespace bit {
  namespace memory {

    //-------------------------------------------------------------------------
    // Pointer Conversion
    //-------------------------------------------------------------------------

    /// \{
    /// \brief Converts a pointer to a raw pointer by repeatedly calling
    ///        \c operator->() until it gets to a raw pointer type
    ///
    /// \param p the pointer to convert to a raw pointer
    /// \return the raw pointer type
    template<typename Ptr>
    auto to_raw_pointer( const Ptr& p ) -> decltype(p.operator->());
    template<typename T>
    constexpr T* to_raw_pointer( T* p ) noexcept;
    /// \}

    //-------------------------------------------------------------------------
    // Alignment Checking
    //-------------------------------------------------------------------------

    /// \brief Determines whether a given value is a power of two (used in
    ///        alignment checks)
    ///
    /// \param x the value to check
    /// \return \c true if the value is a power-of-two
    constexpr bool is_power_of_two( std::size_t x ) noexcept;

    /// \brief Gets the alignment of the pointer
    ///
    /// \param ptr the pointer
    /// \return the alignment of the pointer
    std::size_t align_of( const void* ptr ) noexcept;

    //-------------------------------------------------------------------------
    // Offset Calculation
    //-------------------------------------------------------------------------

    /// \brief Calculates the forward-offset required to align \p ptr to
    ///        \p align alignment
    ///
    /// \param ptr The pointer to align
    /// \param align The alignment to calculate
    /// \param offset The amount of offset to leave prior to alignment
    /// \return the amount of bytes required to align to the next aligned
    ///         address
    /// \{
    std::size_t align_forward_offset( void* ptr,
                                      std::size_t align ) noexcept;
    std::size_t align_forward_offset( void* ptr,
                                      std::size_t align,
                                      std::size_t offset ) noexcept;
    /// \}

    /// \brief Calculates the backward-offset required to align \p ptr to
    ///        \p align alignment
    ///
    /// \param ptr The pointer to align
    /// \param align The alignment to calculate
    /// \param offset The amount of offset to leave prior to alignment
    /// \return the amount of bytes required to align to the next aligned
    ///         address
    /// \{
    std::size_t align_backward_offset( void* ptr,
                                       std::size_t align ) noexcept;
    std::size_t align_backward_offset( void* ptr,
                                       std::size_t align,
                                       std::size_t offset ) noexcept;
    /// \}

    //-------------------------------------------------------------------------
    // Align
    //-------------------------------------------------------------------------

    /// \{
    /// \brief Aligns memory to a higher memory address at an alignment boundary
    ///        of \p alignment
    ///
    /// \param p the pointer to align
    /// \param alignment the alignment
    /// \param adjust [out] the amount the pointer is allocated by
    /// \return the aligned pointer
    void* align_forward( void* ptr,
                         std::size_t alignment,
                         std::size_t* adjust ) noexcept;
    void* align_forward( void* ptr,
                         std::size_t alignment,
                         std::nullptr_t ) noexcept;
    void* align_forward( void* ptr,
                         std::size_t alignment ) noexcept;
    /// \}

    //-------------------------------------------------------------------------

    /// \{
    /// \brief Aligns memory to a lower memory address at an alignment boundary
    ///        of \p alignment
    ///
    /// \param p the pointer to align
    /// \param alignment the alignment
    /// \param adjust [out] the amount the pointer is allocated by
    /// \return the aligned pointer
    void* align_backward( void* p,
                          std::size_t alignment,
                          std::size_t* adjust ) noexcept;
    void* align_backward( void* p,
                          std::size_t alignment,
                          std::nullptr_t ) noexcept;
    void* align_backward( void* p,
                          std::size_t alignment ) noexcept;
    /// \}

    //-------------------------------------------------------------------------
    // Align with Offset
    //-------------------------------------------------------------------------

    /// \{
    /// \brief Aligns memory to a higher memory address at an alignment boundary
    ///        of \p alignment, offset by \p offset
    ///
    /// \param p the pointer to align
    /// \param alignment the alignment
    /// \param offset the amount to offset the alignment
    /// \param adjust [out] the amount the pointer is allocated by
    /// \return the aligned, offset pointer
    void* offset_align_forward( void* p,
                                std::size_t alignment,
                                std::size_t offset,
                                std::size_t* adjust ) noexcept;
    void* offset_align_forward( void* p,
                                std::size_t alignment,
                                std::size_t offset,
                                std::nullptr_t ) noexcept;
    void* offset_align_forward( void* p,
                                std::size_t alignment,
                                std::size_t offset ) noexcept;
    /// \}

    //-------------------------------------------------------------------------

    /// \{
    /// \brief Aligns memory to a lower memory address at an alignment boundary
    ///        of \p alignment, offset by \p offset
    ///
    /// \param p the pointer to align
    /// \param alignment the alignment
    /// \param offset the amount to offset the alignment
    /// \param adjust [out] the amount the pointer is allocated by
    /// \return the aligned, offset pointer.
    void* offset_align_backward( void* p,
                                 std::size_t alignment,
                                 std::size_t offset,
                                 std::size_t* adjust ) noexcept;
    void* offset_align_backward( void* p,
                                 std::size_t alignment,
                                 std::size_t offset,
                                 std::nullptr_t ) noexcept;
    void* offset_align_backward( void* p,
                                 std::size_t alignment,
                                 std::size_t offset ) noexcept;
    /// \}

    //-------------------------------------------------------------------------
    // Pointer Manipulation
    //-------------------------------------------------------------------------

    /// \brief Converts a pointer \p ptr into an integral type representing
    ///        the address
    ///
    /// \param ptr the pointer to convert to an integral value
    /// \return the numeric address of the given pointer
    std::uintptr_t to_address( void* ptr ) noexcept;

    /// \brief Converts a numeric address \p address into a pointer pointing
    ///        to the address location
    ///
    /// \param address the address value to convert to a pointer
    /// \return the pointer pointing to the given address
    void* from_address( std::uintptr_t address ) noexcept;

    //-------------------------------------------------------------------------
    // Nullability
    //-------------------------------------------------------------------------

    /// \brief Checks whether a given pointer is null
    ///
    /// \param ptr the pointer to check
    /// \return \c true if the ptr is \c nullptr
    template<typename Ptr>
    constexpr auto is_null( Ptr&& ptr ) noexcept
      -> decltype(ptr==nullptr, bool());

    /// \brief Checks whether a given pointer is null
    ///
    /// This overload always returns \c true
    ///
    /// \return \c true
    constexpr bool is_null( std::nullptr_t ) noexcept;

    //-------------------------------------------------------------------------
    // Deltas
    //-------------------------------------------------------------------------

    /// \brief Calculates the distance between two pointers
    ///
    /// The distance between to pointers is the absolute value of the difference
    /// between two pointers
    ///
    /// \param lhs the left pointer
    /// \param rhs the right pointer
    /// \return the distance
    std::size_t distance( const void* lhs, const void* rhs ) noexcept;

    /// \brief Calculates the difference between two pointers
    ///
    /// The difference is identical to \c lhs - \c rhs
    ///
    /// \param lhs the left pointer
    /// \param rhs the right pointer
    /// \return the result of \c lhs - \c rhs
    std::ptrdiff_t difference( const void* lhs, const void* rhs ) noexcept;

    /// \{
    /// \brief Adjusts a pointer \p p by \p bytes
    ///
    /// \param p the pointer to adjust
    /// \param bytes the bytes to adjust by
    /// \return the pointer adjusted by \p bytes
    void* advance( void* p, std::ptrdiff_t bytes ) noexcept;
    const void* advance( const void* p, std::ptrdiff_t bytes ) noexcept;
    /// \}

  } // namespace memory
} // namespace bit

#include "detail/pointer_utilities.inl"

#endif /* BIT_MEMORY_UTILITIES_POINTER_UTILITIES_HPP */
