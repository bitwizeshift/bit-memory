#ifndef BIT_MEMORY_DETAIL_MEMORY_BLOCK_CACHE_INL
#define BIT_MEMORY_DETAIL_MEMORY_BLOCK_CACHE_INL

//=============================================================================
// memory_block_cache
//=============================================================================

//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------

inline bit::memory::memory_block_cache::memory_block_cache()
  noexcept
  : m_head()
{

}

//-----------------------------------------------------------------------------
// Observers
//-----------------------------------------------------------------------------

inline bool bit::memory::memory_block_cache::empty()
  const noexcept
{
  return m_head.data() == nullptr;
}

//-----------------------------------------------------------------------------

inline std::size_t bit::memory::memory_block_cache::size()
  const noexcept
{
  auto result = std::size_t{0};

  auto block = &m_head;
  while( *block != nullblock ) {
    ++result;
    block = static_cast<const memory_block*>(block->data());
  }
  return result;
}

//-----------------------------------------------------------------------------

inline std::size_t bit::memory::memory_block_cache::size_bytes()
  const noexcept
{
  auto result = std::size_t{0};

  auto block = &m_head;
  while( *block != nullblock ) {
    result += block->size();
    block = static_cast<const memory_block*>(block->data());
  }

  return result;
}

//-----------------------------------------------------------------------------

inline bool bit::memory::memory_block_cache::contains( const void* ptr )
  const noexcept
{
  auto block = &m_head;
  while( *block != nullblock ) {
    if( block->contains(ptr) ) return true;
    block = static_cast<const memory_block*>(block->data());
  }
  return false;
}

//-----------------------------------------------------------------------------
// Element Access
//-----------------------------------------------------------------------------

inline const bit::memory::memory_block& bit::memory::memory_block_cache::peek()
  const noexcept
{
  return m_head;
}

//-----------------------------------------------------------------------------

inline bit::memory::owner<bit::memory::memory_block>
  bit::memory::memory_block_cache::request_block()
  noexcept
{
  auto block = m_head;
  if( m_head.data() ) {
    m_head = (*static_cast<memory_block*>(block.data()));
  }
  return block;
}

template<typename BlockAllocator>
inline bit::memory::owner<bit::memory::memory_block>
  bit::memory::memory_block_cache::request_block( BlockAllocator& alloc )
{
  auto block = m_head;
  if( m_head.data() ) {
    m_head = (*static_cast<memory_block*>(block.data()));
  } else {
    block = alloc.allocate_block();
  }
  return block;
}

//-----------------------------------------------------------------------------

inline void bit::memory::memory_block_cache::steal_block( memory_block_cache& other )
  noexcept
{
  if( auto block = other.request_block() ) {
    store_block( block );
  }
}

//-----------------------------------------------------------------------------

inline void bit::memory::memory_block_cache::store_block( owner<memory_block> block )
  noexcept
{
  assert( block != nullblock );
  assert( alignof(memory_block) <= align_of(block.data()) );

  uninitialized_construct_at<memory_block>( block.data(), m_head );
  m_head = block;
}

//-----------------------------------------------------------------------------
// Modifiers
//-----------------------------------------------------------------------------

inline void bit::memory::memory_block_cache::swap( memory_block_cache& other )
  noexcept
{
  m_head.swap(other.m_head);
}

//-----------------------------------------------------------------------------
// Free Functions
//-----------------------------------------------------------------------------

inline void bit::memory::swap( memory_block_cache& lhs, memory_block_cache& rhs )
  noexcept
{
  lhs.swap(rhs);
}

#endif /* BIT_MEMORY_DETAIL_MEMORY_BLOCK_CACHE_INL */
