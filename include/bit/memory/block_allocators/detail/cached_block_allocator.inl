#ifndef BIT_MEMORY_BLOCK_ALLOCATORS_DETAIL_CACHED_BLOCK_ALLOCATOR_INL
#define BIT_MEMORY_BLOCK_ALLOCATORS_DETAIL_CACHED_BLOCK_ALLOCATOR_INL

//----------------------------------------------------------------------------
// Constructor / Destructor
//----------------------------------------------------------------------------

template<typename BlockAllocator>
template<typename...Args>
inline bit::memory::cached_block_allocator<BlockAllocator>
  ::cached_block_allocator( Args&&...args )
  : m_storage( std::forward<Args>(args)... )
{

}

//----------------------------------------------------------------------------

template<typename BlockAllocator>
inline bit::memory::cached_block_allocator<BlockAllocator>
  ::~cached_block_allocator()
{
  // Deallocate every allocated block
  while( !m_storage.cache.empty() ) {
    m_storage.deallocate_block( m_storage.cache.request_block() );
  }
}

//----------------------------------------------------------------------------
// Block Allocations
//----------------------------------------------------------------------------

template<typename BlockAllocator>
inline bit::memory::owner<bit::memory::memory_block>
  bit::memory::cached_block_allocator<BlockAllocator>::allocate_block()
{
  return m_storage.cache.request_block( m_storage );
}

//----------------------------------------------------------------------------

template<typename BlockAllocator>
inline void bit::memory::cached_block_allocator<BlockAllocator>
  ::deallocate_block( owner<memory_block> block )
{
  m_storage.cache.store_block( std::move(block) );
}

#endif /* BIT_MEMORY_BLOCK_ALLOCATORS_DETAIL_CACHED_BLOCK_ALLOCATOR_INL */
