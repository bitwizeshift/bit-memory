#ifndef BIT_MEMORY_BLOCK_ALLOCATORS_DETAIL_MALLOC_BLOCK_ALLOCATOR_INL
#define BIT_MEMORY_BLOCK_ALLOCATORS_DETAIL_MALLOC_BLOCK_ALLOCATOR_INL

//-----------------------------------------------------------------------------
// Constructors
//-----------------------------------------------------------------------------

inline bit::memory::malloc_block_allocator::malloc_block_allocator( std::size_t size )
  noexcept
  : m_size( size )
{

}

//-----------------------------------------------------------------------------
// Comparisons
//-----------------------------------------------------------------------------

inline bool bit::memory::operator==( const malloc_block_allocator& lhs,
                                     const malloc_block_allocator& rhs )
  noexcept
{
  return lhs.m_size == rhs.m_size;
}

inline bool bit::memory::operator!=( const malloc_block_allocator& lhs,
                                     const malloc_block_allocator& rhs )
  noexcept
{
  return !(lhs == rhs);
}

#endif /* BIT_MEMORY_BLOCK_ALLOCATORS_DETAIL_MALLOC_BLOCK_ALLOCATOR_INL */
