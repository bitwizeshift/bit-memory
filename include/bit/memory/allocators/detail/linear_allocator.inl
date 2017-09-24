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
                                                          std::size_t align )
  noexcept
{
  assert( size && "cannot allocate 0 bytes");
  assert( align && "cannot allocate with 0 alignment");

  auto this_adjust = std::size_t();

  const auto bound = static_cast<char*>(m_block.data()) + m_block.size();
  auto space       = bound - static_cast<char*>(m_current);
  auto p           = m_current;

  if( std::align( align, size, p, space ) ) {
    m_current = static_cast<char*>(p) + size;
    return p;
  }

  return nullptr;
}

//----------------------------------------------------------------------------

inline void bit::memory::linear_allocator::deallocate( void* p,
                                                       std::size_t size )
{
  assert( m_block.contains( p ) );
}

//----------------------------------------------------------------------------
// Modifiers
//----------------------------------------------------------------------------

inline void bit::memory::linear_allocator::reset()
  noexcept
{
  m_current = m_block.data();
}

#endif /* BIT_MEMORY_ALLOCATORS_DETAIL_LINEAR_ALLOCATOR_INL */
