#include <bit/memory/block_allocators/new_block_allocator.hpp>

#include <bit/memory/macros.hpp> // BIT_MEMORY_UNLIKELY
#include <new> // ::new

//-----------------------------------------------------------------------------
// Block Allocations
//-----------------------------------------------------------------------------

bit::memory::owner<bit::memory::memory_block>
  bit::memory::new_block_allocator::allocate_block()
  noexcept
{
  auto p = ::operator new(m_size,std::nothrow);

  if( BIT_MEMORY_UNLIKELY(!p) ) return nullblock;
  return {p, m_size};
}

void bit::memory::new_block_allocator
  ::deallocate_block( owner<memory_block> block )
  noexcept
{
  ::operator delete( block.data() );
}
