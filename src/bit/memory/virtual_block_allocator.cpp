#include <bit/memory/virtual_block_allocator.hpp>

//============================================================================
// virtual_block_allocator
//============================================================================

const std::size_t bit::memory::virtual_block_allocator::block_size = bit::memory::virtual_memory_page_size;
const std::size_t bit::memory::virtual_block_allocator::align_size = bit::memory::virtual_memory_page_size;

//----------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------

bit::memory::virtual_block_allocator
  ::virtual_block_allocator( std::size_t pages )
  : m_memory( virtual_memory_reserve( pages ) ),
    m_pages(pages),
    m_active_page(0)
{

}

bit::memory::virtual_block_allocator::~virtual_block_allocator()
{
  // Releasing also decommits memory
  virtual_memory_release( m_memory, m_pages );
}

//----------------------------------------------------------------------------
// Block Allocations
//----------------------------------------------------------------------------

bit::memory::owner<bit::memory::memory_block>
  bit::memory::virtual_block_allocator::allocate_block()
  noexcept
{
  if( !m_cache.empty() ) {
    return m_cache.request_block();
  }

  if( (static_cast<std::size_t>(m_active_page) < m_pages) ) {
    auto v = static_cast<byte*>(m_memory) + (m_active_page++ * virtual_memory_page_size);
    auto p = virtual_memory_commit( v, 1 );

    return {p, virtual_memory_page_size};
  }

  return nullblock;
}

void bit::memory::virtual_block_allocator::deallocate_block( owner<memory_block> block )
  noexcept
{
  m_cache.store_block( block );
}
