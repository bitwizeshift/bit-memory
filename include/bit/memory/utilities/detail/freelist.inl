#ifndef BIT_MEMORY_UTILITIES_DETAIL_FREELIST_INL
#define BIT_MEMORY_UTILITIES_DETAIL_FREELIST_INL

//-----------------------------------------------------------------------------
// Constructors
//-----------------------------------------------------------------------------

inline constexpr bit::memory::freelist::freelist()
  noexcept
  : m_head(nullptr)
{

}

//-----------------------------------------------------------------------------
// Capacity
//-----------------------------------------------------------------------------

inline bool bit::memory::freelist::empty()
  const noexcept
{
  return m_head == nullptr;
}

//-----------------------------------------------------------------------------

inline std::size_t bit::memory::freelist::size()
  const noexcept
{
  auto size = 0u;
  auto p = m_head;
  while( p ) {
    ++size;
    p = (*static_cast<void**>(p));
  }
  return size;
}

//-----------------------------------------------------------------------------
// Modifiers
//-----------------------------------------------------------------------------

inline void bit::memory::freelist::clear()
{
  m_head = nullptr;
}

//-----------------------------------------------------------------------------
// Modifiers
//-----------------------------------------------------------------------------

inline void bit::memory::freelist::swap( freelist& other )
  noexcept
{
  using std::swap;

  swap(m_head,other.m_head);
}

//-----------------------------------------------------------------------------
// Caching
//-----------------------------------------------------------------------------

inline void* bit::memory::freelist::request()
  noexcept
{
  auto p = m_head;
  if( m_head ) m_head = (*static_cast<void**>(m_head));
  return p;
}

//-----------------------------------------------------------------------------

inline void bit::memory::freelist::steal( freelist& other )
  noexcept
{
  auto p = other.request();
  if( p ) store(p);
}

//-----------------------------------------------------------------------------

inline void bit::memory::freelist::store( void* p )
  noexcept
{
  assert( alignof(void*) <= align_of(p) );

  using pointer = void*;

  uninitialized_construct_at<pointer>(p,m_head);
  m_head = p;
}

//-----------------------------------------------------------------------------
// Comparisons
//-----------------------------------------------------------------------------

inline bool bit::memory::operator==( const freelist& lhs, const freelist& rhs )
  noexcept
{
  return lhs.m_head == rhs.m_head;
}

inline bool bit::memory::operator!=( const freelist& lhs, const freelist& rhs )
  noexcept
{
  return !(lhs==rhs);
}

#endif /* BIT_MEMORY_UTILITIES_DETAIL_FREELIST_INL */
