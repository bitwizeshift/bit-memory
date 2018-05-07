#ifndef BIT_MEMORY_BLOCK_ALLOCATORS_DETAIL_ALIGNED_BLOCK_ALLOCATOR_INL
#define BIT_MEMORY_BLOCK_ALLOCATORS_DETAIL_ALIGNED_BLOCK_ALLOCATOR_INL

//=============================================================================
// aligned_block_allocator
//=============================================================================

//-----------------------------------------------------------------------------
// Constructors
//-----------------------------------------------------------------------------

template<std::size_t DefaultBlockSize, std::size_t BlockAlign, typename GrowthMultiplier>
template<std::size_t USize, std::size_t UAlign, typename>
inline bit::memory::aligned_block_allocator<DefaultBlockSize,BlockAlign,GrowthMultiplier>
  ::aligned_block_allocator( GrowthMultiplier growth )
  : base_type( std::forward_as_tuple(growth) )
{

}

template<std::size_t DefaultBlockSize, std::size_t BlockAlign, typename GrowthMultiplier>
template<std::size_t USize, typename>
inline bit::memory::aligned_block_allocator<DefaultBlockSize,BlockAlign,GrowthMultiplier>
  ::aligned_block_allocator( block_size_t block_size,
                             GrowthMultiplier growth )
  : base_type( std::forward_as_tuple(growth) ),
    block_size_member( static_cast<std::size_t>(block_size) )
{

}

template<std::size_t DefaultBlockSize, std::size_t BlockAlign, typename GrowthMultiplier>
template<std::size_t UAlign, typename>
inline bit::memory::aligned_block_allocator<DefaultBlockSize,BlockAlign,GrowthMultiplier>
  ::aligned_block_allocator( block_alignment_t block_alignment,
                             GrowthMultiplier growth )
  : base_type( std::forward_as_tuple(growth) ),
    block_align_member( static_cast<std::size_t>(block_alignment) )
{

}

template<std::size_t DefaultBlockSize, std::size_t BlockAlign, typename GrowthMultiplier>
template<std::size_t USize, std::size_t UAlign, typename>
inline bit::memory::aligned_block_allocator<DefaultBlockSize,BlockAlign,GrowthMultiplier>
  ::aligned_block_allocator( block_size_t block_size,
                             block_alignment_t block_alignment,
                             GrowthMultiplier growth )
  : base_type( std::forward_as_tuple(growth) ),
    block_size_member( static_cast<std::size_t>(block_size) ),
    block_align_member( static_cast<std::size_t>(block_alignment) )
{

}

//-----------------------------------------------------------------------------
// Block Allocations
//-----------------------------------------------------------------------------

template<std::size_t DefaultBlockSize, std::size_t BlockAlign, typename GrowthMultiplier>
inline bit::memory::memory_block
  bit::memory::aligned_block_allocator<DefaultBlockSize,BlockAlign,GrowthMultiplier>
  ::allocate_block()
  noexcept
{
  using ::bit::memory::get;
  
  const auto size = next_block_size();

  auto p = aligned_malloc(size,BlockAlign);
  get<0>(*this).grow();

  if( BIT_MEMORY_UNLIKELY(!p) ) return nullblock;
  return {p, size};
}

//-----------------------------------------------------------------------------

template<std::size_t DefaultBlockSize, std::size_t BlockAlign, typename GrowthMultiplier>
inline void
  bit::memory::aligned_block_allocator<DefaultBlockSize,BlockAlign,GrowthMultiplier>
  ::deallocate_block( memory_block block )
  noexcept
{
  aligned_free( block.data() );
}

//-----------------------------------------------------------------------------
// Observers
//-----------------------------------------------------------------------------

template<std::size_t DefaultBlockSize, std::size_t BlockAlign, typename GrowthMultiplier>
inline std::size_t
  bit::memory::aligned_block_allocator<DefaultBlockSize,BlockAlign,GrowthMultiplier>
  ::next_block_size()
  const noexcept
{
  using ::bit::memory::get;

  return block_size_member::value() * get<0>(*this).multiplier();
}


template<std::size_t DefaultBlockSize, std::size_t BlockAlign, typename GrowthMultiplier>
inline bit::memory::allocator_info
  bit::memory::aligned_block_allocator<DefaultBlockSize,BlockAlign,GrowthMultiplier>
  ::info()
  const noexcept
{
  return {"aligned_block_allocator",this};
}

#endif /* BIT_MEMORY_BLOCK_ALLOCATORS_DETAIL_ALIGNED_BLOCK_ALLOCATOR_INL */
