/**
 * \file alignment.hpp
 *
 * \brief This header contains the definition of various alignment utilities
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */
#ifndef BIT_MEMORY_ALIGNMENT_HPP
#define BIT_MEMORY_ALIGNMENT_HPP

#include "memory.hpp" // advance

#include <cstddef> // std::size_t, std::ptrdiff_t
#include <cstdint> // std::uintpr_t
#include <cassert> // assert

namespace bit {
  namespace memory {

    //------------------------------------------------------------------------
    // Alignment Checking
    //------------------------------------------------------------------------

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

    //------------------------------------------------------------------------
    // Offset Calculation
    //------------------------------------------------------------------------

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

    //------------------------------------------------------------------------
    // Align
    //------------------------------------------------------------------------

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

    //------------------------------------------------------------------------
    // Align with Offset
    //------------------------------------------------------------------------

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

  } // namespace memory
}  // namespace bit

#include "detail/alignment.inl"

#endif // BIT_MEMORY_ALIGNMENT_HPP
