#ifndef BIT_MEMORY_DETAIL_UNALIGNED_MEMORY_INL
#define BIT_MEMORY_DETAIL_UNALIGNED_MEMORY_INL

//-----------------------------------------------------------------------------
// Unaligned Loading
//-----------------------------------------------------------------------------

template<typename T>
inline T bit::memory::load_unaligned( void* p )
{
  alignas(alignof(T)) char storage[sizeof(T)];

  std::memcpy(&storage[0],p,sizeof(T));

  return (*reinterpret_cast<T*>(&storage[0]));
}

//-----------------------------------------------------------------------------

inline std::int8_t bit::memory::load_unaligned_int8( void* p )
{
  return load_unaligned<std::int8_t>(p);
}

inline std::uint8_t bit::memory::load_unaligned_uint8( void* p )
{
  return load_unaligned<std::uint8_t>(p);
}

//-----------------------------------------------------------------------------

inline std::int16_t bit::memory::load_unaligned_int16( void* p )
{
  return load_unaligned<std::int16_t>(p);
}

inline std::uint16_t bit::memory::load_unaligned_uint16( void* p )
{
  return load_unaligned<std::uint16_t>(p);
}

//-----------------------------------------------------------------------------

inline std::int32_t bit::memory::load_unaligned_int32( void* p )
{
  return load_unaligned<std::int32_t>(p);
}

inline std::uint32_t bit::memory::load_unaligned_uint32( void* p )
{
  return load_unaligned<std::uint32_t>(p);
}

//-----------------------------------------------------------------------------

inline std::int64_t bit::memory::load_unaligned_int64( void* p )
{
  return load_unaligned<std::int64_t>(p);
}

inline std::uint64_t bit::memory::load_unaligned_uint64( void* p )
{
  return load_unaligned<std::uint64_t>(p);
}

//-----------------------------------------------------------------------------

inline float bit::memory::load_unaligned_float( void* p )
{
  return load_unaligned<float>(p);
}

inline double bit::memory::load_unaligned_double( void* p )
{
  return load_unaligned<double>(p);
}

#endif /* BIT_MEMORY_DETAIL_UNALIGNED_MEMORY_INL */
