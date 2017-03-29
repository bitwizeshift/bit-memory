#ifndef BIT_MEMORY_DETAIL_MEMORY_BLOCK_CACHE_INL
#define BIT_MEMORY_DETAIL_MEMORY_BLOCK_CACHE_INL

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

#endif /* BIT_MEMORY_DETAIL_MEMORY_BLOCK_CACHE_INL */
