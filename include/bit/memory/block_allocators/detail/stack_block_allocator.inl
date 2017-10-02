#ifndef BIT_MEMORY_BLOCK_ALLOCATORS_DETAIL_STACK_BLOCK_ALLOCATOR_INL
#define BIT_MEMORY_BLOCK_ALLOCATORS_DETAIL_STACK_BLOCK_ALLOCATOR_INL

//-----------------------------------------------------------------------------
// Constructors
//-----------------------------------------------------------------------------

template<std::size_t BlockSize, std::size_t Blocks, std::size_t Align>
inline bit::memory::stack_block_allocator<BlockSize,Blocks,Align>
  ::stack_block_allocator()
  noexcept
  : m_index(0)
{

}

//-----------------------------------------------------------------------------
// Block Allocations
//-----------------------------------------------------------------------------

template<std::size_t BlockSize, std::size_t Blocks, std::size_t Align>
inline bit::memory::owner<bit::memory::memory_block>
  bit::memory::stack_block_allocator<BlockSize,Blocks,Align>::allocate_block()
  noexcept
{
  if( m_index > Blocks ) return nullblock;

  auto* p = static_cast<void*>(&m_storage[m_index * BlockSize]);
  ++m_index;

  return { p, BlockSize };
}

template<std::size_t BlockSize, std::size_t Blocks, std::size_t Align>
inline void bit::memory::stack_block_allocator<BlockSize,Blocks,Align>
  ::deallocate_block( owner<memory_block> block )
  noexcept
{
  BIT_MEMORY_UNUSED(block);

  // memory is automatically reclaimed; do nothing
}

#endif /* BIT_MEMORY_BLOCK_ALLOCATORS_DETAIL_STACK_BLOCK_ALLOCATOR_INL */
