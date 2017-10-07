/**
 * \file memory.hpp
 *
 * \brief This header is guaranteed to define the type 'byte'
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */

#ifndef BIT_MEMORY_MEMORY_HPP
#define BIT_MEMORY_MEMORY_HPP

#include "byte.hpp" // byte

#include <cstddef> // std::size_t, std::ptrdiff_t
#include <utility> // std::forward
#include <cstdint> // std::uintpr_t
#include <utility> // std::forward, std::index_sequence
#include <tuple>   // std::tuple, std::forward_as_tuple

namespace bit {
  namespace memory {

    //------------------------------------------------------------------------
    // Byte Literals
    //------------------------------------------------------------------------

    inline namespace literals {
      inline namespace byte_literals {

        constexpr std::size_t operator ""_b( unsigned long long b ) noexcept;
        constexpr std::size_t operator ""_kb( unsigned long long b ) noexcept;
        constexpr std::size_t operator ""_mb( unsigned long long b ) noexcept;
        constexpr std::size_t operator ""_gb( unsigned long long b ) noexcept;

      } // inline namespace byte_literals
    } // inline namespace literals

    //------------------------------------------------------------------------
    // Pointer Manipulation
    //------------------------------------------------------------------------

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



    //------------------------------------------------------------------------
    // Nullability
    //------------------------------------------------------------------------

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

    //------------------------------------------------------------------------
    // Deltas
    //------------------------------------------------------------------------

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

  inline namespace literals {
    using namespace memory::literals;
  } // inline namespace literals
} // namespace bit

#include "detail/memory.inl"

#endif /* BIT_MEMORY_MEMORY_HPP */
