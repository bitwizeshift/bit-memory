#ifndef BIT_MEMORY_DETAIL_FALLBACK_BLOCK_ALLOCATOR_INL
#define BIT_MEMORY_DETAIL_FALLBACK_BLOCK_ALLOCATOR_INL

//----------------------------------------------------------------------------
// Constructors
//----------------------------------------------------------------------------

template<typename BlockAllocator0, typename...BlockAllocatorN>
inline bit::memory::fallback_block_allocator<BlockAllocator0, BlockAllocatorN...>
  ::fallback_block_allocator( BlockAllocator0& a0, BlockAllocatorN&...an )
  : m_storage(&a0,&an...)
{

}

//----------------------------------------------------------------------------
// Block Allocations
//----------------------------------------------------------------------------

template<typename BlockAllocator0, typename...BlockAllocatorN>
inline bit::memory::owner<bit::memory::memory_block>
  bit::memory::fallback_block_allocator<BlockAllocator0,BlockAllocatorN...>
  ::allocate_block()
{
  return allocate_block( std::index_sequence_for<BlockAllocator0,BlockAllocatorN...>() );
}

//----------------------------------------------------------------------------

template<typename BlockAllocator0, typename...BlockAllocatorN>
inline void bit::memory::fallback_block_allocator<BlockAllocator0,BlockAllocatorN...>
  ::deallocate_block( owner<memory_block> block )
{
  return deallocate_block( block, std::index_sequence_for<BlockAllocator0,BlockAllocatorN...>() );
}

//----------------------------------------------------------------------------
// Private Allocations
//----------------------------------------------------------------------------

template<typename BlockAllocator0, typename...BlockAllocatorN>
template<std::size_t Idx, std::size_t...Idxs>
inline bit::memory::owner<bit::memory::memory_block>
  bit::memory::fallback_block_allocator<BlockAllocator0,BlockAllocatorN...>
  ::allocate_block( std::index_sequence<Idx,Idxs...> )
{
  auto block = std::get<Idx>(m_storage)->allocate_block();

  if( block ) return block;
  return allocate_block( std::index_sequence<Idxs...>() );
}

//----------------------------------------------------------------------------

template<typename BlockAllocator0, typename...BlockAllocatorN>
template<std::size_t Idx>
inline bit::memory::owner<bit::memory::memory_block>
  bit::memory::fallback_block_allocator<BlockAllocator0,BlockAllocatorN...>
  ::allocate_block( std::index_sequence<Idx> )
{
  return std::get<Idx>(m_storage)->allocate_block();
}

//----------------------------------------------------------------------------
// Private Deallocations
//----------------------------------------------------------------------------

template<typename BlockAllocator0, typename...BlockAllocatorN>
template<std::size_t Idx, std::size_t...Idxs>
inline void bit::memory::fallback_block_allocator<BlockAllocator0,BlockAllocatorN...>
  ::deallocate_block( owner<memory_block> block, std::index_sequence<Idx,Idxs...> )
{
//  if( std::get<Idx>(m_storage) == block.origin() ) {
//    std::get<Idx>(m_storage)->deallocate_block( block );
//  } else {
//    deallocate_block( block, std::index_sequence<Idxs...>() );
//  }
}

//----------------------------------------------------------------------------

template<typename BlockAllocator0, typename...BlockAllocatorN>
template<std::size_t Idx>
inline void bit::memory::fallback_block_allocator<BlockAllocator0,BlockAllocatorN...>
  ::deallocate_block( owner<memory_block> block, std::index_sequence<Idx> )
{
//  if( std::get<Idx>(m_storage) == block.origin() ) {
//    std::get<Idx>(m_storage)->deallocate_block( block );
//  }
}

#endif /* BIT_MEMORY_DETAIL_FALLBACK_BLOCK_ALLOCATOR_INL */
