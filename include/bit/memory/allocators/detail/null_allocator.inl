#ifndef BIT_MEMORY_ALLOCATORS_DETAIL_NULL_ALLOCATOR_INL
#define BIT_MEMORY_ALLOCATORS_DETAIL_NULL_ALLOCATOR_INL

//============================================================================
// null_allocator
//============================================================================

//----------------------------------------------------------------------------
// Allocation
//----------------------------------------------------------------------------

inline void* bit::memory::null_allocator::allocate( std::size_t size,
                                                    std::size_t align )
{
  (void) align;

  get_out_of_memory_handler()({"null_allocator",nullptr}, size);

  return nullptr;
}

inline void* bit::memory::null_allocator::try_allocate( std::size_t size,
                                                        std::size_t align )
  noexcept
{
  (void) size;
  (void) align;

  return nullptr;
}


inline void bit::memory::null_allocator::deallocate( void* p,
                                                     std::size_t size )
  noexcept
{
  (void) p;
  (void) size;
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
