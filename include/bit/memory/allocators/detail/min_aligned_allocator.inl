#ifndef BIT_MEMORY_ALLOCATORS_DETAIL_MIN_ALIGNED_ALLOCATOR_INL
#define BIT_MEMORY_ALLOCATORS_DETAIL_MIN_ALIGNED_ALLOCATOR_INL

//=============================================================================
// min_aligned_allocator
//=============================================================================

//-----------------------------------------------------------------------------
// Constructors
//-----------------------------------------------------------------------------

template<typename Allocator, std::size_t MinAlign>
inline bit::memory::min_aligned_allocator<Allocator,MinAlign>
  ::min_aligned_allocator()
  : base_type{}
{

}

template<typename Allocator, std::size_t MinAlign>
template<typename Arg0, typename...Args, typename>
inline bit::memory::min_aligned_allocator<Allocator,MinAlign>
  ::min_aligned_allocator( Arg0&& arg0, Args&&...args )
  : base_type( std::forward_as_tuple( std::forward<Arg0>(arg0), std::forward<Args>(args)... ) )
{

}

//-----------------------------------------------------------------------------
// Allocation / Deallocation
//-----------------------------------------------------------------------------

template<typename Allocator, std::size_t MinAlign>
inline bit::memory::owner<void*>
  bit::memory::min_aligned_allocator<Allocator,MinAlign>
  ::allocate( std::size_t size, std::size_t align )
{
  const auto new_align = align > MinAlign ? align : MinAlign;
  auto& allocator = detail::get<0>( *this );

  return traits_type::allocate( allocator, size, new_align );
}

template<typename Allocator, std::size_t MinAlign>
inline bit::memory::owner<void*>
  bit::memory::min_aligned_allocator<Allocator,MinAlign>
  ::try_allocate( std::size_t size, std::size_t align )
  noexcept
{
  const auto new_align = align > MinAlign ? align : MinAlign;
  auto& allocator = detail::get<0>( *this );

  return traits_type::allocate( allocator, size, new_align );
}

template<typename Allocator, std::size_t MinAlign>
inline void bit::memory::min_aligned_allocator<Allocator,MinAlign>
  ::deallocate( owner<void*> p, std::size_t size )
{
  auto& allocator = detail::get<0>( *this );

  traits_type::deallocate( allocator, size, size );
}

//-----------------------------------------------------------------------------
// Naming
//-----------------------------------------------------------------------------

template<typename Allocator, std::size_t MinAlign>
inline const char* bit::memory::min_aligned_allocator<Allocator,MinAlign>
  ::name()
  const noexcept
{
  auto& allocator = detail::get<0>( *this );

  return traits_type::name( allocator );
}

template<typename Allocator, std::size_t MinAlign>
inline void bit::memory::min_aligned_allocator<Allocator,MinAlign>
  ::set_name( const char* name )
  noexcept
{
  auto& allocator = detail::get<0>( *this );

  return traits_type::set_name( allocator, name );
}

//-----------------------------------------------------------------------------
// Capacity
//-----------------------------------------------------------------------------

template<typename Allocator, std::size_t MinAlign>
inline std::size_t bit::memory::min_aligned_allocator<Allocator,MinAlign>
  ::max_size()
  const noexcept
{
  auto& allocator = detail::get<0>( *this );

  return traits_type::max_size( allocator );
}

template<typename Allocator, std::size_t MinAlign>
inline std::size_t bit::memory::min_aligned_allocator<Allocator,MinAlign>
  ::used()
  const noexcept
{
  auto& allocator = detail::get<0>( *this );

  return traits_type::used( allocator );
}

//-----------------------------------------------------------------------------
// Comparisons
//-----------------------------------------------------------------------------

template<typename Allocator, std::size_t MinAlign>
inline bool bit::memory::min_aligned_allocator<Allocator,MinAlign>
  ::operator==( const min_aligned_allocator& rhs )
  const noexcept
{
  auto& lhs_alloc = detail::get<0>( *this );
  auto& rhs_alloc = detail::get<0>( rhs );

  return lhs_alloc == rhs_alloc;
}

template<typename Allocator, std::size_t MinAlign>
inline bool bit::memory::min_aligned_allocator<Allocator,MinAlign>
  ::operator!=( const min_aligned_allocator& rhs )
  const noexcept
{
  auto& lhs_alloc = detail::get<0>( *this );
  auto& rhs_alloc = detail::get<0>( rhs );

  return lhs_alloc != rhs_alloc;
}

#endif /* BIT_MEMORY_ALLOCATORS_DETAIL_MIN_ALIGNED_ALLOCATOR_INL */
