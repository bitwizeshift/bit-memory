#ifndef BIT_MEMORY_BLOCK_ALLOCATORS_DETAIL_ALLOCATOR_TRAITS_INL
#define BIT_MEMORY_BLOCK_ALLOCATORS_DETAIL_ALLOCATOR_TRAITS_INL

//=============================================================================
// allocator_traits
//=============================================================================

//-----------------------------------------------------------------------------
// Allocation
//-----------------------------------------------------------------------------

template<typename Allocator>
inline void* bit::memory::allocator_traits<Allocator>
  ::allocate( Allocator& alloc, std::size_t size, std::size_t align )
{
  return do_allocate( detail::allocator_has_allocate<Allocator>{}, alloc, size, align );
}

template<typename Allocator>
inline void* bit::memory::allocator_traits<Allocator>
  ::try_allocate( Allocator& alloc, std::size_t size, std::size_t align )
  noexcept
{
  // try_allocate *must* be defined
  return alloc.try_allocate( size, align );
}

//-----------------------------------------------------------------------------
// Deallocation
//-----------------------------------------------------------------------------

template<typename Allocator>
inline void bit::memory::allocator_traits<Allocator>
  ::deallocate( Allocator& alloc, void* p, std::size_t size )
{
  // deallocate *must* be defined
  alloc.deallocate( p, size );
}

//-----------------------------------------------------------------------------
// Capacity
//-----------------------------------------------------------------------------

template<typename Allocator>
inline std::size_t bit::memory::allocator_traits<Allocator>
  ::max_size( const Allocator& alloc )
  noexcept
{
  return do_max_size( detail::allocator_has_max_size<Allocator>{}, alloc );
}

template<typename Allocator>
inline std::size_t bit::memory::allocator_traits<Allocator>
  ::used( const Allocator& alloc )
  noexcept
{
  return do_used( detail::allocator_has_used<Allocator>{}, alloc );
}

//-----------------------------------------------------------------------------
// Observers
//-----------------------------------------------------------------------------

template<typename Allocator>
inline const char* bit::memory::allocator_traits<Allocator>
  ::name( const Allocator& alloc )
  noexcept
{
  return do_name( detail::allocator_has_name<Allocator>{}, alloc );
}

template<typename Allocator>
inline void bit::memory::allocator_traits<Allocator>
  ::set_name( Allocator& alloc, const char* name )
  noexcept
{
  return do_set_name( detail::allocator_has_set_name<Allocator>{}, alloc, name );
}
//=============================================================================
// Private Implementation
//=============================================================================

//-----------------------------------------------------------------------------
// Allocation
//-----------------------------------------------------------------------------

template<typename Allocator>
inline void* bit::memory::allocator_traits<Allocator>
  ::do_allocate( std::true_type, Allocator& alloc, std::size_t size, std::size_t align )
{
  return alloc.allocate( size, align );;
}

template<typename Allocator>
inline void* bit::memory::allocator_traits<Allocator>
  ::do_allocate( std::false_type, Allocator& alloc, std::size_t size, std::size_t align )
{
  auto p = allocator_traits<Allocator>::try_allocate(alloc,size,align);

  if( p == nullptr ) {
    auto n = allocator_traits<Allocator>::name( alloc );
    get_out_of_memory_handler()({n,std::addressof(alloc)}, size);
  }

  return p;
}

//-----------------------------------------------------------------------------

template<typename Allocator>
inline std::size_t bit::memory::allocator_traits<Allocator>
  ::do_max_size( std::true_type, const Allocator& alloc )
{
  return alloc.max_size();
}

template<typename Allocator>
inline std::size_t bit::memory::allocator_traits<Allocator>
  ::do_max_size( std::false_type, const Allocator& alloc )
{
  return std::numeric_limits<std::size_t>::max();
}

//-----------------------------------------------------------------------------

template<typename Allocator>
inline std::size_t bit::memory::allocator_traits<Allocator>
  ::do_used( std::true_type, const Allocator& alloc )
{
  return alloc.used();
}

template<typename Allocator>
inline std::size_t bit::memory::allocator_traits<Allocator>
  ::do_used( std::false_type, const Allocator& alloc )
{
  return 0;
}

//-----------------------------------------------------------------------------

template<typename Allocator>
inline const char* bit::memory::allocator_traits<Allocator>
  ::do_name( std::true_type, const Allocator& alloc )
{
  return alloc.name();
}

template<typename Allocator>
inline const char* bit::memory::allocator_traits<Allocator>
  ::do_name( std::false_type, const Allocator& alloc )
{
  return "Unnamed";
}

//-----------------------------------------------------------------------------

template<typename Allocator>
inline void bit::memory::allocator_traits<Allocator>
  ::do_set_name( std::true_type, Allocator& alloc, const char* name )
{
  alloc.set_name( name );
}

template<typename Allocator>
inline void bit::memory::allocator_traits<Allocator>
  ::do_set_name( std::false_type, Allocator& alloc, const char* name )
{
  (void) alloc;
  (void) name;

  // do nothing
}

#endif /* BIT_MEMORY_BLOCK_ALLOCATORS_DETAIL_ALLOCATOR_TRAITS_INL */
