#ifndef BIT_MEMORY_DETAIL_MEMORY_BLOCK_INL
#define BIT_MEMORY_DETAIL_MEMORY_BLOCK_INL

//----------------------------------------------------------------------------
// Constructors
//----------------------------------------------------------------------------

inline constexpr bit::memory::memory_block::memory_block()
  noexcept
  : memory_block( nullptr, 0 )
{

}

inline constexpr bit::memory::memory_block::memory_block( nullblock_t )
  noexcept
  : memory_block()
{

}

inline constexpr bit::memory::memory_block::memory_block( void* ptr, std::size_t size )
  noexcept
  : memory_block(ptr,size,nullptr)
{

}

inline constexpr bit::memory::memory_block::memory_block( void* ptr, std::size_t size, const void* origin )
  noexcept
  : m_origin(origin),
    m_data(ptr),
    m_size(size)
{

}


//----------------------------------------------------------------------------
// Element Access
//----------------------------------------------------------------------------

inline constexpr void* bit::memory::memory_block::data()
  noexcept
{
  return m_data;
}

inline constexpr const void* bit::memory::memory_block::data()
  const noexcept
{
  return m_data;
}

inline constexpr const void* bit::memory::memory_block::origin()
  const noexcept
{
  return m_origin;
}

//----------------------------------------------------------------------------
// Observers
//----------------------------------------------------------------------------

inline constexpr std::size_t bit::memory::memory_block::size()
  const noexcept
{
  return m_size;
}

inline bool bit::memory::memory_block::contains( const void* ptr )
  const noexcept
{
  return ptr >= m_data &&
         ptr < static_cast<const void*>(static_cast<const char*>(m_data) + m_size);
}

inline constexpr bit::memory::memory_block::operator bool()
  const noexcept
{
  return m_data;
}

//----------------------------------------------------------------------------
// Modifiers
//----------------------------------------------------------------------------

inline void bit::memory::memory_block::swap( memory_block& other )
  noexcept
{
  using std::swap;

  swap(m_data, other.m_data);
  swap(m_size, other.m_size);
}

//----------------------------------------------------------------------------
// Free Functions
//----------------------------------------------------------------------------

inline void bit::memory::swap( memory_block& lhs, memory_block& rhs )
  noexcept
{
  lhs.swap(rhs);
}

//----------------------------------------------------------------------------

inline constexpr bool bit::memory::operator == ( const memory_block& lhs,
                                                 const memory_block& rhs )
  noexcept
{
  return lhs.size() == rhs.size() && lhs.data() == rhs.data();
}

inline constexpr bool bit::memory::operator == ( nullblock_t,
                                                 const memory_block& rhs )
  noexcept
{
  return rhs.size() == 0 && rhs.data() == nullptr;
}

inline constexpr bool bit::memory::operator == ( const memory_block& lhs,
                                                 nullblock_t )
  noexcept
{
  return lhs.size() == 0 && lhs.data() == nullptr;
}

//----------------------------------------------------------------------------

inline constexpr bool bit::memory::operator != ( const memory_block& lhs,
                                                 const memory_block& rhs )
  noexcept
{
  return !(lhs==rhs);
}

inline constexpr bool bit::memory::operator != ( nullblock_t,
                                                 const memory_block& rhs )
  noexcept
{
  return !(nullblock==rhs);
}

inline constexpr bool bit::memory::operator != ( const memory_block& lhs,
                                                 nullblock_t )
  noexcept
{
  return !(lhs==nullblock);
}

#endif /* BIT_MEMORY_DETAIL_MEMORY_BLOCK_INL */
