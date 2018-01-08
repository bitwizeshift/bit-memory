#ifndef BIT_MEMORY_ALLOCATOR_STORAGE_DETAIL_SHARED_ALLOCATOR_STORAGE_INL
#define BIT_MEMORY_ALLOCATOR_STORAGE_DETAIL_SHARED_ALLOCATOR_STORAGE_INL

//=============================================================================
// shared_allocator_storage
//=============================================================================

//-----------------------------------------------------------------------------
// Constructors
//-----------------------------------------------------------------------------

template<typename Allocator>
bit::memory::shared_allocator_storage<Allocator>
  ::shared_allocator_storage( std::shared_ptr<Allocator> allocator )
  noexcept
  : m_allocator( std::move(allocator) )
{

}

//-----------------------------------------------------------------------------
// Observers
//-----------------------------------------------------------------------------

template<typename Allocator>
Allocator&
  bit::memory::shared_allocator_storage<Allocator>::get_allocator()
  noexcept
{
  return *m_allocator;
}

template<typename Allocator>
const Allocator&
  bit::memory::shared_allocator_storage<Allocator>::get_allocator()
  const noexcept
{
  return *m_allocator;
}

//=============================================================================
// Free Functions
//=============================================================================

//-----------------------------------------------------------------------------
// Utility
//-----------------------------------------------------------------------------

template<typename Allocator, typename...Args>
inline bit::memory::shared_allocator_storage<Allocator>
  bit::memory::make_shared_allocator_storage( Args&&...args )
{
  auto p = std::make_shared<Allocator>( std::forward<Args>(args)... );
  return shared_allocator_storage<Allocator>{ std::move(p) };
}

template<typename Allocator, typename UAllocator, typename...Args>
inline bit::memory::shared_allocator_storage<Allocator>
  bit::memory::allocate_shared_allocator_storage( UAllocator& alloc,
                                                  Args&&...args )
{
  auto p = std::allocate_shared<Allocator>( alloc, std::forward<Args>(args)... );
  return shared_allocator_storage<Allocator>{ std::move(p) };
}

#endif /* BIT_MEMORY_ALLOCATOR_STORAGE_DETAIL_SHARED_ALLOCATOR_STORAGE_INL */
