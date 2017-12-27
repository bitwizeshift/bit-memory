#ifndef BIT_MEMORY_BLOCK_ALLOCATORS_DETAIL_STATIC_BLOCK_ALLOCATOR_INL
#define BIT_MEMORY_BLOCK_ALLOCATORS_DETAIL_STATIC_BLOCK_ALLOCATOR_INL

//-----------------------------------------------------------------------------
// Static Variables
//-----------------------------------------------------------------------------

template<std::size_t BlockSize, std::size_t Blocks, std::size_t Align, typename Tag>
alignas(Align) char bit::memory::static_block_allocator<BlockSize,Blocks,Align,Tag>
  ::s_storage[BlockSize*Blocks] = {};

//-----------------------------------------------------------------------------
// Block Allocation
//-----------------------------------------------------------------------------

template<std::size_t BlockSize, std::size_t Blocks, std::size_t Align, typename Tag>
inline bit::memory::owner<bit::memory::memory_block>
  bit::memory::static_block_allocator<BlockSize,Blocks,Align,Tag>
  ::allocate_block()
  noexcept
{
  return block_cache().request_block();
}

//-----------------------------------------------------------------------------

template<std::size_t BlockSize, std::size_t Blocks, std::size_t Align, typename Tag>
inline void bit::memory::static_block_allocator<BlockSize,Blocks,Align,Tag>
  ::deallocate_block( owner<memory_block> block )
  noexcept
{
  block_cache().store_block( block );
}

//-----------------------------------------------------------------------------
// Observers
//-----------------------------------------------------------------------------

template<std::size_t BlockSize, std::size_t Blocks, std::size_t Align, typename Tag>
inline std::size_t bit::memory::static_block_allocator<BlockSize,Blocks,Align,Tag>
  ::next_block_size()
  const noexcept
{
  auto& cache = block_cache();

  if( cache.empty() ) {
    return 0;
  }
  return BlockSize;
}

template<std::size_t BlockSize, std::size_t Blocks, std::size_t Align, typename Tag>
inline bit::memory::allocator_info
  bit::memory::static_block_allocator<BlockSize,Blocks,Align,Tag>::info()
  const noexcept
{
  return {"static_block_allocator",this};
}

//-----------------------------------------------------------------------------
// Private Static Member Functions
//-----------------------------------------------------------------------------

template<std::size_t BlockSize, std::size_t Blocks, std::size_t Align, typename Tag>
inline bit::memory::memory_block_cache&
  bit::memory::static_block_allocator<BlockSize,Blocks,Align,Tag>::block_cache()
  noexcept
{
  static auto cache = []()
  {
    auto temp_cache = memory_block_cache{};
    for( auto i=0; i<Blocks; ++i ) {
      auto* p = static_cast<void*>(&s_storage[i * BlockSize]);

      temp_cache.store_block( {p, BlockSize} );
    }

    return temp_cache;
  }();

  return cache;
}

#endif /* BIT_MEMORY_BLOCK_ALLOCATORS_DETAIL_STATIC_BLOCK_ALLOCATOR_INL */
