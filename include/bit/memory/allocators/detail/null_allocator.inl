#ifndef BIT_MEMORY_ALLOCATORS_DETAIL_NULL_ALLOCATOR_INL
#define BIT_MEMORY_ALLOCATORS_DETAIL_NULL_ALLOCATOR_INL

//============================================================================
// null_allocator
//============================================================================

//----------------------------------------------------------------------------
// Allocation
//----------------------------------------------------------------------------

inline bit::memory::owner<void*>
  bit::memory::null_allocator::try_allocate( std::size_t size,
                                             std::size_t align,
                                             std::size_t offset )
  noexcept
{
  (void) size;
  (void) align;
  (void) offset;

  return nullptr;
}


inline void bit::memory::null_allocator::deallocate( owner<void*> p,
                                                     std::size_t size )
  noexcept
{
  (void) p;
  (void) size;
}

//----------------------------------------------------------------------------
// Observers
//----------------------------------------------------------------------------

inline bool bit::memory::null_allocator::owns( const void* p )
  const noexcept
{
  return p == nullptr;
}

inline bool bit::memory::null_allocator::owns( std::nullptr_t )
  const noexcept
{
  return true;
}

//----------------------------------------------------------------------------
// Comparisons
//----------------------------------------------------------------------------

inline bool bit::memory::operator==( const null_allocator&,
                                     const null_allocator& )
  noexcept
{
  return true;
}

inline bool bit::memory::operator!=( const null_allocator&,
                                     const null_allocator& )
  noexcept
{
  return false;
}


#endif /* BIT_MEMORY_ALLOCATORS_DETAIL_NULL_ALLOCATOR_INL */
