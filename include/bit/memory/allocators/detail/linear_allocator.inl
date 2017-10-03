#ifndef BIT_MEMORY_ALLOCATORS_DETAIL_LINEAR_ALLOCATOR_INL
#define BIT_MEMORY_ALLOCATORS_DETAIL_LINEAR_ALLOCATOR_INL

//============================================================================
// linear_allocator
//============================================================================

//----------------------------------------------------------------------------
// Constructors
//----------------------------------------------------------------------------

inline bit::memory::linear_allocator::linear_allocator( memory_block block )
  noexcept
  : m_block(block),
    m_current(m_block.data())
{
  assert( m_block && "Block must not be null" );
}

//----------------------------------------------------------------------------
// Allocation / Deallocation
//----------------------------------------------------------------------------

inline void* bit::memory::linear_allocator::try_allocate( std::size_t size,
                                                          std::size_t align,
                                                          std::size_t offset )
  noexcept
{
  assert( size && "cannot allocate 0 bytes");
  assert( align && "cannot allocate with 0 alignment");
  assert( is_power_of_two(align) && "alignment must be a power of two" );

  auto adjust = std::size_t{};
  auto* p = offset_align_forward(m_current,align,offset,&adjust);

  auto* p_end = static_cast<byte*>(p) + size;

  // If allocated outside the range, return nullptr
  if( BIT_MEMORY_UNLIKELY( p_end > m_block.end_address() ) )
    return nullptr;

  // bump the pointer
  m_current = p_end;

  return p;
}

//----------------------------------------------------------------------------

inline void bit::memory::linear_allocator::deallocate( void* p,
                                                       std::size_t size )
{
  assert( m_block.contains( p ) );

  (void) p;
  (void) size;
}

//----------------------------------------------------------------------------

inline void bit::memory::linear_allocator::deallocate_all()
  noexcept
{
  m_current = m_block.data();
}

//----------------------------------------------------------------------------
// Observers
//----------------------------------------------------------------------------

inline bool bit::memory::linear_allocator::owns( void* p )
  const noexcept
{
  return m_block.start_address() <= p && p < m_current;
}

//----------------------------------------------------------------------------
// Comparisons
//----------------------------------------------------------------------------

bool bit::memory::operator==( const linear_allocator& lhs,
                              const linear_allocator& rhs )
  noexcept
{
  return lhs.m_current == rhs.m_current && lhs.m_block == rhs.m_block;
}

bool bit::memory::operator!=( const linear_allocator& lhs,
                              const linear_allocator& rhs )
  noexcept
{
  return !(lhs==rhs);
}

#endif /* BIT_MEMORY_ALLOCATORS_DETAIL_LINEAR_ALLOCATOR_INL */
