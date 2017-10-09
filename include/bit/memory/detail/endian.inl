#ifndef BIT_MEMORY_DETAIL_ENDIAN_INL
#define BIT_MEMORY_DETAIL_ENDIAN_INL

//-----------------------------------------------------------------------------
// Swapping
//-----------------------------------------------------------------------------

inline char bit::memory::endian_swap( char val )
  noexcept
{
  return val;
}

inline bit::memory::byte bit::memory::endian_swap( byte val )
  noexcept
{
  return val;
}

inline std::int8_t bit::memory::endian_swap( std::int8_t val )
  noexcept
{
  return val;
}

inline std::uint8_t bit::memory::endian_swap( std::uint8_t val )
  noexcept
{
  return val;
}

//-----------------------------------------------------------------------------

inline std::int16_t bit::memory::endian_swap( std::int16_t val )
  noexcept
{
  return endian_swap( static_cast<std::uint16_t>(val) );
}

inline std::uint16_t bit::memory::endian_swap( std::uint16_t val )
  noexcept
{
#ifdef _MSC_VER
  return _byteswap_ushort(val);
#elif defined(__GNUC__)
  return __builtin_bswap16(val);
#else
  return ((x & 00xff) << 8) | ((x & 0xff00) >> 8));
#endif
}

//-----------------------------------------------------------------------------

inline std::int32_t bit::memory::endian_swap( std::int32_t val )
  noexcept
{
  return endian_swap( static_cast<std::uint32_t>(val) );
}

inline std::uint32_t bit::memory::endian_swap( std::uint32_t val )
  noexcept
{
#ifdef _MSC_VER
  return _byteswap_ulong(val);
#elif defined(__GNUC__)
  return __builtin_bswap32(val);
#else
  return ((x & 0xff000000) >> 24) |
         ((x & 0x00ff0000) >> 8 ) |
         ((x & 0x0000ff00) << 8 ) |
         ((x & 0x000000ff) << 24);
#endif
}

//-----------------------------------------------------------------------------

inline std::int64_t bit::memory::endian_swap( std::int64_t val )
  noexcept
{
  return static_cast<std::int64_t>( endian_swap( static_cast<std::uint64_t>(val) ) );
}

inline std::uint64_t bit::memory::endian_swap( std::uint64_t val )
  noexcept
{
#ifdef _MSC_VER
  return _byteswap_uint64(val);
#elif defined(__GNUC__)
  return __builtin_bswap64(val);
#else
  return ((x & 0xff00000000000000ull) >> 56) |
         ((x & 0x00ff000000000000ull) >> 40) |
         ((x & 0x0000ff0000000000ull) >> 24) |
         ((x & 0x000000ff00000000ull) >> 8 ) |
         ((x & 0x00000000ff000000ull) << 8 ) |
         ((x & 0x0000000000ff0000ull) << 24) |
         ((x & 0x000000000000ff00ull) << 40) |
         ((x & 0x00000000000000ffull) << 56);
#endif
}

//-----------------------------------------------------------------------------

inline float bit::memory::endian_swap( float val )
  noexcept
{
  return *endian_swap_in_place(&val);
}

inline double bit::memory::endian_swap( double val )
  noexcept
{
  return *endian_swap_in_place(&val);
}

//-----------------------------------------------------------------------------

template<typename T, typename>
inline T bit::memory::endian_swap( T val )
{
  return *endian_swap_in_place(&val);
}

//-----------------------------------------------------------------------------
// Buffers
//-----------------------------------------------------------------------------

template<typename CharT, typename>
inline CharT* bit::memory::endian_swap_buffer_in_place( CharT* p,
                                                        std::size_t n )
  noexcept
{
  std::reverse(p, p + n);
  return p;
}

template<typename T>
inline T* bit::memory::endian_swap_in_place( T* p )
  noexcept
{
  // Note: Reinterpret casting T to a byte pointer is legal C++

  // TODO(bitwize): Investigate why converting to 'bit::memory::byte*' is being
  //                optimized out, whereas to 'char*' is not. By the standard,
  //                they should both be considered byte pointers.
  endian_swap_buffer_in_place( reinterpret_cast<char*>(p), sizeof(T) );

  return p;
}


//-----------------------------------------------------------------------------
// Endian Casting
//-----------------------------------------------------------------------------

namespace bit { namespace memory { inline namespace casts { namespace detail {

  template<endian Endianness, typename T>
  struct endian_caster
  {
    inline T operator()( T val )
      noexcept
    {
      return endian_swap( std::forward<T>(val) );
    }
  };

  template<typename T>
  struct endian_caster<endian::native,T>
  {
    inline T operator()( T val )
      noexcept
    {
      return std::forward<T>(val);
    }
  };

} } } } // namespace bit::memory::casts::detail

//-----------------------------------------------------------------------------

template<bit::memory::endian Endianness, typename T>
inline T bit::memory::casts::endian_cast( T val )
  noexcept
{
  return detail::endian_caster<Endianness,T>()( val );
}

#endif /* BIT_MEMORY_DETAIL_ENDIAN_INL */
