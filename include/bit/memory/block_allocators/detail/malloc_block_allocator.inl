#ifndef BIT_MEMORY_BLOCK_ALLOCATORS_DETAIL_MALLOC_BLOCK_ALLOCATOR_INL
#define BIT_MEMORY_BLOCK_ALLOCATORS_DETAIL_MALLOC_BLOCK_ALLOCATOR_INL

//-----------------------------------------------------------------------------
// Block Allocations
//-----------------------------------------------------------------------------

template<std::size_t Size>
inline bit::memory::owner<bit::memory::memory_block>
  bit::memory::malloc_block_allocator<Size>::allocate_block()
  noexcept
{
  const auto size = next_block_size();

  auto p = std::malloc( size );

  if( BIT_MEMORY_UNLIKELY(!p) ) return nullblock;
  return { p, size };
}

template<std::size_t Size>
inline void bit::memory::malloc_block_allocator<Size>
  ::deallocate_block( owner<memory_block> block )
  noexcept
{
  std::free( block.data() );
}

//-----------------------------------------------------------------------------
// Observers
//-----------------------------------------------------------------------------

template<std::size_t Size>
inline std::size_t bit::memory::malloc_block_allocator<Size>
  ::next_block_size()
   const noexcept
{
  return block_size_member::value();
}

template<std::size_t Size>
inline bit::memory::allocator_info
  bit::memory::malloc_block_allocator<Size>::info()
  const noexcept
{
  return {"malloc_block_allocator",this};
}

#endif /* BIT_MEMORY_BLOCK_ALLOCATORS_DETAIL_MALLOC_BLOCK_ALLOCATOR_INL */
