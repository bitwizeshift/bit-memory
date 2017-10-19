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
    m_vtable(vtable_type::get_vtable<std::decay_t<Allocator>>())
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

inline bit::memory::allocator_info bit::memory::any_allocator::info()
  const noexcept
{
  return m_vtable->info_fn( m_ptr );
}

//----------------------------------------------------------------------------
// Equality Comparison
//----------------------------------------------------------------------------

inline bool bit::memory::operator==( const any_allocator& lhs,
                                     const any_allocator& rhs )
  noexcept
{
  return lhs.m_vtable == rhs.m_vtable &&
         lhs.m_ptr == rhs.m_ptr;
}

inline bool bit::memory::operator!=( const any_allocator& lhs,
                                     const any_allocator& rhs )
  noexcept
{
  return !(lhs==rhs);
}

#endif /* BIT_MEMORY_ALLOCATORS_DETAIL_ANY_ALLOCATOR_INL */
