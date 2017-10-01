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
  ::try_allocate( Allocator& alloc, std::size_t size, std::size_t align )
  noexcept
{
  // try_allocate *must* be defined
  return alloc.try_allocate( size, align );
}

template<typename Allocator>
template<typename,typename>
inline void* bit::memory::allocator_traits<Allocator>
  ::try_allocate( Allocator& alloc, std::size_t size, std::size_t align, std::size_t offset )
  noexcept
{
  // try_allocate *must* be defined for ExtendedAllocator
  return alloc.try_allocate( size, align, offset );
}

template<typename Allocator>
inline void* bit::memory::allocator_traits<Allocator>
  ::allocate( Allocator& alloc, std::size_t size, std::size_t align )
{
  return do_allocate( detail::allocator_has_allocate<Allocator>{}, alloc, size, align );
}

template<typename Allocator>
template<typename,typename>
inline void* bit::memory::allocator_traits<Allocator>
  ::allocate( Allocator& alloc, std::size_t size, std::size_t align, std::size_t offset )
{
  return do_allocate( detail::allocator_has_extended_allocate<Allocator>{}, alloc, size, align, offset );
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
// Observers
//-----------------------------------------------------------------------------

template<typename Allocator>
inline bool bit::memory::allocator_traits<Allocator>
  ::owns( Allocator& alloc, const void* p )
  noexcept
{
  return alloc.owns(p);
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
  ::min_size( const Allocator& alloc )
  noexcept
{
  return do_min_size( detail::allocator_has_min_size<Allocator>{}, alloc );
}

//-----------------------------------------------------------------------------
// Observers
//-----------------------------------------------------------------------------

template<typename Allocator>
inline bit::memory::allocator_info
  bit::memory::allocator_traits<Allocator>::info( const Allocator& alloc )
  noexcept
{
  return do_info( detail::allocator_has_info<Allocator>{}, alloc );
}

//=============================================================================
// Private Implementation
//=============================================================================

//-----------------------------------------------------------------------------
// Allocation
//-----------------------------------------------------------------------------

template<typename Allocator>
inline void* bit::memory::allocator_traits<Allocator>
  ::do_allocate( std::true_type,
                 Allocator& alloc,
                 std::size_t size,
                 std::size_t align )
{
  return alloc.allocate( size, align );;
}

template<typename Allocator>
inline void* bit::memory::allocator_traits<Allocator>
  ::do_allocate( std::false_type,
                 Allocator& alloc,
                 std::size_t size,
                 std::size_t align )
{
  auto p = allocator_traits<Allocator>::try_allocate(alloc,size,align);

  if( p == nullptr ) {
    const auto info = allocator_traits<Allocator>::info( alloc );

    get_out_of_memory_handler()(info, size);
  }

  return p;
}

//-----------------------------------------------------------------------------

template<typename Allocator>
inline void* bit::memory::allocator_traits<Allocator>
  ::do_allocate( std::true_type,
                 Allocator& alloc,
                 std::size_t size,
                 std::size_t align,
                 std::size_t offset )
{
  return alloc.allocate( size, align, offset );
}

template<typename Allocator>
inline void* bit::memory::allocator_traits<Allocator>
  ::do_allocate( std::false_type,
                 Allocator& alloc,
                 std::size_t size,
                 std::size_t align,
                 std::size_t offset )
{
  auto p = allocator_traits<Allocator>::try_allocate(alloc,size,align,offset);

  if( p == nullptr ) {
    const auto info = allocator_traits<Allocator>::info( alloc );

    get_out_of_memory_handler()(info, size);
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
  ::do_min_size( std::true_type, const Allocator& alloc )
{
  return alloc.min_size();
}

template<typename Allocator>
inline std::size_t bit::memory::allocator_traits<Allocator>
  ::do_min_size( std::false_type, const Allocator& alloc )
{
  return 1;
}

//-----------------------------------------------------------------------------

template<typename Allocator>
inline bit::memory::allocator_info
  bit::memory::allocator_traits<Allocator>::do_info( std::true_type,
                                                     const Allocator& alloc )
{
  return alloc.info();
}

template<typename Allocator>
inline bit::memory::allocator_info
  bit::memory::allocator_traits<Allocator>::do_info( std::false_type,
                                                     const Allocator& alloc )
{
  return {"Unnamed",std::addressof(alloc)};
}

#endif /* BIT_MEMORY_BLOCK_ALLOCATORS_DETAIL_ALLOCATOR_TRAITS_INL */
