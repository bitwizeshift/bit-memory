#include <bit/memory/block_allocators/malloc_block_allocator.hpp>

#include <bit/memory/macros.hpp> // BIT_MEMORY_UNLIKELY

#include <cstdlib> // std::malloc

//----------------------------------------------------------------------------
// Block Allocations
//----------------------------------------------------------------------------

bit::memory::owner<bit::memory::memory_block>
  bit::memory::malloc_block_allocator::allocate_block()
{
  auto p = std::malloc(m_size);
  if( BIT_MEMORY_UNLIKELY(!p) ) return nullblock;

  return {p, m_size};
}

void bit::memory::malloc_block_allocator::deallocate_block( owner<memory_block> block )
{
  std::free( block.data() );
}
