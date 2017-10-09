#ifndef BIT_MEMORY_ALLOCATORS_DETAIL_POLICY_ALLOCATOR_INL
#define BIT_MEMORY_ALLOCATORS_DETAIL_POLICY_ALLOCATOR_INL

template<typename ExtendedAllocator, typename Tagger, typename Tracker,typename Checker, typename Lock>
template<typename...Args, typename>
bit::memory::policy_allocator<ExtendedAllocator,Tagger,Tracker,Checker,Lock>
  ::policy_allocator( Args&&...args )
  : base_type( std::forward_as_tuple(std::forward<Args>(args)...),
               std::make_tuple(),
               std::make_tuple(),
               std::make_tuple(),
               std::make_tuple() )
{
  // The 'make_tuple' calls here are necessary because every other component
  // except for 'Allocator' must be default-constructible
}

template<typename ExtendedAllocator, typename Tagger, typename Tracker,typename Checker, typename Lock>
bit::memory::policy_allocator<ExtendedAllocator,Tagger,Tracker,Checker,Lock>
  ::~policy_allocator()
{
  auto& allocator = detail::get<0>(*this);
  auto& tracker   = detail::get<2>(*this);

  tracker.finalize( allocator_traits<ExtendedAllocator>::info(allocator) );
}

template<typename ExtendedAllocator, typename Tagger, typename Tracker,typename Checker, typename Lock>
const typename bit::memory::policy_allocator<ExtendedAllocator,Tagger,Tracker,Checker,Lock>::tracker_type&
  bit::memory::policy_allocator<ExtendedAllocator,Tagger,Tracker,Checker,Lock>
  ::tracker()
  const noexcept
{
  return detail::get<2>(*this);
}

template<typename ExtendedAllocator, typename Tagger, typename Tracker,typename Checker, typename Lock>
void* bit::memory::policy_allocator<ExtendedAllocator,Tagger,Tracker,Checker,Lock>
  ::try_allocate( std::size_t size, std::size_t align )
  noexcept
{
  using byte_t = unsigned char;

  // 'new_size' technically misses a few bytes reserved for padding and any
  // additional information the allocation policy may require; however it
  // gives a more consistent and deterministic size for all allocations. By
  // requesting 'n' bytes, you will receive 'n+m' bytes -- rather than
  // altering values determined by *order* that allocations occur
  const auto new_size = Checker::front_size + size + Checker::back_size;
  const auto offset   = Checker::front_size;

  auto& allocator = detail::get<0>(*this);
  auto& tagger    = detail::get<1>(*this);
  auto& tracker   = detail::get<2>(*this);
  auto& checker   = detail::get<3>(*this);
  auto& lock      = detail::get<4>(*this);

  byte_t* byte_ptr = nullptr;

  { // critical section
    std::lock_guard<lock_type> scope(lock);

    auto* p = allocator_traits<ExtendedAllocator>::try_allocate( allocator, new_size, align, offset );
    byte_ptr = static_cast<byte_t*>(p);

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
void bit::memory::policy_allocator<ExtendedAllocator,Tagger,Tracker,Checker,Lock>
  ::deallocate( void* p, std::size_t size )
{
  using byte_t = unsigned char;

  auto& allocator = detail::get<0>(*this);
  auto& tagger    = detail::get<1>(*this);
  auto& tracker   = detail::get<2>(*this);
  auto& checker   = detail::get<3>(*this);
  auto& lock      = detail::get<4>(*this);

  const auto new_size = Checker::front_size + size + Checker::back_size;
  const auto offset   = Checker::front_size;

  auto* byte_ptr = static_cast<byte_t*>(p) - offset;

  const auto info = allocator_traits<ExtendedAllocator>::info(allocator);

  // Check the boundary, and tag the deallocation
  checker.check_front_fence( info, byte_ptr, Checker::front_size );
  tagger.tag_deallocation( byte_ptr + Checker::front_size, size );
  checker.check_back_fence( info, byte_ptr + Checker::front_size + size , Checker::back_size );

  { // critical section
    std::lock_guard<lock_type> scope(lock);

    // Untrack the deallocation
    tracker.on_deallocate( info, p, new_size );

    allocator_traits<ExtendedAllocator>::deallocate( allocator, byte_ptr, new_size );
  }
}

template<typename ExtendedAllocator, typename Tagger, typename Tracker,typename Checker, typename Lock>
template<typename, typename>
void bit::memory::policy_allocator<ExtendedAllocator,Tagger,Tracker,Checker,Lock>
  ::deallocate_all()
{
  auto& allocator = detail::get<0>(*this);
  auto& tracker   = detail::get<2>(*this);

  tracker.on_deallocate_all();

  allocator_traits<ExtendedAllocator>::deallocate_all( allocator );
}

template<typename ExtendedAllocator, typename Tagger, typename Tracker,typename Checker, typename Lock>
inline bool bit::memory::policy_allocator<ExtendedAllocator,Tagger,Tracker,Checker,Lock>
  ::equals( const policy_allocator& other )
  const noexcept
{
  auto& lhs_allocator = detail::get<0>( *this );
  auto& rhs_allocator = detail::get<0>( other );

  return lhs_allocator == rhs_allocator;
}

//-----------------------------------------------------------------------------
// Comparison
//-----------------------------------------------------------------------------

template<typename Allocator, typename Tagger, typename Tracker,typename Checker, typename Lock>
inline bool bit::memory
  ::operator==( const policy_allocator<Allocator,Tagger,Tracker,Checker,Lock>& lhs,
                const policy_allocator<Allocator,Tagger,Tracker,Checker,Lock>& rhs )
  noexcept
{
  return lhs.equals(rhs);
}


template<typename Allocator, typename Tagger, typename Tracker,typename Checker, typename Lock>
inline bool bit::memory
  ::operator!=( const policy_allocator<Allocator,Tagger,Tracker,Checker,Lock>& lhs,
                const policy_allocator<Allocator,Tagger,Tracker,Checker,Lock>& rhs )
  noexcept
{
  return !(lhs == rhs);
}


#endif /* BIT_MEMORY_ALLOCATORS_DETAIL_POLICY_ALLOCATOR_INL */
