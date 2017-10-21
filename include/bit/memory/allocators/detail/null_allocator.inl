#ifndef BIT_MEMORY_ALLOCATORS_DETAIL_NULL_ALLOCATOR_INL
#define BIT_MEMORY_ALLOCATORS_DETAIL_NULL_ALLOCATOR_INL

//=============================================================================
// null_allocator
//=============================================================================

//-----------------------------------------------------------------------------
// Allocation
//-----------------------------------------------------------------------------

inline bit::memory::owner<void*>
  bit::memory::null_allocator::try_allocate( std::size_t size,
                                             std::size_t align,
                                             std::size_t offset )
  noexcept
{
  BIT_MEMORY_UNUSED(size);
  BIT_MEMORY_UNUSED(align);
  BIT_MEMORY_UNUSED(offset);

  return nullptr;
}


inline void bit::memory::null_allocator::deallocate( owner<void*> p,
                                                     std::size_t size )
  noexcept
{
  BIT_MEMORY_UNUSED(p);
  BIT_MEMORY_UNUSED(size);
}

//-----------------------------------------------------------------------------
// Observers
//-----------------------------------------------------------------------------

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

//-----------------------------------------------------------------------------

inline bit::memory::allocator_info bit::memory::null_allocator::info()
  const noexcept
{
  return {"null_allocator",this};
}

//-----------------------------------------------------------------------------
// Equality
//-----------------------------------------------------------------------------

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
