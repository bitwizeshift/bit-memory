#include <bit/memory/block_allocators/growing_virtual_block_allocator.hpp>

#include <bit/memory/virtual_memory.hpp> // virtual_alloc, etc

#include <algorithm> // std::min
#include <cassert>   // assert

//=============================================================================
// growing_growing_virtual_block_allocator
//=============================================================================

//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------

bit::memory::growing_virtual_block_allocator
  ::growing_virtual_block_allocator( std::size_t pages )
  : m_memory( virtual_memory_reserve( pages ) ),
    m_pages(pages),
    m_active_page(0),
    m_multiplier(1)
{

}

bit::memory::growing_virtual_block_allocator
  ::growing_virtual_block_allocator( growing_virtual_block_allocator&& other )
  noexcept
  : m_memory( other.m_memory ),
    m_pages( other.m_pages ),
    m_active_page( other.m_active_page ),
    m_multiplier( other.m_multiplier ),
    m_cache( std::move(other.m_cache) )
{
  other.m_memory      = nullptr;
  other.m_active_page = 0;
}

bit::memory::growing_virtual_block_allocator::~growing_virtual_block_allocator()
{
  if( m_memory ) {
    // Releasing also decommits memory
    virtual_memory_release( m_memory, m_pages );
  }
}

//-----------------------------------------------------------------------------
// Block Allocations
//-----------------------------------------------------------------------------

bit::memory::owner<bit::memory::memory_block>
  bit::memory::growing_virtual_block_allocator::allocate_block()
  noexcept
{
  using byte_t = unsigned char;

  if( !m_cache.empty() ) {
    return m_cache.request_block();
  }

  if( (static_cast<std::size_t>(m_active_page) < m_pages) ) {

    const auto pages      = std::min(m_multiplier,(m_pages - m_active_page));
    const auto page_size  = virtual_memory_page_size();
    const auto block_size = page_size * pages;

    auto v = static_cast<byte_t*>(m_memory) + (m_active_page * page_size);
    auto p = virtual_memory_commit( v, pages );

    m_active_page = std::min(m_active_page + m_multiplier, m_pages);

    if( static_cast<std::size_t>(m_active_page) < m_pages ) {
      m_multiplier *= 2;
    }

    return {p, block_size};
  }

  return nullblock;
}

void bit::memory::growing_virtual_block_allocator::deallocate_block( owner<memory_block> block )
  noexcept
{
  m_cache.store_block( block );
}

//-----------------------------------------------------------------------------
// Observers
//-----------------------------------------------------------------------------

std::size_t bit::memory::growing_virtual_block_allocator::next_block_size()
  const noexcept
{
  return virtual_memory_page_size() * static_cast<std::size_t>(m_active_page + 1);
}

void bit::memory::growing_virtual_block_allocator::grow()
{
  if( m_active_page == 0 ) {
    ++m_active_page;
  } else {
    m_active_page += 2 * m_active_page;
  }
}
