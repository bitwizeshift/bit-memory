#ifndef BIT_MEMORY_BLOCK_ALLOCATORS_DETAIL_NEW_BLOCK_ALLOCATOR_INL
#define BIT_MEMORY_BLOCK_ALLOCATORS_DETAIL_NEW_BLOCK_ALLOCATOR_INL

//-----------------------------------------------------------------------------
// Constructors
//-----------------------------------------------------------------------------

inline bit::memory::new_block_allocator::new_block_allocator( std::size_t size )
  noexcept
  : m_size( size )
{

}

//-----------------------------------------------------------------------------
// Comparisons
//-----------------------------------------------------------------------------

inline bool bit::memory::operator==( const new_block_allocator& lhs,
                                     const new_block_allocator& rhs )
  noexcept
{
  return lhs.m_size == rhs.m_size;
}

inline bool bit::memory::operator!=( const new_block_allocator& lhs,
                                     const new_block_allocator& rhs )
  noexcept
{
  return !(lhs == rhs);
}

#endif /* BIT_MEMORY_BLOCK_ALLOCATORS_DETAIL_NEW_BLOCK_ALLOCATOR_INL */
