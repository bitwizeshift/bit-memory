#ifndef BIT_MEMORY_BLOCK_ALLOCATORS_DETAIL_GROWING_NEW_BLOCK_ALLOCATOR_INL
#define BIT_MEMORY_BLOCK_ALLOCATORS_DETAIL_GROWING_NEW_BLOCK_ALLOCATOR_INL

//-----------------------------------------------------------------------------
// Block Allocations
//-----------------------------------------------------------------------------

template<std::size_t Size>
inline bit::memory::owner<bit::memory::memory_block>
  bit::memory::growing_new_block_allocator<Size>::allocate_block()
  noexcept
{
  const auto size = next_block_size();
  grow();

  auto p = ::operator new( size, std::nothrow );

  if( BIT_MEMORY_UNLIKELY(!p) ) return nullblock;
  return { p, size };
}

template<std::size_t Size>
inline void bit::memory::growing_new_block_allocator<Size>
  ::deallocate_block( owner<memory_block> block )
  noexcept
{
  std::free( block.data() );
}

//-----------------------------------------------------------------------------
// Observers
//-----------------------------------------------------------------------------

template<std::size_t Size>
inline std::size_t bit::memory::growing_new_block_allocator<Size>
  ::next_block_size()
   const noexcept
{
  return block_size_member::value() * base_type::m_multiplier;
}

template<std::size_t Size>
inline bit::memory::allocator_info
  bit::memory::growing_new_block_allocator<Size>::info()
   const noexcept
{
  return {"growing_new_block_allocator",this};
}

//-----------------------------------------------------------------------------
// Private Member Functions
//-----------------------------------------------------------------------------

template<std::size_t Size>
inline void bit::memory::growing_new_block_allocator<Size>::grow()
{
  if( base_type::m_growths_remaining == 0 ) return;

  --base_type::m_growths_remaining;
  base_type::m_multiplier *= 2;
}

#endif /* BIT_MEMORY_BLOCK_ALLOCATORS_DETAIL_GROWING_NEW_BLOCK_ALLOCATOR_INL */
