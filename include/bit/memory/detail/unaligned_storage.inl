#ifndef BIT_MEMORY_DETAIL_UNALIGNED_STORAGE_INL
#define BIT_MEMORY_DETAIL_UNALIGNED_STORAGE_INL

//-----------------------------------------------------------------------------
// Storing
//-----------------------------------------------------------------------------

template<typename T>
inline void bit::memory::store_unaligned( void* p, const T& val )
{
  static_assert( std::is_trivially_copyable<T>::value,
                 "T must be trivially copyable" );

  auto val_ptr = reinterpret_cast<const char*>( std::addressof(val) );

  std::memcpy(p,val_ptr,sizeof(T));
}

//-----------------------------------------------------------------------------
// Loading
//-----------------------------------------------------------------------------

template<typename T>
inline T bit::memory::load_unaligned( const void* p )
{
  static_assert( std::is_trivially_copyable<T>::value,
                 "T must be trivially copyable" );

  std::aligned_storage_t<sizeof(T),alignof(T)> result;

  std::memcpy(&result,p,sizeof(T));

  return *reinterpret_cast<T*>(&result);
}

//-----------------------------------------------------------------------------

inline std::int8_t bit::memory::load_unaligned_int8( const void* p )
{
  return load_unaligned<std::int8_t>(p);
}

inline std::uint8_t bit::memory::load_unaligned_uint8( const void* p )
{
  return load_unaligned<std::uint8_t>(p);
}

//-----------------------------------------------------------------------------

inline std::int16_t bit::memory::load_unaligned_int16( const void* p )
{
  return load_unaligned<std::int16_t>(p);
}

inline std::uint16_t bit::memory::load_unaligned_uint16( const void* p )
{
  return load_unaligned<std::uint16_t>(p);
}

//-----------------------------------------------------------------------------

inline std::int32_t bit::memory::load_unaligned_int32( const void* p )
{
  return load_unaligned<std::int32_t>(p);
}

inline std::uint32_t bit::memory::load_unaligned_uint32( const void* p )
{
  return load_unaligned<std::uint32_t>(p);
}

//-----------------------------------------------------------------------------

inline std::int64_t bit::memory::load_unaligned_int64( const void* p )
{
  return load_unaligned<std::int64_t>(p);
}

inline std::uint64_t bit::memory::load_unaligned_uint64( const void* p )
{
  return load_unaligned<std::uint64_t>(p);
}

//-----------------------------------------------------------------------------

inline float bit::memory::load_unaligned_float( const void* p )
{
  return load_unaligned<float>(p);
}

inline double bit::memory::load_unaligned_double( const void* p )
{
  return load_unaligned<double>(p);
}

#endif /* BIT_MEMORY_DETAIL_UNALIGNED_STORAGE_INL */
