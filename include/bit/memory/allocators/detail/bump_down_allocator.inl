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

inline bit::memory::owner<void*>
  bit::memory::bump_down_allocator::try_allocate( std::size_t size,
                                                  std::size_t align,
                                                  std::size_t offset )
  noexcept
{
  assert( size && "cannot allocate 0 bytes");
  assert( align && "cannot allocate with 0 alignment");
  assert( is_power_of_two(align) && "alignment must be a power of two" );

  using byte_t = unsigned char;

  auto* p = offset_align_backward( static_cast<byte_t*>(m_current)-size,align,offset);

  // If allocated outside the range, return nullptr
  if( BIT_MEMORY_UNLIKELY( p < m_block.start_address() ) )
    return nullptr;

  // bump the pointer down
  m_current = p;

  return p;
}

//----------------------------------------------------------------------------

inline void bit::memory::bump_down_allocator::deallocate( owner<void*> p,
                                                          std::size_t size )
{
  BIT_MEMORY_UNUSED(p);
  BIT_MEMORY_UNUSED(size);

  assert( m_block.contains( p ) && "Pointer must be contained by block" );
}

//----------------------------------------------------------------------------

inline void bit::memory::bump_down_allocator::deallocate_all()
  noexcept
{
  m_current = m_block.end_address();
}

//----------------------------------------------------------------------------
// Observers
//----------------------------------------------------------------------------

inline bool bit::memory::bump_down_allocator::owns( const void* p )
  const noexcept
{
  return m_block.start_address() <= p && p < m_current;
}

inline bit::memory::allocator_info bit::memory::bump_down_allocator::info()
  const noexcept
{
  return {"bump_down_allocator",this};
}

#endif /* BIT_MEMORY_ALLOCATORS_DETAIL_BUMP_DOWN_ALLOCATOR_INL */
