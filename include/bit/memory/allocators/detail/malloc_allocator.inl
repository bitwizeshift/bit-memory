#ifndef BIT_MEMORY_ALLOCATORS_DETAIL_MALLOC_ALLOCATOR_INL
#define BIT_MEMORY_ALLOCATORS_DETAIL_MALLOC_ALLOCATOR_INL

//-----------------------------------------------------------------------------
// Allocation / Deallocation
//-----------------------------------------------------------------------------

inline bit::memory::owner<void*>
  bit::memory::malloc_allocator::try_allocate( std::size_t size,
                                               std::size_t align )
  noexcept
{
  BIT_MEMORY_UNUSED(align);

  return std::malloc( size );
}

//-----------------------------------------------------------------------------

inline void bit::memory::malloc_allocator::deallocate( owner<void*> p,
                                                       std::size_t size )
{
  BIT_MEMORY_UNUSED(size);

  std::free( p );
}

//-----------------------------------------------------------------------------
// Observers
//-----------------------------------------------------------------------------

inline bit::memory::allocator_info bit::memory::malloc_allocator::info()
  const noexcept
{
  return {"malloc_allocator",this};
}

#endif /* BIT_MEMORY_ALLOCATORS_DETAIL_MALLOC_ALLOCATOR_INL */
