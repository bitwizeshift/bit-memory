/**
 * \file byte.hpp
 *
 * \brief todo: fill in documentation
 *
 * \note This is an internal header file, included by other library headers.
 *       Do not attempt to use it directly.
 */
#ifndef BIT_MEMORY_DETAIL_MEMORY_BYTE_HPP
#define BIT_MEMORY_DETAIL_MEMORY_BYTE_HPP

namespace bit {
  namespace memory {

    enum class byte : unsigned char{};

    //------------------------------------------------------------------------
    // Operators
    //------------------------------------------------------------------------

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

    //------------------------------------------------------------------------

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

    //------------------------------------------------------------------------
    // Byte Literals
    //------------------------------------------------------------------------

    inline namespace literals {
      inline namespace byte_literals {

        constexpr byte operator ""_byte( unsigned long long b ) noexcept;

      } // inline namespace byte_literals
    } // inline namespace literals

    template<typename IntT>
    IntT read_packed_bytes( const void* p ) noexcept;

    template<typename IntT>
    void write_packed_bytes( void* p, IntT i ) noexcept;

  } // namespace memory
} // namespace bit

#include "byte.inl"

#endif // BIT_MEMORY_DETAIL_MEMORY_BYTE_HPP
