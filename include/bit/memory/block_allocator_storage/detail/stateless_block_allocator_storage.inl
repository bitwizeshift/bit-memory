#ifndef BIT_MEMORY_BLOCK_ALLOCATOR_STORAGE_DETAIL_STATELESS_BLOCK_ALLOCATOR_STORAGE_INL
#define BIT_MEMORY_BLOCK_ALLOCATOR_STORAGE_DETAIL_STATELESS_BLOCK_ALLOCATOR_STORAGE_INL

//=============================================================================
// stateless_block_allocator_storage
//=============================================================================

//-----------------------------------------------------------------------------
// Observers
//-----------------------------------------------------------------------------

template<typename StatelessBlockAllocator>
inline StatelessBlockAllocator&
  bit::memory::stateless_block_allocator_storage<StatelessBlockAllocator>
  ::get_block_allocator()
  noexcept
{
  return get<0>(*this);
}

template<typename StatelessBlockAllocator>
inline const StatelessBlockAllocator&
  bit::memory::stateless_block_allocator_storage<StatelessBlockAllocator>
  ::get_block_allocator()
  const noexcept
{
  return get<0>(*this);
}

//=============================================================================
// Free Functions
//=============================================================================

//-----------------------------------------------------------------------------
// Utilities
//-----------------------------------------------------------------------------

template<typename Allocator>
inline bit::memory::stateless_block_allocator_storage<Allocator>
  bit::memory::make_stateless_block_allocator_storage()
{
  return {};
}

#endif /* BIT_MEMORY_BLOCK_ALLOCATOR_STORAGE_DETAIL_STATELESS_BLOCK_ALLOCATOR_STORAGE_INL */
