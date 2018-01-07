#ifndef BIT_MEMORY_BLOCK_ALLOCATOR_STORAGE_DETAIL_SHARED_BLOCK_ALLOCATOR_STORAGE_INL
#define BIT_MEMORY_BLOCK_ALLOCATOR_STORAGE_DETAIL_SHARED_BLOCK_ALLOCATOR_STORAGE_INL

//=============================================================================
// shared_block_allocator_storage
//=============================================================================

//-----------------------------------------------------------------------------
// Constructors
//-----------------------------------------------------------------------------

template<typename BlockAllocator>
bit::memory::shared_block_allocator_storage<BlockAllocator>
  ::shared_block_allocator_storage( std::shared_ptr<BlockAllocator> allocator )
  noexcept
  : m_allocator( std::move(allocator) )
{

}

//-----------------------------------------------------------------------------
// Observers
//-----------------------------------------------------------------------------

template<typename BlockAllocator>
BlockAllocator&
  bit::memory::shared_block_allocator_storage<BlockAllocator>
  ::get_block_allocator()
  noexcept
{
  return *m_allocator;
}

template<typename BlockAllocator>
const BlockAllocator&
  bit::memory::shared_block_allocator_storage<BlockAllocator>
  ::get_block_allocator()
  const noexcept
{
  return *m_allocator;
}

//=============================================================================
// Free Functions
//=============================================================================

//-----------------------------------------------------------------------------
// Utility
//-----------------------------------------------------------------------------

template<typename BlockAllocator, typename...Args>
inline bit::memory::shared_block_allocator_storage<BlockAllocator>
  bit::memory::make_shared_block_allocator_storage( Args&&...args )
{
  auto p = std::make_shared<BlockAllocator>( std::forward<Args>(args)... );
  return shared_block_allocator_storage<BlockAllocator>{ std::move(p) };
}

template<typename BlockAllocator, typename Allocator, typename...Args>
inline bit::memory::shared_block_allocator_storage<BlockAllocator>
  bit::memory::allocate_shared_block_allocator_storage( Allocator& alloc,
                                                        Args&&...args )
{
  auto p = std::allocate_shared<BlockAllocator>( alloc, std::forward<Args>(args)... );
  return shared_block_allocator_storage<BlockAllocator>{ std::move(p) };
}

#endif /* BIT_MEMORY_BLOCK_ALLOCATOR_STORAGE_DETAIL_SHARED_BLOCK_ALLOCATOR_STORAGE_INL */
