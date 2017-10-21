#ifndef BIT_MEMORY_ALLOCATORS_DETAIL_POOL_ALLOCATOR_INL
#define BIT_MEMORY_ALLOCATORS_DETAIL_POOL_ALLOCATOR_INL

//-----------------------------------------------------------------------------
// Constructors / Assignment
//-----------------------------------------------------------------------------

inline bit::memory::pool_allocator::pool_allocator( std::size_t chunk_size,
                                                    memory_block block )
  : m_block(block),
    m_chunk_size(chunk_size)
{
  // It is a requirement that chunk_size is a power-of-2 that is greater
  // than alignof(void*) and sizeof(void*) -- otherwise the pool would
  // suffer misalignment issues on the internal freelist
  assert( is_power_of_two(chunk_size) );
  assert( chunk_size >= sizeof(void*) );
  assert( chunk_size >= alignof(void*) );
  assert( chunk_size <= m_block.size() );

  using byte_t = unsigned char;

  auto* p = m_block.data();
  const auto chunks = m_block.size() / chunk_size;

  // Store each entry in the freelist in reverse order
  for( auto i = chunks-1; i != 0; --i ) {
    m_freelist.store( static_cast<byte_t*>(p) + (i*chunk_size) );
  }
}

//-----------------------------------------------------------------------------
// Allocation / Deallocation
//-----------------------------------------------------------------------------

inline bit::memory::owner<void*>
  bit::memory::pool_allocator::try_allocate( std::size_t size,
                                             std::size_t align,
                                             std::size_t offset )
  noexcept
{
  using byte_t = unsigned char;

  auto p      = m_freelist.request();
  auto adjust = std::size_t{};
  p           = offset_align_forward(p, align, offset+1, &adjust);

  const auto new_size = (size + 1 + adjust);

  if( BIT_MEMORY_UNLIKELY(new_size > max_size()) ) return nullptr;

  // Store the adjustment made to align correctly
  *static_cast<byte_t*>(p) = static_cast<byte_t>(adjust);

  return static_cast<char*>(p) + 1;
}

inline void bit::memory::pool_allocator::deallocate( owner<void*> p,
                                                     std::size_t size )
{
  BIT_MEMORY_UNUSED(size);

  using byte_t = unsigned char;

  p           = static_cast<byte_t*>(p) - 1;
  auto adjust = static_cast<std::ptrdiff_t>(*static_cast<byte_t*>(p));
  p           = static_cast<byte_t*>(p) - adjust;

  m_freelist.store( p );
}

//-----------------------------------------------------------------------------
// Observers
//-----------------------------------------------------------------------------

inline bool bit::memory::pool_allocator::owns( const void* p )
  const noexcept
{
  return m_block.contains(p);
}

inline std::size_t bit::memory::pool_allocator::max_size()
  const noexcept
{
  return m_chunk_size;
}

//-----------------------------------------------------------------------------

inline bit::memory::allocator_info bit::memory::pool_allocator::info()
  const noexcept
{
  return {"pool_allocator",this};
}

//-----------------------------------------------------------------------------
// Comparison
//-----------------------------------------------------------------------------

inline bool bit::memory::operator==( const pool_allocator& lhs,
                                     const pool_allocator& rhs )
  noexcept
{
  return lhs.m_freelist == rhs.m_freelist &&
         lhs.m_block == rhs.m_block &&
         lhs.m_chunk_size == rhs.m_chunk_size;
}

inline bool bit::memory::operator!=( const pool_allocator& lhs,
                                     const pool_allocator& rhs )
  noexcept
{
  return !(lhs==rhs);
}

#endif /* BIT_MEMORY_ALLOCATORS_DETAIL_POOL_ALLOCATOR_INL */
