#ifndef BIT_MEMORY_DETAIL_NULL_ALLOCATOR_INL
#define BIT_MEMORY_DETAIL_NULL_ALLOCATOR_INL

//============================================================================
// null_allocator
//============================================================================

//----------------------------------------------------------------------------
// Allocation
//----------------------------------------------------------------------------

inline void* bit::memory::null_allocator
  ::allocate( std::size_t, std::size_t, std::size_t )
  noexcept
{
  return nullptr;
}

inline void bit::memory::null_allocator::deallocate( void*, std::size_t )
  noexcept
{

}

#endif /* BIT_MEMORY_DETAIL_NULL_ALLOCATOR_INL */
