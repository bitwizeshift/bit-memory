#ifndef BIT_MEMORY_UTILITIES_DETAIL_ADDRESS_INL
#define BIT_MEMORY_UTILITIES_DETAIL_ADDRESS_INL

//=============================================================================
// X.Y.1 : enum class address
//=============================================================================

//-----------------------------------------------------------------------------
// Compound Operators
//-----------------------------------------------------------------------------

template<typename IntT, typename>
inline bit::memory::address&
  bit::memory::operator+=( address& lhs, IntT rhs )
  noexcept
{
  return lhs = static_cast<address>( static_cast<std::uintptr_t>(lhs) + rhs );
}

template<typename IntT, typename>
inline bit::memory::address&
  bit::memory::operator-=( address& lhs, IntT rhs )
  noexcept
{
  return lhs = static_cast<address>( static_cast<std::uintptr_t>(lhs) - rhs );
}

//-----------------------------------------------------------------------------

inline bit::memory::address&
  bit::memory::operator&=( address& lhs, address rhs )
  noexcept
{
  return lhs = static_cast<address>( static_cast<std::uintptr_t>(lhs) &
                                     static_cast<std::uintptr_t>(rhs) );
}

template<typename IntT, typename>
inline bit::memory::address&
  bit::memory::operator&=( address& lhs, IntT rhs )
  noexcept
{
  return lhs = static_cast<address>( static_cast<std::uintptr_t>(lhs) & rhs );
}

inline bit::memory::address&
  bit::memory::operator|=( address& lhs, address rhs )
  noexcept
{
  return lhs = static_cast<address>( static_cast<std::uintptr_t>(lhs) |
                                     static_cast<std::uintptr_t>(rhs) );
}

template<typename IntT, typename>
inline bit::memory::address&
  bit::memory::operator|=( address& lhs, IntT rhs )
  noexcept
{
  return lhs = static_cast<address>( static_cast<std::uintptr_t>(lhs) | rhs );
}

inline bit::memory::address&
  bit::memory::operator^=( address& lhs, address rhs )
  noexcept
{
  return lhs = static_cast<address>( static_cast<std::uintptr_t>(lhs) ^
                                     static_cast<std::uintptr_t>(rhs) );
}

template<typename IntT, typename>
inline bit::memory::address&
  bit::memory::operator^=( address& lhs, IntT rhs )
  noexcept
{
  return lhs = static_cast<address>( static_cast<std::uintptr_t>(lhs) ^ rhs );
}

//-----------------------------------------------------------------------------
// Binary Operators
//-----------------------------------------------------------------------------

template<typename IntT, typename>
inline constexpr bit::memory::address
  bit::memory::operator+( address lhs, IntT rhs )
  noexcept
{
  return static_cast<address>( static_cast<std::uintptr_t>(lhs) + rhs );
}

template<typename IntT, typename>
inline constexpr bit::memory::address
  bit::memory::operator+( IntT lhs, address rhs )
  noexcept
{
  return static_cast<address>( lhs + static_cast<std::uintptr_t>(rhs) );
}
template<typename IntT, typename>
inline constexpr bit::memory::address
  bit::memory::operator-( address lhs, IntT rhs )
  noexcept
{
  return static_cast<address>( static_cast<std::uintptr_t>(lhs) - rhs );
}

//-----------------------------------------------------------------------------

inline constexpr bit::memory::address
  bit::memory::operator~( address x )
  noexcept
{
  return static_cast<address>( ~static_cast<std::uintptr_t>(x) );
}

//-----------------------------------------------------------------------------

template<typename IntT, typename>
inline constexpr bit::memory::address
  bit::memory::operator&( address lhs, IntT rhs )
  noexcept
{
  return static_cast<address>( static_cast<std::uintptr_t>(lhs) & rhs );
}

inline constexpr bit::memory::address
  bit::memory::operator&( address lhs, address rhs )
  noexcept
{
  return static_cast<address>( static_cast<std::uintptr_t>(lhs) &
                               static_cast<std::uintptr_t>(rhs) );
}

template<typename IntT, typename>
inline constexpr bit::memory::address
  bit::memory::operator|( address lhs, IntT rhs )
  noexcept
{
  return static_cast<address>( static_cast<std::uintptr_t>(lhs) | rhs );
}

inline constexpr bit::memory::address
  bit::memory::operator|( address lhs, address rhs )
  noexcept
{
  return static_cast<address>( static_cast<std::uintptr_t>(lhs) |
                               static_cast<std::uintptr_t>(rhs) );
}

template<typename IntT, typename>
inline constexpr bit::memory::address
  bit::memory::operator^( address lhs, IntT rhs )
  noexcept
{
  return static_cast<address>( static_cast<std::uintptr_t>(lhs) ^ rhs );
}

