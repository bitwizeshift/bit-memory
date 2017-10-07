/**
 * \file byte.hpp
 *
 * \brief This header defines an unambiguous byte type, used for pointers and
 *        byte management
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */
#ifndef BIT_MEMORY_BYTE_HPP
#define BIT_MEMORY_BYTE_HPP

#include <type_traits> // std::enable_if, std::is_integral

namespace bit {
  namespace memory {

    ///////////////////////////////////////////////////////////////////////////
    /// \brief An unambiguous integral type of exactly size 1
    ///
    /// This avoids ambiguity between char, signed char, and unsigned char
    ///////////////////////////////////////////////////////////////////////////
    enum class byte : unsigned char{};

    //-------------------------------------------------------------------------
    // Operators
    //-------------------------------------------------------------------------

#ifndef BIT_DOXYGEN_BUILD
    template<typename IntT, typename = std::enable_if_t<std::is_integral<IntT>::value>>
#else
    template<typename IntT>
#endif
    constexpr byte operator<<(byte lhs, IntT shift) noexcept;

#ifndef BIT_DOXYGEN_BUILD
    template<typename IntT, typename = std::enable_if_t<std::is_integral<IntT>::value>>
#else
    template<typename IntT>
#endif
    constexpr byte operator>>(byte lhs, IntT shift) noexcept;
    constexpr byte operator|(byte lhs, byte rhs) noexcept;
    constexpr byte operator&(byte lhs, byte rhs) noexcept;
    constexpr byte operator^(byte lhs, byte rhs) noexcept;
    constexpr byte operator~(byte lhs) noexcept;

    //-------------------------------------------------------------------------

#ifndef BIT_DOXYGEN_BUILD
    template<typename IntT, typename = std::enable_if_t<std::is_integral<IntT>::value>>
#else
    template<typename IntT>
#endif
    constexpr byte& operator<<=(byte& lhs, IntT shift) noexcept;

#ifndef BIT_DOXYGEN_BUILD
    template<typename IntT, typename = std::enable_if_t<std::is_integral<IntT>::value>>
#else
    template<typename IntT>
#endif
    constexpr byte& operator>>=(byte& lhs, IntT shift) noexcept;

    byte& operator|=(byte& lhs, byte rhs) noexcept;
    byte& operator&=(byte& lhs, byte rhs) noexcept;
    byte& operator^=(byte& lhs, byte rhs) noexcept;

    //-------------------------------------------------------------------------
    // Byte Literals
    //-------------------------------------------------------------------------

    inline namespace literals {
      inline namespace byte_literals {

        constexpr byte operator ""_byte( unsigned long long b ) noexcept;

      } // inline namespace byte_literals
    } // inline namespace literals

  } // namespace memory
} // namespace bit

#include "detail/byte.inl"

#endif // BIT_MEMORY_BYTE_HPP
