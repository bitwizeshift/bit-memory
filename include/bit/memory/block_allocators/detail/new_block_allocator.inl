#ifndef BIT_MEMORY_BLOCK_ALLOCATORS_DETAIL_NEW_BLOCK_ALLOCATOR_INL
#define BIT_MEMORY_BLOCK_ALLOCATORS_DETAIL_NEW_BLOCK_ALLOCATOR_INL

//-----------------------------------------------------------------------------
// Constructors
//-----------------------------------------------------------------------------

template<std::size_t Size>
template<std::size_t,typename>
inline bit::memory::new_block_allocator<Size>
  ::new_block_allocator( std::size_t size )
  noexcept
  : block_size_member( size )
{

}

//-----------------------------------------------------------------------------
// Block Allocations
//-----------------------------------------------------------------------------

template<std::size_t Size>
inline bit::memory::owner<bit::memory::memory_block>
  bit::memory::new_block_allocator<Size>::allocate_block()
  noexcept
{
  auto p = ::operator new(block_size_member::value(), std::nothrow);

  if( BIT_MEMORY_UNLIKELY(!p) ) return nullblock;
  return {p, block_size_member::value() };
}

template<std::size_t Size>
inline void bit::memory::new_block_allocator<Size>
  ::deallocate_block( owner<memory_block> block )
  noexcept
{
  ::operator delete( block.data() );
}


//-----------------------------------------------------------------------------
// Comparisons
//-----------------------------------------------------------------------------

template<std::size_t Size>
inline bool bit::memory::operator==( const new_block_allocator<Size>& lhs,
                                     const new_block_allocator<Size>& rhs )
  noexcept
{
  return lhs.value() == rhs.value();
}

template<std::size_t Size>
inline bool bit::memory::operator!=( const new_block_allocator<Size>& lhs,
                                     const new_block_allocator<Size>& rhs )
  noexcept
{
  return !(lhs == rhs.value);
}

#endif /* BIT_MEMORY_BLOCK_ALLOCATORS_DETAIL_NEW_BLOCK_ALLOCATOR_INL */
