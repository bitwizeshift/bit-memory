/**
 * \file pointer_utilities.hpp
 *
 * \brief This header defines various pointer utilities to simplify deriving
 *        pointers
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */
#ifndef BIT_MEMORY_POINTER_UTILITIES_HPP
#define BIT_MEMORY_POINTER_UTILITIES_HPP

#include "byte.hpp" // byte

#include <cstdint> // std::uintptr_t
#include <cstddef> // std::size_t, std::ptrdiff_t, std::nullptr_t
#include <cassert> // assert

namespace bit {
  namespace memory {

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

    /// \brief Aligns the pointer \p ptr forward to the next
    ///        \p align byte address
    ///
    /// \param ptr
    /// \param size
    /// \param align
    /// \return
    void* align_forward( void* ptr,
                         std::size_t alignment,
                         std::size_t* adjust = nullptr ) noexcept;

    void* align_backward( void* ptr,
                          std::size_t alignment,
                          std::size_t* adjust = nullptr ) noexcept;

    /// \copydoc align_forward
    constexpr auto align = &align_forward;

    //-------------------------------------------------------------------------
    // Align with Offset
    //-------------------------------------------------------------------------

    void* offset_align_forward( void* ptr,
                                std::size_t alignment,
                                std::size_t offset,
                                std::size_t* adjust = nullptr ) noexcept;

    void* offset_align_backward( void* ptr,
                                 std::size_t alignment,
                                 std::size_t offset,
                                 std::size_t* adjust = nullptr ) noexcept;

    /// \copydoc offset_align_forward
    constexpr auto offset_align = &offset_align_forward;
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

#endif /* BIT_MEMORY_POINTER_UTILITIES_HPP */
