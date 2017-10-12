#ifndef BIT_MEMORY_ALLOCATORS_DETAIL_FALLBACK_ALLOCATOR_INL
#define BIT_MEMORY_ALLOCATORS_DETAIL_FALLBACK_ALLOCATOR_INL

template<typename...Allocators>
template<typename...Tuples>
bit::memory::fallback_allocator<Allocators...>
  ::fallback_allocator( std::piecewise_construct_t, Tuples&&...tuples )
  : base_type( std::forward<Tuples>(tuples)... )
{

}

template<typename...Allocators>
template<typename Arg0, typename...Args, typename>
bit::memory::fallback_allocator<Allocators...>
  ::fallback_allocator( Arg0&& arg0, Args&&...args )
  : base_type( std::forward_as_tuple( std::forward<Arg0>(arg0) ),
               std::forward_as_tuple( std::forward<Args>(args) )... )
{

}

template<typename...Allocators>
void* bit::memory::fallback_allocator<Allocators...>
  ::try_allocate( std::size_t size,
                  std::size_t align )
  noexcept
{
  return do_try_allocate( std::integral_constant<std::size_t,0>{}, size, align );
}


template<typename...Allocators>
void bit::memory::fallback_allocator<Allocators...>
  ::deallocate( void* p, std::size_t size )
{
  do_deallocate( std::integral_constant<std::size_t,0>{}, p, size );
}

template<typename...Allocators>
inline bool bit::memory::fallback_allocator<Allocators...>::owns( const void *p )
  const noexcept
{
  return do_owns( std::make_index_sequence<sizeof...(Allocators)>{}, p );
}

template<typename...Allocators>
inline std::size_t bit::memory::fallback_allocator<Allocators...>::max_size()
  const noexcept
{
  return do_max_size( std::make_index_sequence<sizeof...(Allocators)>{} );
}

template<typename...Allocators>
inline std::size_t bit::memory::fallback_allocator<Allocators...>::min_size()
  const noexcept
{
  return do_min_size( std::make_index_sequence<sizeof...(Allocators)>{} );
}

template<typename...Allocators>
template<std::size_t Idx>
void* bit::memory::fallback_allocator<Allocators...>
  ::do_try_allocate( std::integral_constant<std::size_t,Idx>,
                     std::size_t size,
                     std::size_t align )
  noexcept
{
  auto& allocator = detail::get<Idx>(static_cast<base_type&>(*this));
  using traits_type = allocator_traits<decltype(allocator)>;

  // If a pointer is allocated, return it. Otherwise fallback to the next
  // allocator.
  auto p = traits_type::try_allocate( allocator, size, align );
  if( p != nullptr ) return p;

  return do_try_allocate( std::integral_constant<std::size_t,Idx+1>{},
                          size, align );
}

template<typename...Allocators>
void* bit::memory::fallback_allocator<Allocators...>
  ::do_try_allocate( std::integral_constant<std::size_t,sizeof...(Allocators)-1>,
                     std::size_t size,
                     std::size_t align )
  noexcept
{
  auto& allocator = detail::get<sizeof...(Allocators)-1>(static_cast<base_type&>(*this));
  using traits_type = allocator_traits<decltype(allocator)>;

  return traits_type::try_allocate( allocator, size, align );;
}

template<typename...Allocators>
template<std::size_t Idx>
void bit::memory::fallback_allocator<Allocators...>
  ::do_deallocate( std::integral_constant<std::size_t,Idx>,
                   void* p,
                   std::size_t size)
{
  auto& allocator = detail::get<Idx>(static_cast<base_type&>(*this));
  using traits_type = allocator_traits<decltype(allocator)>;

  // If the allocator is known to own it, deallocate it using that allocator
  // otherwise fallback onto the next allocator
  if( traits_type::owns( allocator, p ) ) {
    traits_type::deallocate( allocator, p, size );
  } else {
    do_deallocate( std::integral_constant<std::size_t,Idx+1>{}, p, size );
  }
}

template<typename...Allocators>
void bit::memory::fallback_allocator<Allocators...>
  ::do_deallocate( std::integral_constant<std::size_t,sizeof...(Allocators)-1>,
                   void* p,
                   std::size_t size )
{
  auto& allocator = detail::get<sizeof...(Allocators)-1>(static_cast<base_type&>(*this));
  using traits_type = allocator_traits<decltype(allocator)>;

  traits_type::deallocate( allocator, p, size );
}

template<typename...Allocators>
template<std::size_t...Idxs>
inline bool bit::memory::fallback_allocator<Allocators...>
  ::do_owns( std::index_sequence<Idxs...>, const void *p )
  const noexcept
{
  // max of a bunch of bools is the disjunction (logical or)
  return std::max( { allocator_traits<Allocators>::owns( detail::get<Idxs>(static_cast<base_type&>(*this)), p )... } );
}


template<typename...Allocators>
template<std::size_t...Idxs>
std::size_t bit::memory::fallback_allocator<Allocators...>
  ::do_max_size( std::index_sequence<Idxs...> )
  const noexcept
{
  return std::max( { allocator_traits<Allocators>::max_size( detail::get<Idxs>(static_cast<base_type&>(*this)) )... } );
}
template<typename...Allocators>
template<std::size_t...Idxs>
std::size_t bit::memory::fallback_allocator<Allocators...>
  ::do_min_size( std::index_sequence<Idxs...> )
  const noexcept
{
  return std::min( { allocator_traits<Allocators>::min_size( detail::get<Idxs>(static_cast<base_type&>(*this)) )... } );
}

#endif /* BIT_MEMORY_ALLOCATORS_DETAIL_FALLBACK_ALLOCATOR_INL */
