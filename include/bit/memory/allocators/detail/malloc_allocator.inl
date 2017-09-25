#ifndef BIT_MEMORY_ALLOCATORS_DETAIL_MALLOC_ALLOCATOR_INL
#define BIT_MEMORY_ALLOCATORS_DETAIL_MALLOC_ALLOCATOR_INL

//-----------------------------------------------------------------------------
// Allocation / Deallocation
//-----------------------------------------------------------------------------

inline bit::memory::owner<void*>
  bit::memory::malloc_allocator::allocate( std::size_t size,
                                           std::size_t align )
{
  auto p = try_allocate(size,align);

  if( p == nullptr ) {
    get_out_of_memory_handler()( {"malloc_allocator", nullptr}, size );
  }

  return p;
}

inline bit::memory::owner<void*>
  bit::memory::malloc_allocator::try_allocate( std::size_t size,
                                               std::size_t align )
  noexcept
{
  (void) align;

  return std::malloc( size );
}

//-----------------------------------------------------------------------------

inline void bit::memory::malloc_allocator::deallocate( owner<void*> p,
                                                       std::size_t size )
{
  (void) size;

  std::free( p );
}

//-----------------------------------------------------------------------------
// Comparisons
//-----------------------------------------------------------------------------

inline bool bit::memory::operator==( const malloc_allocator&,
                                     const malloc_allocator& )
  noexcept
{
  return true;
}

inline bool bit::memory::operator!=( const malloc_allocator&,
                                     const malloc_allocator& )
  noexcept
{
  return false;
}

#endif /* BIT_MEMORY_ALLOCATORS_DETAIL_MALLOC_ALLOCATOR_INL */
