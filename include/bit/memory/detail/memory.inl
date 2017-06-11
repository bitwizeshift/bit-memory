#ifndef BIT_MEMORY_DETAIL_MEMORY_INL
#define BIT_MEMORY_DETAIL_MEMORY_INL


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

template<typename T, typename...Args>
inline T* bit::memory::uninitialized_construct_at( void* ptr, Args&&...args )
{
  return new (ptr) T( std::forward<Args>(args)... );
}

template<typename T>
inline T* bit::memory::uninitialized_construct_array_at( void* p,
                                                         std::size_t n )
{
  auto current   = static_cast<T*>(p);
  const auto end = current + n;

  while( current != end ) {
    uninitialized_construct_at(current++);
  }
}

template<typename T>
inline void bit::memory::destroy_at( T* p )
{
  p->~T();
}

template<typename T>
inline void bit::memory::destroy_array_at( T* p, std::size_t n )
{
  const auto end = static_cast<T*>(p);
  auto current   = static_cast<T*>(p) + n;

  while( current != end ) {
    destroy_at(--current);
  }
}

#endif /* BIT_MEMORY_DETAIL_MEMORY_INL */
