#ifndef BIT_MEMORY_BYTE_INL
#define BIT_MEMORY_BYTE_INL

template<typename IntT, typename>
inline constexpr bit::memory::byte bit::memory::operator<<(byte lhs, IntT shift)
  noexcept
{
  return byte(static_cast<unsigned char>(lhs) << shift);
}

template<typename IntT, typename>
inline constexpr bit::memory::byte bit::memory::operator>>(byte lhs, IntT shift)
  noexcept
{
  return byte(static_cast<unsigned char>(lhs) >> shift);
}

inline constexpr bit::memory::byte bit::memory::operator|(byte lhs, byte rhs)
  noexcept
{
  return byte(static_cast<unsigned char>(lhs) | static_cast<unsigned char>(rhs));
}

inline constexpr bit::memory::byte bit::memory::operator&(byte lhs, byte rhs)
  noexcept
{
  return byte(static_cast<unsigned char>(lhs) & static_cast<unsigned char>(rhs));
}


inline constexpr bit::memory::byte bit::memory::operator^(byte lhs, byte rhs)
  noexcept
{
  return byte(static_cast<unsigned char>(lhs) ^ static_cast<unsigned char>(rhs));
}

inline constexpr bit::memory::byte bit::memory::operator~(byte lhs)
  noexcept
{
  return byte(~static_cast<unsigned char>(lhs));
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------

template<typename IntT, typename>
inline constexpr bit::memory::byte& bit::memory::operator<<=(byte& lhs, IntT shift)
  noexcept
{
  return lhs = byte(static_cast<unsigned char>(lhs) << shift);
}

template<typename IntT, typename>
inline constexpr bit::memory::byte& bit::memory::operator>>=(byte& lhs, IntT shift)
  noexcept
{
  return lhs = byte(static_cast<unsigned char>(lhs) >> shift);
}

inline bit::memory::byte& bit::memory::operator|=(byte& lhs, byte rhs)
  noexcept
{
  return lhs = byte(static_cast<unsigned char>(lhs) | static_cast<unsigned char>(rhs));
}

inline bit::memory::byte& bit::memory::operator&=(byte& lhs, byte rhs)
  noexcept
{
  return lhs = byte(static_cast<unsigned char>(lhs) & static_cast<unsigned char>(rhs));
}

inline bit::memory::byte& bit::memory::operator^=(byte& lhs, byte rhs)
  noexcept
{
  return lhs = byte(static_cast<unsigned char>(lhs) ^ static_cast<unsigned char>(rhs));
}

//----------------------------------------------------------------------------
// Byte Literals
//----------------------------------------------------------------------------

inline constexpr bit::memory::byte
  bit::memory::literals::byte_literals::operator ""_byte( unsigned long long b )
  noexcept
{
  return static_cast<byte>(b);
}

#endif // BIT_MEMORY_BYTE_INL
