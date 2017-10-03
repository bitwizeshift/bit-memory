#ifndef BIT_MEMORY_ALLOCATORS_DETAIL_ARENA_ALLOCATOR_INL
#define BIT_MEMORY_ALLOCATORS_DETAIL_ARENA_ALLOCATOR_INL

template<typename ExtendedAllocator, typename Tagger, typename Tracker,typename Checker, typename Lock>
template<typename...Args, typename>
bit::memory::arena_allocator<ExtendedAllocator,Tagger,Tracker,Checker,Lock>
  ::arena_allocator( Args&&...args )
  : m_storage( std::forward_as_tuple(std::forward<Args>(args)...),
               std::make_tuple(),
               std::make_tuple(),
               std::make_tuple(),
               std::make_tuple() )
{
  // The 'make_tuple' calls here are necessary because every other component
  // except for 'Allocator' must be default-constructible
}

template<typename ExtendedAllocator, typename Tagger, typename Tracker,typename Checker, typename Lock>
bit::memory::arena_allocator<ExtendedAllocator,Tagger,Tracker,Checker,Lock>
  ::~arena_allocator()
{
  auto& allocator = detail::get<0>(m_storage);
  auto& tracker   = detail::get<2>(m_storage);

  tracker.finalize( allocator_traits<ExtendedAllocator>::info(allocator) );
}

template<typename ExtendedAllocator, typename Tagger, typename Tracker,typename Checker, typename Lock>
const typename bit::memory::arena_allocator<ExtendedAllocator,Tagger,Tracker,Checker,Lock>::tracker_type&
  bit::memory::arena_allocator<ExtendedAllocator,Tagger,Tracker,Checker,Lock>
  ::tracker()
  const noexcept
{
  return detail::get<2>( m_storage );
}

template<typename ExtendedAllocator, typename Tagger, typename Tracker,typename Checker, typename Lock>
void* bit::memory::arena_allocator<ExtendedAllocator,Tagger,Tracker,Checker,Lock>
  ::try_allocate( std::size_t size, std::size_t align )
  noexcept
{
  // 'new_size' technically misses a few bytes reserved for padding and any
  // additional information the allocation policy may require; however it
  // gives a more consistent and deterministic size for all allocations. By
  // requesting 'n' bytes, you will receive 'n+m' bytes -- rather than
  // altering values determined by *order* that allocations occur
  const auto new_size = Checker::front_size + size + Checker::back_size;
  const auto offset   = Checker::front_size;

  auto& allocator = detail::get<0>(m_storage);
  auto& tagger    = detail::get<1>(m_storage);
  auto& tracker   = detail::get<2>(m_storage);
  auto& checker   = detail::get<3>(m_storage);
  auto& lock      = detail::get<4>(m_storage);

  byte* byte_ptr = nullptr;

  { // critical section
    std::lock_guard<lock_type> scope(lock);

    auto* p = allocator_traits<ExtendedAllocator>::try_allocate( allocator, new_size, align, offset );
    byte_ptr = static_cast<byte*>(p);

    // nullptr being returned is not the hot code-path
    if( BIT_MEMORY_UNLIKELY(!byte_ptr) ) return nullptr;

    // Track the allocation
    tracker.on_allocate( byte_ptr + Checker::front_size, new_size, align );
  }


  // Check the boundary, and tag the allocation
  checker.prepare_front_fence( byte_ptr, Checker::front_size );
  tagger.tag_allocation( byte_ptr + Checker::front_size, size );
  checker.prepare_back_fence( byte_ptr + Checker::front_size + size, Checker::back_size );

  // adjust the pointer by the offset, and return it
  return byte_ptr + Checker::front_size;
}

template<typename ExtendedAllocator, typename Tagger, typename Tracker,typename Checker, typename Lock>
void bit::memory::arena_allocator<ExtendedAllocator,Tagger,Tracker,Checker,Lock>
  ::deallocate( void* p, std::size_t size )
{
  auto& allocator = detail::get<0>(m_storage);
  auto& tagger    = detail::get<1>(m_storage);
  auto& tracker   = detail::get<2>(m_storage);
  auto& checker   = detail::get<3>(m_storage);
  auto& lock      = detail::get<4>(m_storage);

  const auto new_size = Checker::front_size + size + Checker::back_size;
  const auto offset   = Checker::front_size;

  auto* byte_ptr = static_cast<byte*>(p) - offset;

  const auto info = allocator_traits<ExtendedAllocator>::info(allocator);

  // Check the boundary, and tag the deallocation
  checker.check_front_fence( info, byte_ptr, Checker::front_size );
  tagger.tag_deallocation( byte_ptr + Checker::front_size, size );
  checker.check_back_fence( info, byte_ptr + Checker::front_size + size , Checker::back_size );

  { // critical section
    std::lock_guard<lock_type> scope(lock);

    // Untrack the deallocation
    tracker.on_deallocate( p, new_size );

    allocator_traits<ExtendedAllocator>::deallocate( allocator, byte_ptr, new_size );
  }
}

template<typename ExtendedAllocator, typename Tagger, typename Tracker,typename Checker, typename Lock>
template<typename, typename>
void bit::memory::arena_allocator<ExtendedAllocator,Tagger,Tracker,Checker,Lock>
  ::deallocate_all()
{
  auto& allocator = detail::get<0>(m_storage);
  auto& tracker   = detail::get<2>(m_storage);

  tracker.on_deallocate_all();

  allocator_traits<ExtendedAllocator>::deallocate_all( allocator );
}

//-----------------------------------------------------------------------------
// Comparison
//-----------------------------------------------------------------------------

template<typename Allocator, typename Tagger, typename Tracker,typename Checker, typename Lock>
inline bool bit::memory
  ::operator==( const arena_allocator<Allocator,Tagger,Tracker,Checker,Lock>& lhs,
                const arena_allocator<Allocator,Tagger,Tracker,Checker,Lock>& rhs )
  noexcept
{
  auto& lhs_allocator = detail::get<0>( lhs.m_storage );
  auto& rhs_allocator = detail::get<0>( rhs.m_storage );

  return lhs_allocator == rhs_allocator;
}


template<typename Allocator, typename Tagger, typename Tracker,typename Checker, typename Lock>
inline bool bit::memory
  ::operator!=( const arena_allocator<Allocator,Tagger,Tracker,Checker,Lock>& lhs,
                const arena_allocator<Allocator,Tagger,Tracker,Checker,Lock>& rhs )
  noexcept
{
  return !(lhs == rhs);
}


#endif /* BIT_MEMORY_ALLOCATORS_DETAIL_ARENA_ALLOCATOR_INL */
