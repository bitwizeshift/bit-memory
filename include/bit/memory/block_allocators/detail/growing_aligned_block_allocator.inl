#ifndef BIT_MEMORY_BLOCK_ALLOCATORS_DETAIL_GROWING_ALIGNED_BLOCK_ALLOCATOR_INL
#define BIT_MEMORY_BLOCK_ALLOCATORS_DETAIL_GROWING_ALIGNED_BLOCK_ALLOCATOR_INL

//=============================================================================
// growing_aligned_block_allocator
//=============================================================================

//-----------------------------------------------------------------------------
// Block Allocations
//-----------------------------------------------------------------------------

template<std::size_t Size, std::size_t Align>
inline bit::memory::memory_block
  bit::memory::growing_aligned_block_allocator<Size,Align>
  ::allocate_block()
  noexcept
{
  const auto size = next_block_size();
  grow();

  auto p = aligned_malloc(size,Align);

  if( BIT_MEMORY_UNLIKELY(!p) ) return nullblock;
  return {p, size};
}

//-----------------------------------------------------------------------------

template<std::size_t Size, std::size_t Align>
inline void
  bit::memory::growing_aligned_block_allocator<Size,Align>
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
  bit::memory::growing_aligned_block_allocator<Size,Align>::next_block_size()
  const noexcept
{
  return block_size_member::value() * base_type::m_multiplier;
}

template<std::size_t Size, std::size_t Align>
inline bit::memory::allocator_info
  bit::memory::growing_aligned_block_allocator<Size,Align>::info()
  const noexcept
{
  return {"growing_aligned_block_allocator",this};
}

//-----------------------------------------------------------------------------
// Private Member Functions
//-----------------------------------------------------------------------------

template<std::size_t Size, std::size_t Align>
inline void bit::memory::growing_aligned_block_allocator<Size,Align>::grow()
{
  if( base_type::m_growths_remaining == 0 ) return;

  --base_type::m_growths_remaining;
  base_type::m_multiplier *= 2;
}

#endif /* BIT_MEMORY_BLOCK_ALLOCATORS_DETAIL_GROWING_ALIGNED_BLOCK_ALLOCATOR_INL */