inline constexpr bit::memory::address
  bit::memory::operator^( address lhs, address rhs )
  noexcept
{
  return static_cast<address>( static_cast<std::uintptr_t>(lhs) ^
                               static_cast<std::uintptr_t>(rhs) );
}

//=============================================================================
// X.Y.2 : enum class const_address
//=============================================================================

//-----------------------------------------------------------------------------
// Compound Operators
//-----------------------------------------------------------------------------

template<typename IntT, typename>
inline bit::memory::const_address&
  bit::memory::operator+=( const_address& lhs, IntT rhs )
  noexcept
{
  return lhs = static_cast<const_address>( static_cast<std::uintptr_t>(lhs) + rhs );
}

template<typename IntT, typename>
inline bit::memory::const_address&
  bit::memory::operator-=( const_address& lhs, IntT rhs )
  noexcept
{
  return lhs = static_cast<const_address>( static_cast<std::uintptr_t>(lhs) - rhs );
}

//-----------------------------------------------------------------------------

inline bit::memory::const_address&
  bit::memory::operator&=( const_address& lhs, const_address rhs )
  noexcept
{
  return lhs = static_cast<const_address>( static_cast<std::uintptr_t>(lhs) &
                                           static_cast<std::uintptr_t>(rhs) );
}

template<typename IntT, typename>
inline bit::memory::const_address&
  bit::memory::operator&=( const_address& lhs, IntT rhs )
  noexcept
{
  return lhs = static_cast<const_address>( static_cast<std::uintptr_t>(lhs) & rhs );
}

inline bit::memory::const_address&
  bit::memory::operator|=( const_address& lhs, const_address rhs )
  noexcept
{
  return lhs = static_cast<const_address>( static_cast<std::uintptr_t>(lhs) |
                                           static_cast<std::uintptr_t>(rhs) );
}

template<typename IntT, typename>
inline bit::memory::const_address&
  bit::memory::operator|=( const_address& lhs, IntT rhs )
  noexcept
{
  return lhs = static_cast<const_address>( static_cast<std::uintptr_t>(lhs) | rhs );
}

inline bit::memory::const_address&
  bit::memory::operator^=( const_address& lhs, const_address rhs )
  noexcept
{
  return lhs = static_cast<const_address>( static_cast<std::uintptr_t>(lhs) ^
                                           static_cast<std::uintptr_t>(rhs) );
}

template<typename IntT, typename>
inline bit::memory::const_address&
  bit::memory::operator^=( const_address& lhs, IntT rhs )
  noexcept
{
  return lhs = static_cast<const_address>( static_cast<std::uintptr_t>(lhs) ^ rhs );
}

//-----------------------------------------------------------------------------
// Binary Operators
//-----------------------------------------------------------------------------

template<typename IntT, typename>
inline constexpr bit::memory::const_address
  bit::memory::operator+( const_address lhs, IntT rhs )
  noexcept
{
  return static_cast<const_address>( static_cast<std::uintptr_t>(lhs) + rhs );
}

template<typename IntT, typename>
inline constexpr bit::memory::const_address
  bit::memory::operator+( IntT lhs, const_address rhs )
  noexcept
{
  return static_cast<const_address>( lhs + static_cast<std::uintptr_t>(rhs) );
}
template<typename IntT, typename>
inline constexpr bit::memory::const_address
  bit::memory::operator-( const_address lhs, IntT rhs )
  noexcept
{
  return static_cast<const_address>( static_cast<std::uintptr_t>(lhs) - rhs );
}

//-----------------------------------------------------------------------------

inline constexpr bit::memory::const_address
  bit::memory::operator~( const_address x )
  noexcept
{
  return static_cast<const_address>( ~static_cast<std::uintptr_t>(x) );
}

//-----------------------------------------------------------------------------

template<typename IntT, typename>
inline constexpr bit::memory::const_address
  bit::memory::operator&( const_address lhs, IntT rhs )
  noexcept
{
  return static_cast<const_address>( static_cast<std::uintptr_t>(lhs) & rhs );
}

inline constexpr bit::memory::const_address
  bit::memory::operator&( const_address lhs, const_address rhs )
  noexcept
{
  return static_cast<const_address>( static_cast<std::uintptr_t>(lhs) &
                                     static_cast<std::uintptr_t>(rhs) );
}

template<typename IntT, typename>
inline constexpr bit::memory::const_address
  bit::memory::operator|( const_address lhs, IntT rhs )
  noexcept
{
  return static_cast<const_address>( static_cast<std::uintptr_t>(lhs) | rhs );
}

