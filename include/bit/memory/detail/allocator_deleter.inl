#ifndef BIT_MEMORY_DETAIL_ALLOCATOR_DELETER_INL
#define BIT_MEMORY_DETAIL_ALLOCATOR_DELETER_INL

//=============================================================================
// allocator_deleter<T,Allocator>
//=============================================================================

//-----------------------------------------------------------------------------
// Constructors
//-----------------------------------------------------------------------------

template<typename T, typename Allocator>
inline bit::memory::allocator_deleter<T,Allocator>
  ::allocator_deleter( Allocator& alloc )
  : base_type( std::forward_as_tuple(alloc) )
{

}

//-----------------------------------------------------------------------------
// Deallocations
//-----------------------------------------------------------------------------

template<typename T, typename Allocator>
inline void bit::memory::allocator_deleter<T,Allocator>::operator()( pointer p )
{
  auto& allocator = detail::get<0>(*this);

  destroy_at( pointer_traits::pointer_to(p) );
  alloc_traits::deallocate( allocator, static_cast<void_pointer>(p), sizeof(T) );
}

//=============================================================================
// allocator_deleter<T,Allocator>
//=============================================================================

//-----------------------------------------------------------------------------
// Constructors
//-----------------------------------------------------------------------------

template<typename T, typename Allocator>
inline bit::memory::allocator_deleter<T[],Allocator>
  ::allocator_deleter( Allocator& alloc, size_type size )
  : base_type( std::forward_as_tuple(alloc) ),
    m_size(size)
{

}

//-----------------------------------------------------------------------------
// Deallocations
//-----------------------------------------------------------------------------

template<typename T, typename Allocator>
inline void bit::memory::allocator_deleter<T[],Allocator>::operator()( pointer p )
{
  auto& allocator = detail::get<0>(*this);

  destroy_at( pointer_traits::pointer_to(p) );
  alloc_traits::deallocate( allocator, static_cast<void_pointer>(p), sizeof(T) * m_size );
}

#endif /* BIT_MEMORY_DETAIL_ALLOCATOR_DELETER_INL */
