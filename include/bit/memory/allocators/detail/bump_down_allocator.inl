#ifndef BIT_MEMORY_ALLOCATORS_DETAIL_BUMP_DOWN_ALLOCATOR_INL
#define BIT_MEMORY_ALLOCATORS_DETAIL_BUMP_DOWN_ALLOCATOR_INL

//============================================================================
// bump_down_allocator
//============================================================================

//----------------------------------------------------------------------------
// Constructors
//----------------------------------------------------------------------------

inline bit::memory::bump_down_allocator::bump_down_allocator( memory_block block )
  noexcept
  : m_block(block),
    m_current(m_block.end_address())
{
  assert( m_block && "Block must not be null" );
}

//----------------------------------------------------------------------------
// Allocation / Deallocation
//----------------------------------------------------------------------------

inline void* bit::memory::bump_down_allocator::try_allocate( std::size_t size,
                                                             std::size_t align,
                                                             std::size_t offset )
  noexcept
{
  assert( size && "cannot allocate 0 bytes");
  assert( align && "cannot allocate with 0 alignment");
  assert( is_power_of_two(align) && "alignment must be a power of two" );

  auto adjust = std::size_t{};
  auto* p = offset_align_backward(m_current,align,offset,&adjust);

  // If allocated outside the range, return nullptr
  if( BIT_MEMORY_UNLIKELY( p < m_block.start_address() ) )
    return nullptr;

  // bump the pointer down
  m_current = p;

  return p;
}

//----------------------------------------------------------------------------

inline void bit::memory::bump_down_allocator::deallocate( void* p,
                                                        std::size_t size )
{
  BIT_MEMORY_UNUSED(size);

  assert( m_block.contains( p ) && "Pointer must be contained by block" );
  assert( m_current < p && "Deallocations occurred out-of-order" );

  m_current = p;
}

//----------------------------------------------------------------------------

inline void bit::memory::bump_down_allocator::deallocate_all()
  noexcept
{
  m_current = m_block.data();
}

//----------------------------------------------------------------------------
// Observers
//----------------------------------------------------------------------------

inline bool bit::memory::bump_down_allocator::owns( void* p )
  const noexcept
{
  return m_block.start_address() <= p && p < m_current;
}

//----------------------------------------------------------------------------
// Comparisons
//----------------------------------------------------------------------------

bool bit::memory::operator==( const bump_down_allocator& lhs,
                              const bump_down_allocator& rhs )
  noexcept
{
  return lhs.m_current == rhs.m_current && lhs.m_block == rhs.m_block;
}

bool bit::memory::operator!=( const bump_down_allocator& lhs,
                              const bump_down_allocator& rhs )
  noexcept
{
  return !(lhs==rhs);
}

#endif /* BIT_MEMORY_ALLOCATORS_DETAIL_BUMP_DOWN_ALLOCATOR_INL */