inline constexpr bit::memory::const_address
  bit::memory::operator|( const_address lhs, const_address rhs )
  noexcept
{
  return static_cast<const_address>( static_cast<std::uintptr_t>(lhs) |
                                     static_cast<std::uintptr_t>(rhs) );
}

template<typename IntT, typename>
inline constexpr bit::memory::const_address
  bit::memory::operator^( const_address lhs, IntT rhs )
  noexcept
{
  return static_cast<const_address>( static_cast<std::uintptr_t>(lhs) ^ rhs );
}

inline constexpr bit::memory::const_address
  bit::memory::operator^( const_address lhs, const_address rhs )
  noexcept
{
  return static_cast<const_address>( static_cast<std::uintptr_t>(lhs) ^
                                     static_cast<std::uintptr_t>(rhs) );
}

//=============================================================================
// X.Y.3 : address/const_address utilities
//=============================================================================

inline bit::memory::address bit::memory::to_address( void* p )
  noexcept
{
  return static_cast<address>( reinterpret_cast<std::uintptr_t>(p) );
}

inline bit::memory::const_address bit::memory::to_address( const void* p )
  noexcept
{
  return static_cast<const_address>( reinterpret_cast<std::uintptr_t>(p) );
}

//-----------------------------------------------------------------------------

inline void* bit::memory::to_pointer( address a )
  noexcept
{
  return reinterpret_cast<void*>( static_cast<std::uintptr_t>(a) );
}

inline const void* bit::memory::to_pointer( const_address a )
  noexcept
{
  return reinterpret_cast<const void*>( static_cast<std::uintptr_t>(a) );
}

//-----------------------------------------------------------------------------

template<typename T>
inline T* bit::memory::to_pointer( address a )
  noexcept
{
  return static_cast<T*>( to_pointer(a) );
}

template<typename T>
inline const T* bit::memory::to_pointer( const_address a )
  noexcept
{
  return static_cast<const T*>( to_pointer(a) );
}

//=============================================================================
// X.Y.4 : address/const_address comparisons
//=============================================================================

inline constexpr bool bit::memory::operator==( address lhs, const_address rhs )
  noexcept
{
  return static_cast<std::uintptr_t>(lhs)==static_cast<std::uintptr_t>(rhs);
}

inline constexpr bool bit::memory::operator==( const_address lhs, address rhs )
  noexcept
{
  return static_cast<std::uintptr_t>(lhs)==static_cast<std::uintptr_t>(rhs);
}

inline constexpr bool bit::memory::operator!=( address lhs, const_address rhs )
  noexcept
{
  return static_cast<std::uintptr_t>(lhs)==static_cast<std::uintptr_t>(rhs);
}

inline constexpr bool bit::memory::operator!=( const_address lhs, address rhs )
  noexcept
{
  return static_cast<std::uintptr_t>(lhs)==static_cast<std::uintptr_t>(rhs);
}

inline constexpr bool bit::memory::operator<( address lhs, const_address rhs )
  noexcept
{
  return static_cast<std::uintptr_t>(lhs)==static_cast<std::uintptr_t>(rhs);
}

inline constexpr bool bit::memory::operator<( const_address lhs, address rhs )
  noexcept
{
  return static_cast<std::uintptr_t>(lhs)==static_cast<std::uintptr_t>(rhs);
}

inline constexpr bool bit::memory::operator>( address lhs, const_address rhs )
  noexcept
{
  return static_cast<std::uintptr_t>(lhs)==static_cast<std::uintptr_t>(rhs);
}

inline constexpr bool bit::memory::operator>( const_address lhs, address rhs )
  noexcept
{
  return static_cast<std::uintptr_t>(lhs)==static_cast<std::uintptr_t>(rhs);
}

inline constexpr bool bit::memory::operator<=( address lhs, const_address rhs )
  noexcept
{
  return static_cast<std::uintptr_t>(lhs)==static_cast<std::uintptr_t>(rhs);
}

inline constexpr bool bit::memory::operator<=( const_address lhs, address rhs )
  noexcept
{
  return static_cast<std::uintptr_t>(lhs)==static_cast<std::uintptr_t>(rhs);
}

inline constexpr bool bit::memory::operator>=( address lhs, const_address rhs )
  noexcept
{
  return static_cast<std::uintptr_t>(lhs)==static_cast<std::uintptr_t>(rhs);
}

inline constexpr bool bit::memory::operator>=( const_address lhs, address rhs )
  noexcept
{
  return static_cast<std::uintptr_t>(lhs)==static_cast<std::uintptr_t>(rhs);
}

#endif /* BIT_MEMORY_UTILITIES_DETAIL_ADDRESS_INL */
