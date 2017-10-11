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

//=============================================================================
// Free Functions
//=============================================================================

//-----------------------------------------------------------------------------
// unique_ptr
//-----------------------------------------------------------------------------

template<typename T, typename Allocator, typename... Args>
inline bit::memory::detail::unique_ptr_single_t<T,Allocator>
  bit::memory::allocate_unique( Allocator& allocator, Args&&...args )
{
  using traits_type = allocator_traits<std::decay_t<Allocator>>;
  auto* p = traits_type::allocate( allocator, sizeof(T), alignof(T) );

  return { uninitialized_construct_at<T>(p, std::forward<Args>(args)... ), allocator };
}

template<typename T, typename Allocator, typename... Args>
inline bit::memory::detail::unique_ptr_array_t<T,Allocator>
  bit::memory::allocate_unique( Allocator& allocator, std::size_t n )
{
  using traits_type = allocator_traits<std::decay_t<Allocator>>;
  auto* p = traits_type::allocate( allocator, sizeof(T) * n, alignof(T) );

  return { uninitialized_construct_array_at<T>(p, n), allocator, n };
}

//-----------------------------------------------------------------------------
// shared_ptr
//-----------------------------------------------------------------------------

template<typename T, typename Allocator, typename... Args>
inline bit::memory::detail::shared_ptr_single_t<T>
  bit::memory::allocate_shared( Allocator& allocator, Args&&...args )
{
  using traits_type = allocator_traits<std::decay_t<Allocator>>;

  auto* p      = traits_type::allocate( allocator, sizeof(T), alignof(T) );
  auto deleter = allocator_deleter<T,std::decay_t<Allocator>>{allocator};

  return { uninitialized_construct_at<T>(p, std::forward<Args>(args)... ), deleter };
}

template<typename T, typename Allocator, typename... Args>
inline bit::memory::detail::shared_ptr_array_t<T>
  bit::memory::allocate_shared( Allocator& allocator, std::size_t n )
{
  using traits_type = allocator_traits<std::decay_t<Allocator>>;

  auto* p      = traits_type::allocate( allocator, sizeof(T) * n, alignof(T) );
  auto deleter = allocator_deleter<T,std::decay_t<Allocator>>{allocator,n};

  return { uninitialized_construct_array_at<T>(p, n), deleter };
}

#endif /* BIT_MEMORY_DETAIL_ALLOCATOR_DELETER_INL */
