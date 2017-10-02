#ifndef BIT_MEMORY_DETAIL_STD_ALLOCATOR_ADAPTER_INL
#define BIT_MEMORY_DETAIL_STD_ALLOCATOR_ADAPTER_INL

//============================================================================
// std_allocator_adapter
//============================================================================

//----------------------------------------------------------------------------
// Constructors
//----------------------------------------------------------------------------

template<typename T, typename Allocator>
inline bit::memory::std_allocator_adapter<T,Allocator>
  ::std_allocator_adapter( Allocator& allocator )
  noexcept
  : base_type( allocator )
{

}

template<typename T, typename Allocator>
template<typename U>
inline bit::memory::std_allocator_adapter<T,Allocator>
  ::std_allocator_adapter( const std_allocator_adapter<U,Allocator>& other )
  noexcept
  : base_type( other.get() )
{

}

template<typename T, typename Allocator>
template<typename U>
inline bit::memory::std_allocator_adapter<T,Allocator>
  ::std_allocator_adapter( std_allocator_adapter<U,Allocator>&& other )
  noexcept
  : base_type( other.get() )
{

}

//----------------------------------------------------------------------------
// Allocation
//----------------------------------------------------------------------------

template<typename T, typename Allocator>
inline typename bit::memory::std_allocator_adapter<T,Allocator>::pointer
  bit::memory::std_allocator_adapter<T,Allocator>
  ::allocate( size_type n )
{
  auto& allocator = detail::get<0>( *this );

  return traits_type::try_allocate( allocator, n, alignof(T) );
}

template<typename T, typename Allocator>
inline void bit::memory::std_allocator_adapter<T,Allocator>
  ::deallocate( pointer p, size_type n )
{
  auto& allocator = detail::get<0>( *this );

  traits_type::deallocate( allocator, p, n );
}

//----------------------------------------------------------------------------
// Observers
//----------------------------------------------------------------------------

template<typename T, typename Allocator>
template<typename,typename>
typename bit::memory::std_allocator_adapter<T,Allocator>::size_type
  bit::memory::std_allocator_adapter<T,Allocator>::max_size()
  const noexcept
{
  auto& allocator = detail::get<0>( *this );

  return traits_type::max_size( allocator );
}

template<typename T, typename Allocator>
Allocator& bit::memory::std_allocator_adapter<T,Allocator>::get()
  const noexcept
{
  auto& allocator = detail::get<0>( *this );

  return allocator.get();
}

//----------------------------------------------------------------------------
// Utilities
//----------------------------------------------------------------------------

template<typename T, typename Allocator>
inline bit::memory::std_allocator_adapter<T,Allocator>
  bit::memory::make_allocator_adapter( Allocator& allocator )
  noexcept
{
  return std_allocator_adapter<T,Allocator>( allocator );
}

//----------------------------------------------------------------------------
// Equality Comparisons
//----------------------------------------------------------------------------

template<typename T1, typename T2, typename Allocator>
inline bool bit::memory
  ::operator==( const std_allocator_adapter<T1,Allocator>& lhs,
                const std_allocator_adapter<T2,Allocator>& rhs )
  noexcept
{
  return lhs.get() == rhs.get();
}

template<typename T1, typename T2, typename Allocator1, typename Allocator2>
inline bool bit::memory
  ::operator==( const std_allocator_adapter<T1,Allocator1>&,
                const std_allocator_adapter<T2,Allocator2>& )
  noexcept
{
  return false;
}

template<typename T1, typename T2, typename Allocator>
inline bool bit::memory
  ::operator!=( const std_allocator_adapter<T1,Allocator>& lhs,
                const std_allocator_adapter<T2,Allocator>& rhs )
  noexcept
{
  return !(lhs==rhs);
}

template<typename T1, typename T2, typename Allocator1, typename Allocator2>
inline bool bit::memory
  ::operator!=( const std_allocator_adapter<T1,Allocator1>& lhs,
                const std_allocator_adapter<T2,Allocator2>& rhs )
  noexcept
{
  return !(lhs==rhs);
}

#endif /* BIT_MEMORY_DETAIL_STD_ALLOCATOR_ADAPTER_INL */
