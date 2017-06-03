#include <bit/memory/new_block_allocator.hpp>

#include <new> // ::new

bit::memory::owner<bit::memory::memory_block>
  bit::memory::new_block_allocator::allocate_block()
{
  return memory_block( ::operator new(m_size), m_size, this );
}

void bit::memory::new_block_allocator::deallocate_block( owner<memory_block> block )
{
  ::operator delete( block.data() );
}
