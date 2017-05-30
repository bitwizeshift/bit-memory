#ifndef BIT_MEMORY_DETAIL_NULL_BLOCK_ALLOCATOR_INL
#define BIT_MEMORY_DETAIL_NULL_BLOCK_ALLOCATOR_INL

//----------------------------------------------------------------------------
// Block Allocations
//----------------------------------------------------------------------------

inline bit::memory::owner<bit::memory::memory_block>
  bit::memory::null_block_allocator::allocate_block()
  noexcept
{
  return nullblock;
}

inline void bit::memory::null_block_allocator
  ::deallocate_block( owner<memory_block> )
  noexcept
{
  // Do nothing
}

#endif /* BIT_MEMORY_DETAIL_NULL_BLOCK_ALLOCATOR_INL */
