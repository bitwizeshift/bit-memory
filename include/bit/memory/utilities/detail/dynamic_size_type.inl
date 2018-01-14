#ifndef BIT_MEMORY_UTILITIES_DETAIL_DYNAMIC_SIZE_TYPE_INL
#define BIT_MEMORY_UTILITIES_DETAIL_DYNAMIC_SIZE_TYPE_INL

//=============================================================================
// dynamic_size_type<Index,Size>
//=============================================================================

//-----------------------------------------------------------------------------
// Observers
//-----------------------------------------------------------------------------

template<std::size_t Index, std::size_t Size>
inline constexpr std::size_t
  bit::memory::dynamic_size_type<Index,Size>::value()
  const noexcept
{
  return Size;
}

//=============================================================================
// dynamic_size_type<Index,dynamic_size>
//=============================================================================

//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------

template<std::size_t Index>
inline constexpr bit::memory::dynamic_size_type<Index,bit::memory::dynamic_size>
  ::dynamic_size_type( std::size_t size )
  noexcept
  : m_size(size)
{

}

//-----------------------------------------------------------------------------
// Observers
//-----------------------------------------------------------------------------

template<std::size_t Index>
inline constexpr std::size_t
  bit::memory::dynamic_size_type<Index,bit::memory::dynamic_size>::value()
  const noexcept
{
  return m_size;
}

#endif /* BIT_MEMORY_UTILITIES_DETAIL_DYNAMIC_SIZE_TYPE_INL */
