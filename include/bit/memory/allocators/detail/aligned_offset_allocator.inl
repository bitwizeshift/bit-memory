#ifndef BIT_MEMORY_ALLOCATORS_DETAIL_ALIGNED_OFFSET_ALLOCATOR_INL
#define BIT_MEMORY_ALLOCATORS_DETAIL_ALIGNED_OFFSET_ALLOCATOR_INL

//-----------------------------------------------------------------------------
// Allocations / Deallocation
//-----------------------------------------------------------------------------

inline void* bit::memory::aligned_offset_allocator
  ::try_allocate( std::size_t size,
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
  (void) size;
  aligned_offset_free( p );
}

//-----------------------------------------------------------------------------
// Comparators
//-----------------------------------------------------------------------------

inline bool bit::memory::operator==( const aligned_offset_allocator&,
                                     const aligned_offset_allocator& )
  noexcept
{
  return true;
}

inline bool bit::memory::operator!=( const aligned_offset_allocator&,
                                     const aligned_offset_allocator& )
  noexcept
{
  return false;
}

#endif /* BIT_MEMORY_ALLOCATORS_DETAIL_ALIGNED_OFFSET_ALLOCATOR_INL */
