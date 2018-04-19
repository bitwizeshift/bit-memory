#ifndef BIT_MEMORY_ALLOCATORS_DETAIL_ALIGNED_OFFSET_ALLOCATOR_INL
#define BIT_MEMORY_ALLOCATORS_DETAIL_ALIGNED_OFFSET_ALLOCATOR_INL

//-----------------------------------------------------------------------------
// Allocations / Deallocation
//-----------------------------------------------------------------------------

inline bit::memory::owner<void*>
  bit::memory::aligned_offset_allocator::try_allocate( std::size_t size,
                                                       std::size_t align,
                                                       std::size_t offset )
  noexcept
{
  return aligned_offset_malloc( size, align, offset );
}

inline void bit::memory::aligned_offset_allocator
  ::deallocate( owner<void*> p,
                std::size_t size )
{
  BIT_MEMORY_UNUSED(size);
  aligned_offset_free( p );
}

//-----------------------------------------------------------------------------
// Observers
//-----------------------------------------------------------------------------

inline bit::memory::allocator_info bit::memory::aligned_offset_allocator::info()
  const noexcept
{
  return {"aligned_offset_allocator",nullptr};
}

#endif /* BIT_MEMORY_ALLOCATORS_DETAIL_ALIGNED_OFFSET_ALLOCATOR_INL */
