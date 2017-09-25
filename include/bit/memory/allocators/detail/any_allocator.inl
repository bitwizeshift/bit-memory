#ifndef BIT_MEMORY_ALLOCATORS_DETAIL_ANY_ALLOCATOR_INL
#define BIT_MEMORY_ALLOCATORS_DETAIL_ANY_ALLOCATOR_INL

//============================================================================
// any_allocator
//============================================================================

//----------------------------------------------------------------------------
// Constructors
//----------------------------------------------------------------------------

template<typename Allocator, typename>
inline bit::memory::any_allocator::any_allocator( Allocator&& any_allocator )
  noexcept
  : m_ptr( std::addressof(any_allocator) ),
    m_vtable(vtable_type::get_vtable<Allocator>())
{

}

//----------------------------------------------------------------------------
// Block Allocation
//----------------------------------------------------------------------------

inline bit::memory::owner<void*> bit::memory::any_allocator
  ::allocate( std::size_t size, std::size_t align )
{
  return m_vtable->allocate_fn( m_ptr, size, align );
}

inline bit::memory::owner<void*> bit::memory::any_allocator
  ::try_allocate( std::size_t size, std::size_t align )
  noexcept
{
  return m_vtable->try_allocate_fn( m_ptr, size, align );
}

//----------------------------------------------------------------------------

inline void bit::memory::any_allocator::deallocate( owner<void*> p,
                                                    std::size_t size )
{
  m_vtable->deallocate_fn( m_ptr, p, size );
}

//----------------------------------------------------------------------------
// Capacity
//----------------------------------------------------------------------------

inline std::size_t bit::memory::any_allocator::max_size()
  const noexcept
{
  return m_vtable->max_size_fn( m_ptr );
}

inline std::size_t bit::memory::any_allocator::used()
  const noexcept
{
  return m_vtable->used_fn( m_ptr );
}


#endif /* BIT_MEMORY_ALLOCATORS_DETAIL_ANY_ALLOCATOR_INL */
