#ifndef BIT_MEMORY_DETAIL_MEMORY_INL
#define BIT_MEMORY_DETAIL_MEMORY_INL

//----------------------------------------------------------------------------
// Byte Literals
//----------------------------------------------------------------------------

inline constexpr std::size_t
  bit::memory::literals::byte_literals::operator ""_b( unsigned long long b )
  noexcept
{
  return b;
}

inline constexpr std::size_t
  bit::memory::literals::byte_literals::operator ""_kb( unsigned long long b )
  noexcept
{
  return b << 10;
}

inline constexpr std::size_t
  bit::memory::literals::byte_literals::operator ""_mb( unsigned long long b )
  noexcept
{
  return b << 20;
}

inline constexpr std::size_t
  bit::memory::literals::byte_literals::operator ""_gb( unsigned long long b )
  noexcept
{
  return b << 30;
}

#endif /* BIT_MEMORY_DETAIL_MEMORY_INL */
