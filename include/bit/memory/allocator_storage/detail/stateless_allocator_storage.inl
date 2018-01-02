#ifndef BIT_MEMORY_ALLOCATOR_STORAGE_DETAIL_STATELESS_ALLOCATOR_STORAGE_INL
#define BIT_MEMORY_ALLOCATOR_STORAGE_DETAIL_STATELESS_ALLOCATOR_STORAGE_INL

//=============================================================================
// stateless_allocator_storage
//=============================================================================

//-----------------------------------------------------------------------------
// Observers
//-----------------------------------------------------------------------------

template<typename StatelessAllocator>
inline StatelessAllocator&
  bit::memory::stateless_allocator_storage<StatelessAllocator>::get_allocator()
  noexcept
{
  return detail::get<0>(*this);
}

template<typename StatelessAllocator>
inline const StatelessAllocator&
  bit::memory::stateless_allocator_storage<StatelessAllocator>::get_allocator()
  const noexcept
{
  return detail::get<0>(*this);
}

//=============================================================================
// Free Functions
//=============================================================================

//-----------------------------------------------------------------------------
// Utilities
//-----------------------------------------------------------------------------

template<typename Allocator>
inline bit::memory::stateless_allocator_storage<Allocator>
  bit::memory::make_stateless_allocator_storage()
{
  return {};
}

#endif /* BIT_MEMORY_ALLOCATOR_STORAGE_DETAIL_STATELESS_ALLOCATOR_STORAGE_INL */
