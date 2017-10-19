#ifndef BIT_MEMORY_DETAIL_STD_ANY_ALLOCATOR_ADAPTER_INL
#define BIT_MEMORY_DETAIL_STD_ANY_ALLOCATOR_ADAPTER_INL

//============================================================================
// std_allocator_adapter
//============================================================================

//----------------------------------------------------------------------------
// Constructors
//----------------------------------------------------------------------------

template<typename T>
template<typename Allocator, typename>
inline bit::memory::std_any_allocator_adapter<T>
  ::std_any_allocator_adapter( Allocator& allocator )
  noexcept
  : m_allocator( allocator )
{

}

template<typename T>
template<typename U>
inline bit::memory::std_any_allocator_adapter<T>
  ::std_any_allocator_adapter( const std_any_allocator_adapter<U>& other )
  noexcept
  : m_allocator( other.get() )
{

}

template<typename T>
template<typename U>
inline bit::memory::std_any_allocator_adapter<T>
  ::std_any_allocator_adapter( std_any_allocator_adapter<U>&& other )
  noexcept
  : m_allocator( other.get() )
{

}

//----------------------------------------------------------------------------
// Allocation
//----------------------------------------------------------------------------

template<typename T>
inline typename bit::memory::std_any_allocator_adapter<T>::pointer
  bit::memory::std_any_allocator_adapter<T>
  ::allocate( size_type n )
{
  auto p = m_allocator.try_allocate( sizeof(T) * n, alignof(T) );
  return static_cast<pointer>(p);
}

template<typename T>
inline void bit::memory::std_any_allocator_adapter<T>
  ::deallocate( pointer p, size_type n )
{
  m_allocator.deallocate( static_cast<void_pointer>(p), sizeof(T) * n );
}

//----------------------------------------------------------------------------
// Observers
//----------------------------------------------------------------------------

template<typename T>
inline bit::memory::any_allocator
  bit::memory::std_any_allocator_adapter<T>::get()
  const noexcept
{
  return m_allocator;
}

//----------------------------------------------------------------------------
// Equality Comparisons
//----------------------------------------------------------------------------

template<typename T, typename U>
inline bool bit::memory
  ::operator==( const std_any_allocator_adapter<T>& lhs,
                const std_any_allocator_adapter<U>& rhs )
  noexcept
{
  return lhs.get() == rhs.get();
}

template<typename T, typename U>
inline bool bit::memory
  ::operator!=( const std_any_allocator_adapter<T>& lhs,
                const std_any_allocator_adapter<U>& rhs )
  noexcept
{
  return !(lhs==rhs);
}

#endif /* BIT_MEMORY_DETAIL_STD_ANY_ALLOCATOR_ADAPTER_INL */
