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

//----------------------------------------------------------------------------
// Pointer Manipulation
//----------------------------------------------------------------------------

inline std::uintptr_t bit::memory::to_address( void* ptr )
noexcept
{
  return reinterpret_cast<std::uintptr_t>(ptr);
}

inline void* bit::memory::from_address( std::uintptr_t address )
noexcept
{
  return reinterpret_cast<void*>(address);
}

//----------------------------------------------------------------------------
// Nullability
//----------------------------------------------------------------------------

template<typename Ptr>
inline constexpr auto bit::memory::is_null( Ptr&& ptr )
  noexcept -> decltype( ptr==nullptr, bool() )
{
  return ptr == nullptr;
}

inline constexpr bool bit::memory::is_null( std::nullptr_t )
  noexcept
{
  return true;
}

//----------------------------------------------------------------------------
// Deltas
//----------------------------------------------------------------------------

inline std::size_t bit::memory::distance( const void* lhs, const void* rhs )
  noexcept
{
  return (lhs > rhs) ? difference(lhs,rhs) : difference(rhs,lhs);
}

inline std::ptrdiff_t bit::memory::difference( const void* lhs, const void* rhs )
  noexcept
{
  return static_cast<const byte*>(lhs) - static_cast<const byte*>(rhs);
}

inline void* bit::memory::advance( void* p, std::ptrdiff_t bytes )
  noexcept
{
  return static_cast<byte*>(p) + bytes;
}

inline const void* bit::memory::advance( const void* p, std::ptrdiff_t bytes )
  noexcept
{
  return static_cast<const byte*>(p) + bytes;
}

#endif /* BIT_MEMORY_DETAIL_MEMORY_INL */
