#include <bit/memory/malloc_block_allocator.hpp>

#include <cstdlib> // std::malloc

bit::memory::malloc_block_allocator::malloc_block_allocator( std::size_t n )
  noexcept
  : m_size(n)
{

}

bit::memory::owner<bit::memory::memory_block>
  bit::memory::malloc_block_allocator::allocate_block()
{
  return memory_block( std::malloc(m_size), m_size, this );
}

void bit::memory::malloc_block_allocator::deallocate_block( owner<memory_block> block )
{
  std::free( block.data() );
}
