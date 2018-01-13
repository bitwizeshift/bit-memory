#ifndef BIT_MEMORY_ADAPTERS_DETAIL_STANDARD_ALLOCATOR_INL
#define BIT_MEMORY_ADAPTERS_DETAIL_STANDARD_ALLOCATOR_INL

//============================================================================
// standard_allocator
//============================================================================

//----------------------------------------------------------------------------
// Constructors
//----------------------------------------------------------------------------

template<typename T, typename AllocatorStorage>
inline bit::memory::standard_allocator<T,AllocatorStorage>
  ::standard_allocator( AllocatorStorage storage )
  noexcept
  : base_type( std::forward_as_tuple( std::move(storage) ) )
{

}

template<typename T, typename AllocatorStorage>
template<typename U, typename UAllocatorStorage, typename>
inline bit::memory::standard_allocator<T,AllocatorStorage>
  ::standard_allocator( const standard_allocator<U,UAllocatorStorage>& other )
  noexcept
  : base_type( std::forward_as_tuple( other.storage() ) )
{

}

template<typename T, typename AllocatorStorage>
template<typename U, typename UAllocatorStorage, typename>
inline bit::memory::standard_allocator<T,AllocatorStorage>
  ::standard_allocator( standard_allocator<U,UAllocatorStorage>&& other )
  noexcept
  : base_type( std::forward_as_tuple( other.storage() ) )
{

}

//----------------------------------------------------------------------------
// Allocation
//----------------------------------------------------------------------------

template<typename T, typename AllocatorStorage>
inline typename bit::memory::standard_allocator<T,AllocatorStorage>::pointer
  bit::memory::standard_allocator<T,AllocatorStorage>
  ::allocate( size_type n )
{
  auto& allocator = detail::get<0>( *this );

  auto p = alloc_traits::try_allocate( allocator, sizeof(T) * n, alignof(T) );
  return static_cast<pointer>(p);
}

template<typename T, typename AllocatorStorage>
inline void bit::memory::standard_allocator<T,AllocatorStorage>
  ::deallocate( pointer p, size_type n )
{
  auto& allocator = detail::get<0>( *this );

  alloc_traits::deallocate( allocator, static_cast<void_pointer>(p), sizeof(T) * n );
}

//----------------------------------------------------------------------------
// Observers
//----------------------------------------------------------------------------

template<typename T, typename AllocatorStorage>
template<typename,typename>
typename bit::memory::standard_allocator<T,AllocatorStorage>::size_type
  bit::memory::standard_allocator<T,AllocatorStorage>::max_size()
  const noexcept
{
  auto& allocator = detail::get<0>( *this );

  return alloc_traits::max_size( allocator );
}

//----------------------------------------------------------------------------

template<typename T, typename AllocatorStorage>
typename bit::memory::standard_allocator<T,AllocatorStorage>::allocator_type&
  bit::memory::standard_allocator<T,AllocatorStorage>::get_underlying()
  noexcept
{
  auto& storage = detail::get<0>( *this );

  return storage.get_allocator();
}

template<typename T, typename AllocatorStorage>
const typename bit::memory::standard_allocator<T,AllocatorStorage>::allocator_type&
  bit::memory::standard_allocator<T,AllocatorStorage>::get_underlying()
  const noexcept
{
  auto& storage = detail::get<0>( *this );

  return storage.get_allocator();
}

//----------------------------------------------------------------------------
// Private Observers
//----------------------------------------------------------------------------

template<typename T, typename AllocatorStorage>
AllocatorStorage bit::memory::standard_allocator<T,AllocatorStorage>::storage()
  const
{
  return detail::get<0>( *this );
}

//----------------------------------------------------------------------------
// Utilities
//----------------------------------------------------------------------------

template<typename T, typename AllocatorStorage>
inline bit::memory::standard_allocator<T,AllocatorStorage>
  bit::memory::make_standard_allocator( AllocatorStorage allocator )
  noexcept
{
  return standard_allocator<T,AllocatorStorage>( allocator );
}

//----------------------------------------------------------------------------
// Equality Comparisons
//----------------------------------------------------------------------------

template<typename T1, typename T2, typename AllocatorStorage>
inline bool bit::memory
  ::operator==( const standard_allocator<T1,AllocatorStorage>& lhs,
                const standard_allocator<T2,AllocatorStorage>& rhs )
  noexcept
{
  return lhs.get_underlying() == rhs.get_underlying();
}

template<typename T1, typename T2, typename AllocatorStorage>
inline bool bit::memory
  ::operator!=( const standard_allocator<T1,AllocatorStorage>& lhs,
                const standard_allocator<T2,AllocatorStorage>& rhs )
  noexcept
{
  return !(lhs==rhs);
}

#endif /* BIT_MEMORY_ADAPTERS_DETAIL_STANDARD_ALLOCATOR_INL */
