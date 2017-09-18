#ifndef BIT_MEMORY_DETAIL_NEW_ALLOCATOR_INL
#define BIT_MEMORY_DETAIL_NEW_ALLOCATOR_INL

inline void* bit::memory::new_allocator::allocate( std::size_t size,
                                                   std::size_t align )
{
  auto p = try_allocate( size, align );

  if( p == nullptr ) {
    out_of_memory_handler({"new_allocator",nullptr},size);
  }
  return p;
}

inline void* bit::memory::new_allocator::try_allocate( std::size_t size,
                                                       std::size_t align )
  noexcept
{
  (void) align;

  return ::operator new( size, std::nothrow );
}

inline void bit::memory::new_allocator::deallocate( void* p,
                                                    std::size_t size )
{
  (void) size;

  ::operator delete(p);
}

#endif /* BIT_MEMORY_DETAIL_NEW_ALLOCATOR_INL */
