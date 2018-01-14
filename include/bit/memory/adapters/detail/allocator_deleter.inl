#ifndef BIT_MEMORY_ADAPTERS_DETAIL_ALLOCATOR_DELETER_INL
#define BIT_MEMORY_ADAPTERS_DETAIL_ALLOCATOR_DELETER_INL

//=============================================================================
// allocator_deleter<T,AllocatorStorage>
//=============================================================================

//-----------------------------------------------------------------------------
// Constructors
//-----------------------------------------------------------------------------

template<typename T, typename AllocatorStorage>
inline bit::memory::allocator_deleter<T,AllocatorStorage>
  ::allocator_deleter( AllocatorStorage storage )
  : base_type( std::forward_as_tuple( std::move(storage) ) )
{

}

//-----------------------------------------------------------------------------
// Deallocations
//-----------------------------------------------------------------------------

template<typename T, typename AllocatorStorage>
inline void bit::memory::allocator_deleter<T,AllocatorStorage>::operator()( pointer p )
{
  auto& storage   = get<0>(*this);
  auto& allocator = storage.get_allocator();

  destroy_at( to_raw_pointer(p) );
  alloc_traits::deallocate( allocator, static_cast<void_pointer>(p), sizeof(T) );
}

//=============================================================================
// allocator_deleter<T,Allocator>
//=============================================================================

//-----------------------------------------------------------------------------
// Constructors
//-----------------------------------------------------------------------------

template<typename T, typename AllocatorStorage>
inline bit::memory::allocator_deleter<T[],AllocatorStorage>
  ::allocator_deleter( AllocatorStorage storage, size_type size )
  : base_type( std::forward_as_tuple( std::move(storage) ) ),
    m_size(size)
{

}

//-----------------------------------------------------------------------------
// Deallocations
//-----------------------------------------------------------------------------

template<typename T, typename Allocator>
inline void bit::memory::allocator_deleter<T[],Allocator>::operator()( pointer p )
{
  auto& storage   = get<0>(*this);
  auto& allocator = storage.get_allocator();

  destroy_at( to_raw_pointer(p) );
  alloc_traits::deallocate( allocator, static_cast<void_pointer>(p), sizeof(T) * m_size );
}

#endif /* BIT_MEMORY_ADAPTERS_DETAIL_ALLOCATOR_DELETER_INL */
