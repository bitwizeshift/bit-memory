#ifndef BIT_MEMORY_ALLOCATORS_DETAIL_FALLBACK_ALLOCATOR_INL
#define BIT_MEMORY_ALLOCATORS_DETAIL_FALLBACK_ALLOCATOR_INL

//-----------------------------------------------------------------------------
// Constructors
//-----------------------------------------------------------------------------

template<typename...AllocatorStorages>
inline bit::memory::fallback_allocator<AllocatorStorages...>
  ::fallback_allocator( AllocatorStorages...storages )
  noexcept
  : base_type( std::forward_as_tuple( std::move(storages) )... )
{

}

//-----------------------------------------------------------------------------
// Allocation / Deallocation
//-----------------------------------------------------------------------------

template<typename...AllocatorStorages>
inline bit::memory::owner<void*>
  bit::memory::fallback_allocator<AllocatorStorages...>
  ::try_allocate( std::size_t size, std::size_t align )
  noexcept
{
  return do_try_allocate( std::integral_constant<std::size_t,0>{}, size, align );
}


template<typename...AllocatorStorages>
inline void bit::memory::fallback_allocator<AllocatorStorages...>
  ::deallocate( owner<void*> p, std::size_t size )
{
  do_deallocate( std::integral_constant<std::size_t,0>{}, p, size );
}

//-----------------------------------------------------------------------------
// Observers
//-----------------------------------------------------------------------------

template<typename...AllocatorStorages>
inline bool bit::memory::fallback_allocator<AllocatorStorages...>::owns( const void *p )
  const noexcept
{
  return do_owns( std::make_index_sequence<sizeof...(AllocatorStorages)>{}, p );
}

//-----------------------------------------------------------------------------
// Capacity
//-----------------------------------------------------------------------------

template<typename...AllocatorStorages>
inline std::size_t bit::memory::fallback_allocator<AllocatorStorages...>::max_size()
  const noexcept
{
  return do_max_size( std::make_index_sequence<sizeof...(AllocatorStorages)>{} );
}

template<typename...AllocatorStorages>
inline std::size_t bit::memory::fallback_allocator<AllocatorStorages...>::min_size()
  const noexcept
{
  return do_min_size( std::make_index_sequence<sizeof...(AllocatorStorages)>{} );
}

//-----------------------------------------------------------------------------
// Private : Allocation / Deallocation
//-----------------------------------------------------------------------------

template<typename...AllocatorStorages>
template<std::size_t Idx>
inline void* bit::memory::fallback_allocator<AllocatorStorages...>
  ::do_try_allocate( std::integral_constant<std::size_t,Idx>,
                     std::size_t size,
                     std::size_t align )
  noexcept
{
  auto& storage   = get<Idx>(static_cast<base_type&>(*this));
  auto& allocator = storage.get_allocator();

  using traits_type = allocator_traits<decltype(allocator)>;

  // If a pointer is allocated, return it. Otherwise fallback to the next
  // allocator.
  auto p = traits_type::try_allocate( allocator, size, align );
  if( p != nullptr ) return p;

  static constexpr auto tag = std::integral_constant<std::size_t,Idx+1>{};

  return do_try_allocate( tag, size, align );
}

template<typename...AllocatorStorages>
inline void* bit::memory::fallback_allocator<AllocatorStorages...>
  ::do_try_allocate( std::integral_constant<std::size_t,sizeof...(AllocatorStorages)-1>,
                     std::size_t size,
                     std::size_t align )
  noexcept
{
  static constexpr auto n = sizeof...(AllocatorStorages)-1;

  auto& storage   = get<n>(static_cast<base_type&>(*this));
  auto& allocator = storage.get_allocator();

  using traits_type = allocator_traits<decltype(allocator)>;

  return traits_type::try_allocate( allocator, size, align );;
}

//-----------------------------------------------------------------------------

template<typename...AllocatorStorages>
template<std::size_t Idx>
inline void bit::memory::fallback_allocator<AllocatorStorages...>
  ::do_deallocate( std::integral_constant<std::size_t,Idx>,
                   void* p,
                   std::size_t size)
{
  auto& storage     = get<Idx>(static_cast<base_type&>(*this));
  auto& allocator   = storage.get_allocator();

  using traits_type = allocator_traits<decltype(allocator)>;

  static constexpr auto tag = std::integral_constant<std::size_t,Idx+1>{};

  // If the allocator is known to own it, deallocate it using that allocator
  // otherwise fallback onto the next allocator
  if( traits_type::owns( allocator, p ) ) {
    traits_type::deallocate( allocator, p, size );
    return;
  }

  do_deallocate( tag, p, size );
}

template<typename...AllocatorStorages>
inline void bit::memory::fallback_allocator<AllocatorStorages...>
  ::do_deallocate( std::integral_constant<std::size_t,sizeof...(AllocatorStorages)-1>,
                   void* p,
                   std::size_t size )
{
  static constexpr auto n = sizeof...(AllocatorStorages)-1;

  auto& storage   = get<n>(static_cast<base_type&>(*this));
  auto& allocator = storage.get_allocator();

  using traits_type = allocator_traits<decltype(allocator)>;

  traits_type::deallocate( allocator, p, size );
}

//-----------------------------------------------------------------------------
// Private : Observers
//-----------------------------------------------------------------------------

// unfortunately, the following 3 expansions are not (easily) able to be
// shortened since the variadic set of 'AllocatorStorages' and 'Idx' need to be
// expanded at the same time

template<typename...AllocatorStorages>
template<std::size_t...Idxs>
inline bool bit::memory::fallback_allocator<AllocatorStorages...>
  ::do_owns( std::index_sequence<Idxs...>, const void *p )
  const noexcept
{
  // max of a bunch of bools is the disjunction (logical or)
  return std::max( { allocator_traits<AllocatorStorages>::owns( get<Idxs>(static_cast<base_type&>(*this)), p )... } );
}

//-----------------------------------------------------------------------------
// Private : Capacity
//-----------------------------------------------------------------------------

template<typename...AllocatorStorages>
template<std::size_t...Idxs>
inline std::size_t bit::memory::fallback_allocator<AllocatorStorages...>
  ::do_max_size( std::index_sequence<Idxs...> )
  const noexcept
{
  return std::max( { allocator_traits<AllocatorStorages>::max_size( get<Idxs>(static_cast<base_type&>(*this)) )... } );
}

template<typename...AllocatorStorages>
template<std::size_t...Idxs>
inline std::size_t bit::memory::fallback_allocator<AllocatorStorages...>
  ::do_min_size( std::index_sequence<Idxs...> )
  const noexcept
{
  return std::min( { allocator_traits<AllocatorStorages>::min_size( get<Idxs>(static_cast<base_type&>(*this)) )... } );
}

//=============================================================================
// Free Functions
//=============================================================================

//-----------------------------------------------------------------------------
// Utilities
//-----------------------------------------------------------------------------

template<typename...AllocatorStorages>
bit::memory::fallback_allocator<AllocatorStorages...>
  bit::memory::make_fallback_allocator( AllocatorStorages...storages )
{
  using type = fallback_allocator<AllocatorStorages...>;

  return type{ std::move(storages)... };
}

template<typename...AllocatorStorages>
bit::memory::named_fallback_allocator<AllocatorStorages...>
  bit::memory::make_named_fallback_allocator( const char* name,
                                              AllocatorStorages...storages )
{
  using type = named_fallback_allocator<AllocatorStorages...>;

  return type{ name, std::move(storages)... };
}


#endif /* BIT_MEMORY_ALLOCATORS_DETAIL_FALLBACK_ALLOCATOR_INL */
