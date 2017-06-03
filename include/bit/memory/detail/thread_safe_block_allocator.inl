#ifndef BIT_MEMORY_DETAIL_THREAD_SAFE_BLOCK_ALLOCATOR_INL
#define BIT_MEMORY_DETAIL_THREAD_SAFE_BLOCK_ALLOCATOR_INL

//----------------------------------------------------------------------------
// Constructors
//----------------------------------------------------------------------------

template<typename BlockAllocator, typename Lockable>
template<typename...Args,decltype(BlockAllocator( std::declval<Args>()... ), void())*>
bit::memory::thread_safe_block_allocator<BlockAllocator,Lockable>
  ::thread_safe_block_allocator( Args&&...args )
  : m_storage( std::forward<Args>(args)... )
{

}

//----------------------------------------------------------------------------
// Block Allocations
//----------------------------------------------------------------------------

template<typename BlockAllocator, typename Lockable>
bit::memory::owner<bit::memory::memory_block>
  bit::memory::thread_safe_block_allocator<BlockAllocator,Lockable>::allocate_block()
{
  std::lock_guard<Lockable> guard(m_storage.lock);

  return m_storage.allocate_block();
}

//----------------------------------------------------------------------------

template<typename BlockAllocator, typename Lockable>
void bit::memory::thread_safe_block_allocator<BlockAllocator,Lockable>
  ::deallocate_block( owner<memory_block> block )
{
  std::lock_guard<Lockable> guard(m_storage.lock);

  return m_storage.deallocate_block( block );
}

#endif /* BIT_MEMORY_DETAIL_THREAD_SAFE_BLOCK_ALLOCATOR_INL */
