#ifndef BIT_MEMORY_ALLOCATORS_DETAIL_NEW_ALLOCATOR_INL
#define BIT_MEMORY_ALLOCATORS_DETAIL_NEW_ALLOCATOR_INL

inline bit::memory::owner<void*>
  bit::memory::new_allocator::try_allocate( std::size_t size,
                                            std::size_t align )
  noexcept
{
  BIT_MEMORY_UNUSED(align);

  return ::operator new( size, std::nothrow );
}

inline void bit::memory::new_allocator::deallocate( owner<void*> p,
                                                    std::size_t size )
{
  BIT_MEMORY_UNUSED(size);

  ::operator delete(p);
}


//-----------------------------------------------------------------------------
// Observers
//-----------------------------------------------------------------------------

inline bit::memory::allocator_info bit::memory::new_allocator::info()
  const noexcept
{
  return {"new_allocator",this};
}

//-----------------------------------------------------------------------------
// Equality
//-----------------------------------------------------------------------------

inline bool bit::memory::operator==( const new_allocator&,
                                     const new_allocator& )
  noexcept
{
  return true;
}

inline bool bit::memory::operator!=( const new_allocator&,
                                     const new_allocator& )
  noexcept
{
  return false;
}

#endif /* BIT_MEMORY_ALLOCATORS_DETAIL_NEW_ALLOCATOR_INL */
