#ifndef BIT_MEMORY_BLOCK_ALLOCATORS_DETAIL_ALIGNED_BLOCK_ALLOCATOR_INL
#define BIT_MEMORY_BLOCK_ALLOCATORS_DETAIL_ALIGNED_BLOCK_ALLOCATOR_INL

//=============================================================================
// aligned_block_allocator
//=============================================================================

//-----------------------------------------------------------------------------
// Block Allocations
//-----------------------------------------------------------------------------

template<std::size_t Size, std::size_t Align>
inline bit::memory::memory_block
  bit::memory::aligned_block_allocator<Size,Align>
  ::allocate_block()
  noexcept
{
  const auto size = next_block_size();

  auto p = aligned_malloc(size,Align);

  if( BIT_MEMORY_UNLIKELY(!p) ) return nullblock;
  return {p, size};
}

//-----------------------------------------------------------------------------

template<std::size_t Size, std::size_t Align>
inline void
  bit::memory::aligned_block_allocator<Size,Align>
  ::deallocate_block( memory_block block )
  noexcept
{
  aligned_free( block.data() );
}

//-----------------------------------------------------------------------------
// Observers
//-----------------------------------------------------------------------------

template<std::size_t Size, std::size_t Align>
inline std::size_t
  bit::memory::aligned_block_allocator<Size,Align>::next_block_size()
  const noexcept
{
  return block_size_member::value();
}


template<std::size_t Size, std::size_t Align>
inline bit::memory::allocator_info
  bit::memory::aligned_block_allocator<Size,Align>::info()
  const noexcept
{
  return {"aligned_block_allocator",this};
}

#endif /* BIT_MEMORY_BLOCK_ALLOCATORS_DETAIL_ALIGNED_BLOCK_ALLOCATOR_INL */
