#ifndef BIT_MEMORY_BLOCK_ALLOCATORS_DETAIL_CACHED_BLOCK_ALLOCATOR_INL
#define BIT_MEMORY_BLOCK_ALLOCATORS_DETAIL_CACHED_BLOCK_ALLOCATOR_INL

//-----------------------------------------------------------------------------
// Constructor / Destructor
//-----------------------------------------------------------------------------

template<typename BlockAllocator>
template<typename Arg0, typename...Args, typename>
inline bit::memory::detail::cached_block_allocator<BlockAllocator>
  ::cached_block_allocator( Arg0&& arg0, Args&&...args )
  : BlockAllocator( std::forward<Arg0>(arg0), std::forward<Args>(args)... )
{

}

//-----------------------------------------------------------------------------

template<typename BlockAllocator>
inline bit::memory::detail::cached_block_allocator<BlockAllocator>
  ::~cached_block_allocator()
{
  // Deallocate every allocated block
  while( !m_cache.empty() ) {
    BlockAllocator::deallocate_block( m_cache.request_block() );
  }
}

//-----------------------------------------------------------------------------
// Block Allocations
//-----------------------------------------------------------------------------

template<typename BlockAllocator>
inline bit::memory::owner<bit::memory::memory_block>
  bit::memory::detail::cached_block_allocator<BlockAllocator>::allocate_block()
{
  return m_cache.request_block( static_cast<BlockAllocator&>(*this) );
}

//-----------------------------------------------------------------------------

template<typename BlockAllocator>
inline void bit::memory::detail::cached_block_allocator<BlockAllocator>
  ::deallocate_block( owner<memory_block> block )
{
  m_cache.store_block( std::move(block) );
}

//-----------------------------------------------------------------------------
// Block Allocations
//-----------------------------------------------------------------------------

template<typename BlockAllocator>
inline std::size_t bit::memory::detail::cached_block_allocator<BlockAllocator>
  ::next_block_size()
  const noexcept
{
  if( !m_cache.empty() ) {
    return m_cache.peek().size();
  }
  return BlockAllocator::next_block_size();
}

#endif /* BIT_MEMORY_BLOCK_ALLOCATORS_DETAIL_CACHED_BLOCK_ALLOCATOR_INL */
