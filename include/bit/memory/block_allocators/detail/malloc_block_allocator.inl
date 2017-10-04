#ifndef BIT_MEMORY_BLOCK_ALLOCATORS_DETAIL_MALLOC_BLOCK_ALLOCATOR_INL
#define BIT_MEMORY_BLOCK_ALLOCATORS_DETAIL_MALLOC_BLOCK_ALLOCATOR_INL

//-----------------------------------------------------------------------------
// Constructors
//-----------------------------------------------------------------------------

template<std::size_t Size>
template<std::size_t,typename>
inline bit::memory::malloc_block_allocator<Size>
  ::malloc_block_allocator( std::size_t size )
  noexcept
  : block_size_member( size )
{

}

//-----------------------------------------------------------------------------
// Block Allocations
//-----------------------------------------------------------------------------

template<std::size_t Size>
inline bit::memory::owner<bit::memory::memory_block>
  bit::memory::malloc_block_allocator<Size>::allocate_block()
  noexcept
{
  auto p = std::malloc( block_size_member::value() );

  if( BIT_MEMORY_UNLIKELY(!p) ) return nullblock;
  return { p, block_size_member::value() };
}

template<std::size_t Size>
inline void bit::memory::malloc_block_allocator<Size>
  ::deallocate_block( owner<memory_block> block )
  noexcept
{
  std::free( block.data() );
}


//-----------------------------------------------------------------------------
// Comparisons
//-----------------------------------------------------------------------------

template<std::size_t Size>
inline bool bit::memory::operator==( const malloc_block_allocator<Size>& lhs,
                                     const malloc_block_allocator<Size>& rhs )
  noexcept
{
  return lhs.value() == rhs.value();
}

template<std::size_t Size>
inline bool bit::memory::operator!=( const malloc_block_allocator<Size>& lhs,
                                     const malloc_block_allocator<Size>& rhs )
  noexcept
{
  return !(lhs == rhs.value);
}


#endif /* BIT_MEMORY_BLOCK_ALLOCATORS_DETAIL_MALLOC_BLOCK_ALLOCATOR_INL */
