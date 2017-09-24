#include <bit/memory/new_block_allocator.hpp>

#include <new> // ::new

//----------------------------------------------------------------------------
// Constructors
//----------------------------------------------------------------------------

bit::memory::new_block_allocator::new_block_allocator( std::size_t size )
  noexcept
  : m_size( size )
{

}

//----------------------------------------------------------------------------
// Block Allocations
//----------------------------------------------------------------------------

bit::memory::owner<bit::memory::memory_block>
  bit::memory::new_block_allocator::allocate_block()
{
  auto p = ::operator new(m_size,std::nothrow);

  if( !p ) return nullblock;
  return {p, m_size};
}

void bit::memory::new_block_allocator::deallocate_block( owner<memory_block> block )
{
  ::operator delete( block.data() );
}
