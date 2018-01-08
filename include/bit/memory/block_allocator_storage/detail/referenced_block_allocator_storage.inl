#ifndef BIT_MEMORY_BLOCK_ALLOCATOR_STORAGE_DETAIL_REFERENCED_BLOCK_ALLOCATOR_STORAGE_INL
#define BIT_MEMORY_BLOCK_ALLOCATOR_STORAGE_DETAIL_REFERENCED_BLOCK_ALLOCATOR_STORAGE_INL

//=============================================================================
// referenced_block_allocator_storage
//=============================================================================

//-----------------------------------------------------------------------------
// Constructors / Assignment
//-----------------------------------------------------------------------------

template<typename BlockAllocator>
inline bit::memory::referenced_block_allocator_storage<BlockAllocator>
  ::referenced_block_allocator_storage( BlockAllocator& alloc )
  noexcept
  : m_allocator( std::addressof(alloc) )
{

}

//-----------------------------------------------------------------------------
// Observer
//-----------------------------------------------------------------------------

template<typename BlockAllocator>
inline BlockAllocator&
  bit::memory::referenced_block_allocator_storage<BlockAllocator>
  ::get_block_allocator()
  noexcept
{
  return *m_allocator;
}

template<typename BlockAllocator>
inline const BlockAllocator&
  bit::memory::referenced_block_allocator_storage<BlockAllocator>
  ::get_block_allocator()
  const noexcept
{
  return *m_allocator;
}

//=============================================================================
// Free Functions
//=============================================================================

//-----------------------------------------------------------------------------
// Utilities
//-----------------------------------------------------------------------------

template<typename Allocator>
inline bit::memory::referenced_block_allocator_storage<Allocator>
  bit::memory::make_referenced_block_allocator_storage( Allocator& alloc )
  noexcept
{
  return referenced_block_allocator_storage<Allocator>{ alloc };
}


#endif /* BIT_MEMORY_BLOCK_ALLOCATOR_STORAGE_DETAIL_REFERENCED_BLOCK_ALLOCATOR_STORAGE_INL */
