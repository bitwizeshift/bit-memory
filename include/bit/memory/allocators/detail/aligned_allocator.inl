#ifndef BIT_MEMORY_ALLOCATORS_DETAIL_ALIGNED_ALLOCATOR_INL
#define BIT_MEMORY_ALLOCATORS_DETAIL_ALIGNED_ALLOCATOR_INL

//-----------------------------------------------------------------------------
// Allocations / Deallocation
//-----------------------------------------------------------------------------

inline void* bit::memory::aligned_allocator::allocate( std::size_t size,
                                                       std::size_t align )
{
  auto p = try_allocate( size, align );

  if( p == nullptr ) {
    get_out_of_memory_handler()({"aligned_allocator",nullptr},size);
  }

  return p;
}

inline void* bit::memory::aligned_allocator::try_allocate( std::size_t size,
                                                           std::size_t align )
  noexcept
{
  return aligned_malloc( size, align );
}

inline void bit::memory::aligned_allocator::deallocate( void* p,
                                                        std::size_t size )
{
  (void) size;
  aligned_free( p );
}

//-----------------------------------------------------------------------------
// Comparators
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
