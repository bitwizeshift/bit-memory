#ifndef BIT_MEMORY_BLOCK_ALLOCATORS_DETAIL_NULL_BLOCK_ALLOCATOR_INL
#define BIT_MEMORY_BLOCK_ALLOCATORS_DETAIL_NULL_BLOCK_ALLOCATOR_INL

//-----------------------------------------------------------------------------
// Block Allocations
//-----------------------------------------------------------------------------

inline bit::memory::owner<bit::memory::memory_block>
  bit::memory::null_block_allocator::allocate_block()
  noexcept
{
  return memory_block{ nullptr, 0 };
}

inline void bit::memory::null_block_allocator
  ::deallocate_block( owner<memory_block> )
  noexcept
{
  // Do nothing
}

//-----------------------------------------------------------------------------
// Observers
//-----------------------------------------------------------------------------

std::size_t bit::memory::null_block_allocator::next_block_size()
  const noexcept
{
  return 0;
}

//-----------------------------------------------------------------------------
// Comparisons
//-----------------------------------------------------------------------------

bool bit::memory::operator==( const null_block_allocator&,
                              const null_block_allocator& )
  noexcept
{
  return true;
}

bool bit::memory::operator!=( const null_block_allocator&,
                              const null_block_allocator& )
  noexcept
{
  return false;
}

#endif /* BIT_MEMORY_BLOCK_ALLOCATORS_DETAIL_NULL_BLOCK_ALLOCATOR_INL */
