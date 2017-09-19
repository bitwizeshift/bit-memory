#ifndef BIT_MEMORY_DETAIL_ALLOCATOR_INFO_INL
#define BIT_MEMORY_DETAIL_ALLOCATOR_INFO_INL

//============================================================================
// allocator_info
//============================================================================

//----------------------------------------------------------------------------
// Constructors
//----------------------------------------------------------------------------

inline bit::memory::allocator_info
  ::allocator_info( const char* name, const void* allocator )
  : m_name(name),
    m_address(allocator)
{

}

//----------------------------------------------------------------------------
// Observers
//----------------------------------------------------------------------------

inline const char* bit::memory::allocator_info::name()
  const noexcept
{
  return m_name;
}

inline const void* bit::memory::allocator_info::address()
  const noexcept
{
  return m_address;
}

//----------------------------------------------------------------------------
// Comparisons
//----------------------------------------------------------------------------

inline bool bit::memory::operator==( const allocator_info& lhs,
                                     const allocator_info& rhs )
  noexcept
{
  return lhs.name() == rhs.name() && lhs.address() == rhs.address();
}

inline bool bit::memory::operator!=( const allocator_info& lhs,
                                     const allocator_info& rhs )
  noexcept
{
  return !(lhs==rhs);
}




#endif /* BIT_MEMORY_DETAIL_ALLOCATOR_INFO_INL */
