#ifndef BIT_MEMORY_BLOCK_ALLOCATORS_DETAIL_MALLOC_BLOCK_ALLOCATOR_INL
#define BIT_MEMORY_BLOCK_ALLOCATORS_DETAIL_MALLOC_BLOCK_ALLOCATOR_INL

//-----------------------------------------------------------------------------
// Constructors
//-----------------------------------------------------------------------------

template<std::size_t DefaultBlockSize, typename GrowthMultiplier>
template<std::size_t USize, typename>
inline bit::memory::malloc_block_allocator<DefaultBlockSize,GrowthMultiplier>
  ::malloc_block_allocator( GrowthMultiplier growth )
  : base_type( std::forward_as_tuple(growth) )
{

}

template<std::size_t DefaultBlockSize, typename GrowthMultiplier>
template<std::size_t USize, typename>
inline bit::memory::malloc_block_allocator<DefaultBlockSize,GrowthMultiplier>
  ::malloc_block_allocator( block_size_t block_size, GrowthMultiplier growth )
   : base_type(),
     block_size_member( static_cast<std::size_t>(block_size) )
{

}

//-----------------------------------------------------------------------------
// Block Allocations
//-----------------------------------------------------------------------------

template<std::size_t DefaultBlockSize, typename GrowthMultiplier>
inline bit::memory::owner<bit::memory::memory_block>
  bit::memory::malloc_block_allocator<DefaultBlockSize,GrowthMultiplier>
  ::allocate_block()
  noexcept
{
  using ::bit::memory::get;

  const auto size = next_block_size();

  auto p = std::malloc( size );
  get<0>(*this).grow();

  if( BIT_MEMORY_UNLIKELY(!p) ) return nullblock;
  return { p, size };
}

template<std::size_t DefaultBlockSize, typename GrowthMultiplier>
inline void bit::memory::malloc_block_allocator<DefaultBlockSize,GrowthMultiplier>
  ::deallocate_block( owner<memory_block> block )
  noexcept
{
  std::free( block.data() );
}

//-----------------------------------------------------------------------------
// Observers
//-----------------------------------------------------------------------------

template<std::size_t DefaultBlockSize, typename GrowthMultiplier>
inline std::size_t bit::memory::malloc_block_allocator<DefaultBlockSize,GrowthMultiplier>
  ::next_block_size()
   const noexcept
{
  using ::bit::memory::get;

  return block_size_member::value() * get<0>(*this).multiplier();
}

template<std::size_t DefaultBlockSize, typename GrowthMultiplier>
inline bit::memory::allocator_info
  bit::memory::malloc_block_allocator<DefaultBlockSize,GrowthMultiplier>::info()
  const noexcept
{
  return {"malloc_block_allocator",this};
}

#endif /* BIT_MEMORY_BLOCK_ALLOCATORS_DETAIL_MALLOC_BLOCK_ALLOCATOR_INL */
