#ifndef BIT_MEMORY_ALLOCATOR_STORAGE_DETAIL_REFERENCED_ALLOCATOR_STORAGE_INL
#define BIT_MEMORY_ALLOCATOR_STORAGE_DETAIL_REFERENCED_ALLOCATOR_STORAGE_INL

//=============================================================================
// referenced_allocator_storage
//=============================================================================

//-----------------------------------------------------------------------------
// Constructors / Assignment
//-----------------------------------------------------------------------------

template<typename Allocator>
inline bit::memory::referenced_allocator_storage<Allocator>
  ::referenced_allocator_storage( Allocator& alloc )
  noexcept
  : m_allocator( std::addressof(alloc) )
{

}

//-----------------------------------------------------------------------------
// Observer
//-----------------------------------------------------------------------------

template<typename Allocator>
inline Allocator&
  bit::memory::referenced_allocator_storage<Allocator>::get_allocator()
  noexcept
{
  return *m_allocator;
}

template<typename Allocator>
inline const Allocator&
  bit::memory::referenced_allocator_storage<Allocator>::get_allocator()
  const noexcept
{
  return *m_allocator;
}

//=============================================================================
// Free Functions
//=============================================================================

//-----------------------------------------------------------------------------
// Utilities
//-----------------------------------------------------------------------------

template<typename Allocator>
inline bit::memory::referenced_allocator_storage<Allocator>
  bit::memory::make_referenced_allocator_storage( Allocator& alloc )
  noexcept
{
  return referenced_allocator_storage<Allocator>{ alloc };
}


#endif /* BIT_MEMORY_ALLOCATOR_STORAGE_DETAIL_REFERENCED_ALLOCATOR_STORAGE_INL */
