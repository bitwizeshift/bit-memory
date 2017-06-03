#include <bit/memory/virtual_block_allocator.hpp>

//============================================================================
// virtual_block_allocator
//============================================================================

const std::size_t bit::memory::virtual_block_allocator::align_size = bit::memory::virtual_memory_page_size;
const std::size_t bit::memory::virtual_block_allocator::block_size = bit::memory::virtual_memory_page_size;

//----------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------

bit::memory::virtual_block_allocator
  ::virtual_block_allocator( std::size_t pages )
{
  auto ptr = (char*) virtual_memory_reserve( pages );

  for( auto i = 0; i < pages; ++i ) {
    m_cache.store_block( memory_block(ptr, block_size, this) );
    ptr += block_size;
  }
}

bit::memory::virtual_block_allocator
  ::~virtual_block_allocator()
{
  for( auto block : m_cache ) {
    virtual_memory_release( block.data(), block.size() );
  }
}

//----------------------------------------------------------------------------
// Block Allocations
//----------------------------------------------------------------------------

bit::memory::owner<bit::memory::memory_block>
  bit::memory::virtual_block_allocator::allocate_block()
  noexcept
{
  auto block = m_cache.request_block();

  virtual_memory_commit( block.data(), block.size() );

  return block;
}

void bit::memory::virtual_block_allocator::deallocate_block( owner<memory_block> block )
  noexcept
{
  virtual_memory_decommit( block.data(), block.size() );

  m_cache.store_block( block );
}
