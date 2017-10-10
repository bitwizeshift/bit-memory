#ifndef BIT_MEMORY_BLOCK_ALLOCATORS_DETAIL_ALLOCATOR_TRAITS_INL
#define BIT_MEMORY_BLOCK_ALLOCATORS_DETAIL_ALLOCATOR_TRAITS_INL

//=============================================================================
// allocator_traits
//=============================================================================

//-----------------------------------------------------------------------------
// Allocation
//-----------------------------------------------------------------------------

template<typename Allocator>
inline typename bit::memory::allocator_traits<Allocator>::pointer
  bit::memory::allocator_traits<Allocator>::try_allocate( Allocator& alloc,
                                                          size_type size,
                                                          size_type align )
  noexcept
{
  // try_allocate *must* be defined
  return alloc.try_allocate( size, align );
}

template<typename Allocator>
template<typename,typename>
inline typename bit::memory::allocator_traits<Allocator>::pointer
  bit::memory::allocator_traits<Allocator>::try_allocate( Allocator& alloc,
                                                          size_type size,
                                                          size_type align,
                                                          size_type offset )
  noexcept
{
  // try_allocate *must* be defined for ExtendedAllocator
  return alloc.try_allocate( size, align, offset );
}

template<typename Allocator>
inline typename bit::memory::allocator_traits<Allocator>::pointer
  bit::memory::allocator_traits<Allocator>::allocate( Allocator& alloc,
                                                      size_type size,
                                                      size_type align )
{
  return do_allocate( allocator_has_allocate<Allocator>{}, alloc, size, align );
}

template<typename Allocator>
template<typename,typename>
inline typename bit::memory::allocator_traits<Allocator>::pointer
  bit::memory::allocator_traits<Allocator>::allocate( Allocator& alloc,
                                                      size_type size,
                                                      size_type align,
                                                      size_type offset )
{
  return do_allocate( allocator_has_extended_allocate<Allocator>{}, alloc, size, align, offset );
}

//-----------------------------------------------------------------------------
// Deallocation
//-----------------------------------------------------------------------------

template<typename Allocator>
inline void bit::memory::allocator_traits<Allocator>
  ::deallocate( Allocator& alloc, pointer p, size_type size )
{
  // deallocate *must* be defined
  alloc.deallocate( p, size );
}

template<typename Allocator>
template<typename,typename>
inline void bit::memory::allocator_traits<Allocator>
  ::deallocate_all( Allocator& alloc )
{
  // Only enabled if alloc has deallocate_all
  alloc.deallocate_all();
}

//-----------------------------------------------------------------------------
// Observers
//-----------------------------------------------------------------------------

template<typename Allocator>
inline bool bit::memory::allocator_traits<Allocator>
  ::owns( Allocator& alloc, const_pointer p )
  noexcept
{
  return alloc.owns(p);
}

template<typename Allocator>
inline bit::memory::allocator_info
  bit::memory::allocator_traits<Allocator>::info( const Allocator& alloc )
  noexcept
{
  return do_info( allocator_has_info<Allocator>{}, alloc );
}

//-----------------------------------------------------------------------------
// Capacity
//-----------------------------------------------------------------------------

template<typename Allocator>
inline typename bit::memory::allocator_traits<Allocator>::size_type
  bit::memory::allocator_traits<Allocator>::max_size( const Allocator& alloc )
  noexcept
{
  return do_max_size( allocator_has_max_size<Allocator>{}, alloc );
}

template<typename Allocator>
inline typename bit::memory::allocator_traits<Allocator>::size_type
  bit::memory::allocator_traits<Allocator>::min_size( const Allocator& alloc )
  noexcept
{
  return do_min_size( allocator_has_min_size<Allocator>{}, alloc );
}

//=============================================================================
// Private Implementation
//=============================================================================

//-----------------------------------------------------------------------------
// Allocation
//-----------------------------------------------------------------------------

template<typename Allocator>
inline typename bit::memory::allocator_traits<Allocator>::pointer
  bit::memory::allocator_traits<Allocator>::do_allocate( std::true_type,
                                                         Allocator& alloc,
                                                         size_type size,
                                                         size_type align )
{
  return alloc.allocate( size, align );
}

template<typename Allocator>
inline typename bit::memory::allocator_traits<Allocator>::pointer
  bit::memory::allocator_traits<Allocator>::do_allocate( std::false_type,
                                                         Allocator& alloc,
                                                         size_type size,
                                                         size_type align )
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
inline typename bit::memory::allocator_traits<Allocator>::pointer
  bit::memory::allocator_traits<Allocator>::do_allocate( std::true_type,
                                                         Allocator& alloc,
                                                         size_type size,
                                                         size_type align,
                                                         size_type offset )
{
  return alloc.allocate( size, align, offset );
}

template<typename Allocator>
inline typename bit::memory::allocator_traits<Allocator>::pointer
  bit::memory::allocator_traits<Allocator>::do_allocate( std::false_type,
                                                         Allocator& alloc,
                                                         size_type size,
                                                         size_type align,
                                                         size_type offset )
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
inline typename bit::memory::allocator_traits<Allocator>::size_type
  bit::memory::allocator_traits<Allocator>::do_max_size( std::true_type,
                                                         const Allocator& alloc )
{
  return alloc.max_size();
}

template<typename Allocator>
inline typename bit::memory::allocator_traits<Allocator>::size_type
  bit::memory::allocator_traits<Allocator>::do_max_size( std::false_type,
                                                         const Allocator& alloc )
{
  BIT_MEMORY_UNUSED(alloc);

  return std::numeric_limits<std::size_t>::max();
}

//-----------------------------------------------------------------------------

template<typename Allocator>
inline typename bit::memory::allocator_traits<Allocator>::size_type
  bit::memory::allocator_traits<Allocator>::do_min_size( std::true_type,
                                                         const Allocator& alloc )
{
  return alloc.min_size();
}

template<typename Allocator>
inline typename bit::memory::allocator_traits<Allocator>::size_type
  bit::memory::allocator_traits<Allocator>::do_min_size( std::false_type,
                                                         const Allocator& alloc )
{
  BIT_MEMORY_UNUSED(alloc);

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
  BIT_MEMORY_UNUSED(alloc);

  return {"Unnamed",std::addressof(alloc)};
}

#endif /* BIT_MEMORY_BLOCK_ALLOCATORS_DETAIL_ALLOCATOR_TRAITS_INL */
