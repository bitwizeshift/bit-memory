#ifndef BIT_MEMORY_ALLOCATORS_DETAIL_ALIGNED_ALLOCATOR_INL
#define BIT_MEMORY_ALLOCATORS_DETAIL_ALIGNED_ALLOCATOR_INL

//-----------------------------------------------------------------------------
// Allocations / Deallocation
//-----------------------------------------------------------------------------

inline bit::memory::owner<void*>
  bit::memory::aligned_allocator::try_allocate( std::size_t size,
                                                std::size_t align )
  noexcept
{
  return aligned_malloc( size, align );
}

inline void bit::memory::aligned_allocator::deallocate( owner<void*> p,
                                                        std::size_t size )
{
  BIT_MEMORY_UNUSED(size);

  aligned_free( p );
}

//-----------------------------------------------------------------------------
// Observers
//-----------------------------------------------------------------------------

inline bit::memory::allocator_info bit::memory::aligned_allocator::info()
  const noexcept
{
  return {"aligned_allocator",this};
}

//-----------------------------------------------------------------------------
// Equality
//-----------------------------------------------------------------------------

inline bool bit::memory::operator==( const aligned_allocator&,
                                     const aligned_allocator& )
  noexcept
{
  return true;
}

inline bool bit::memory::operator!=( const aligned_allocator&,
                                     const aligned_allocator& )
  noexcept
{
  return false;
}

#endif /* BIT_MEMORY_ALLOCATORS_DETAIL_ALIGNED_ALLOCATOR_INL */
