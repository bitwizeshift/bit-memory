#ifndef BIT_MEMORY_ALLOCATORS_DETAIL_BUMP_UP_LIFO_ALLOCATOR_INL
#define BIT_MEMORY_ALLOCATORS_DETAIL_BUMP_UP_LIFO_ALLOCATOR_INL

//============================================================================
// bump_up_lifo_allocator
//============================================================================

//----------------------------------------------------------------------------
// Constructors
//----------------------------------------------------------------------------

inline bit::memory::bump_up_lifo_allocator::bump_up_lifo_allocator( memory_block block )
  noexcept
  : m_block(block),
    m_current(m_block.data())
{
  assert( m_block && "Block must not be null" );
}

//----------------------------------------------------------------------------
// Allocation / Deallocation
//----------------------------------------------------------------------------

inline void* bit::memory::bump_up_lifo_allocator::try_allocate( std::size_t size,
                                                                std::size_t align,
                                                                std::size_t offset )
  noexcept
{
  assert( size && "cannot allocate 0 bytes");
  assert( align && "cannot allocate with 0 alignment");
  assert( is_power_of_two(align) && "alignment must be a power of two" );

  using byte_t = unsigned char;

  auto adjust = std::size_t{};
  auto* p = offset_align_forward(m_current,align,offset+1,&adjust);

  auto* p_end = static_cast<byte_t*>(p) + size + 1;

  // If allocated outside the range, return nullptr
  if( BIT_MEMORY_UNLIKELY( p_end > m_block.end_address() ) )
    return nullptr;

  auto* byte_ptr = static_cast<byte_t*>(p);
  *byte_ptr      = static_cast<byte_t>(adjust);

  // bump the pointer
  m_current = p_end;

  return byte_ptr + 1;
}

//----------------------------------------------------------------------------

inline void bit::memory::bump_up_lifo_allocator::deallocate( void* p,
                                                             std::size_t size )
{
  BIT_MEMORY_UNUSED(size);

  assert( m_block.contains( p ) && "Pointer must be contained by block" );
  assert( m_current > p && "Deallocations occurred out-of-order" );

  using byte_t = unsigned char;

  // Adjust the pointer
  auto* byte_ptr = static_cast<byte_t*>(p);
  --byte_ptr;

  auto adjust = static_cast<std::size_t>(*byte_ptr);
  byte_ptr -= adjust;

  m_current = byte_ptr;
}

//----------------------------------------------------------------------------

inline void bit::memory::bump_up_lifo_allocator::deallocate_all()
  noexcept
{
  m_current = m_block.data();
}

//----------------------------------------------------------------------------
// Observers
//----------------------------------------------------------------------------

inline bool bit::memory::bump_up_lifo_allocator::owns( const void* p )
  const noexcept
{
  return m_block.start_address() <= p && p < m_current;
}

inline bit::memory::allocator_info bit::memory::bump_up_lifo_allocator::info()
  const noexcept
{
  return {"bump_up_lifo_allocator",this};
}

#endif /* BIT_MEMORY_ALLOCATORS_DETAIL_BUMP_UP_LIFO_ALLOCATOR_INL */
