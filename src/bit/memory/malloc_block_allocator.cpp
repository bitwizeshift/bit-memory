#include <bit/memory/malloc_block_allocator.hpp>

#include <cstdlib> // std::malloc

//----------------------------------------------------------------------------
// Constructors
//----------------------------------------------------------------------------

bit::memory::malloc_block_allocator::malloc_block_allocator( std::size_t n )
  noexcept
  : m_size(n)
{

}

//----------------------------------------------------------------------------
// Block Allocations
//----------------------------------------------------------------------------

bit::memory::owner<bit::memory::memory_block>
  bit::memory::malloc_block_allocator::allocate_block()
{
  auto p = std::malloc(m_size);
  if( !p ) return nullblock;

  return {p, m_size};
}

void bit::memory::malloc_block_allocator::deallocate_block( owner<memory_block> block )
{
  std::free( block.data() );
}
