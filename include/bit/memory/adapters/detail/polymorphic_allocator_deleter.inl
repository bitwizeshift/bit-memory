#ifndef BIT_MEMORY_ADAPTERS_DETAIL_POLYMORPHIC_ALLOCATOR_DELETER_INL
#define BIT_MEMORY_ADAPTERS_DETAIL_POLYMORPHIC_ALLOCATOR_DELETER_INL

//=============================================================================
// polymorphic_allocator_deleter<T,AllocatorStorage>
//=============================================================================

//-----------------------------------------------------------------------------
// Constructors / Assignment
//-----------------------------------------------------------------------------

template<typename T, typename AllocatorStorage>
inline bit::memory::polymorphic_allocator_deleter<T,AllocatorStorage>
  ::polymorphic_allocator_deleter( AllocatorStorage storage )
  noexcept
  : base_type( std::forward_as_tuple( std::move(storage) ) ),
    m_size( sizeof(T) )
{

}

//-----------------------------------------------------------------------------

template<typename T, typename AllocatorStorage>
template<typename U, typename>
inline bit::memory::polymorphic_allocator_deleter<T,AllocatorStorage>
  ::polymorphic_allocator_deleter( polymorphic_allocator_deleter<U,AllocatorStorage>&& other )
  noexcept
  : base_type( std::forward_as_tuple( other.get_storage() ) ),
    m_size( other.size() )
{

}

template<typename T, typename AllocatorStorage>
template<typename U, typename>
inline bit::memory::polymorphic_allocator_deleter<T,AllocatorStorage>
  ::polymorphic_allocator_deleter( const polymorphic_allocator_deleter<U,AllocatorStorage>& other )
  noexcept
  : base_type( std::forward_as_tuple( other.get_storage() ) ),
    m_size( other.size() )
{

}

//-----------------------------------------------------------------------------

template<typename T, typename AllocatorStorage>
template<typename U, typename>
inline bit::memory::polymorphic_allocator_deleter<T,AllocatorStorage>&
  bit::memory::polymorphic_allocator_deleter<T,AllocatorStorage>
  ::operator=( polymorphic_allocator_deleter<U,AllocatorStorage>&& other )
  noexcept
{
  get<0>(*this) = other.storage();
  m_size = other.size();

  return (*this);
}

template<typename T, typename AllocatorStorage>
template<typename U, typename>
inline bit::memory::polymorphic_allocator_deleter<T,AllocatorStorage>&
  bit::memory::polymorphic_allocator_deleter<T,AllocatorStorage>
  ::operator=( const polymorphic_allocator_deleter<U,AllocatorStorage>& other )
  noexcept
{
  get<0>(*this) = other.storage();
  m_size = other.size();

  return (*this);
}

//-----------------------------------------------------------------------------
// Deallocations
//-----------------------------------------------------------------------------

template<typename T, typename AllocatorStorage>
inline void bit::memory::polymorphic_allocator_deleter<T,AllocatorStorage>
  ::operator()( pointer p )
{
  auto& storage   = get<0>(*this);
  auto& allocator = storage.get_allocator();

  alloc_traits::destroy( allocator, to_raw_pointer(p) );
  alloc_traits::deallocate( allocator, static_cast<void_pointer>(p), m_size );
}

//-----------------------------------------------------------------------------
// Private Observers
//-----------------------------------------------------------------------------

template<typename T, typename AllocatorStorage>
inline AllocatorStorage
  bit::memory::polymorphic_allocator_deleter<T,AllocatorStorage>::storage()
  const noexcept
{
  return get<0>(*this);
}

template<typename T, typename AllocatorStorage>
inline std::size_t
  bit::memory::polymorphic_allocator_deleter<T,AllocatorStorage>::size()
  const noexcept
{
  return m_size;
}

//=============================================================================
// polymorphic_allocator_deleter<T,Allocator>
//=============================================================================

//-----------------------------------------------------------------------------
// Constructors
//-----------------------------------------------------------------------------

template<typename T, typename AllocatorStorage>
inline bit::memory::polymorphic_allocator_deleter<T[],AllocatorStorage>
  ::polymorphic_allocator_deleter( AllocatorStorage storage, size_type size )
  noexcept
  : base_type( std::forward_as_tuple( std::move(storage) ) ),
    m_size( size )
{

}

//-----------------------------------------------------------------------------
// Deallocations
//-----------------------------------------------------------------------------

template<typename T, typename AllocatorStorage>
inline void bit::memory::polymorphic_allocator_deleter<T[],AllocatorStorage>
  ::operator()( pointer p )
{
  auto& storage   = get<0>(*this);
  auto& allocator = storage.get_allocator();

  // Destroy all memory, then deallocate it
  auto end     = to_raw_pointer(p);
  auto current = end + m_size;

  while( current != end ) {
    alloc_traits::destroy( allocator, --current );
  }
  alloc_traits::deallocate( allocator, static_cast<void_pointer>(p), sizeof(T) * m_size );
}

#endif /* BIT_MEMORY_ADAPTERS_DETAIL_POLYMORPHIC_ALLOCATOR_DELETER_INL */
