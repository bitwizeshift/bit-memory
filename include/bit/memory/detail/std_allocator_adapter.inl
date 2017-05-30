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
  : m_instance( std::addressof(allocator) )
{

}

template<typename T, typename Allocator>
template<typename U>
inline bit::memory::std_allocator_adapter<T,Allocator>
  ::std_allocator_adapter( const std_allocator_adapter<U,Allocator>& other )
  noexcept
  : m_instance( std::addressof(other.get()) )
{

}

template<typename T, typename Allocator>
template<typename U>
inline bit::memory::std_allocator_adapter<T,Allocator>
  ::std_allocator_adapter( std_allocator_adapter<U,Allocator>&& other )
  noexcept
  : m_instance( std::addressof(other.get()) )
{

}

//----------------------------------------------------------------------------
// Allocation
//----------------------------------------------------------------------------

template<typename T, typename Allocator>
inline T* bit::memory::std_allocator_adapter<T,Allocator>
  ::allocate( std::size_t n )
{
  return m_instance->allocate( sizeof(T), alignof(T), n );
}

template<typename T, typename Allocator>
inline void bit::memory::std_allocator_adapter<T,Allocator>
  ::deallocate( T* p, std::size_t n )
{
  m_instance->deallocate( p, n );
}

//----------------------------------------------------------------------------
// Observers
//----------------------------------------------------------------------------

template<typename T, typename Allocator>
Allocator& bit::memory::std_allocator_adapter<T,Allocator>::get()
  const noexcept
{
  return *m_instance;
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
