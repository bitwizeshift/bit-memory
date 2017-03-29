#include <bit/memory/virtual_memory.hpp>

//============================================================================
// virtual_memory
//============================================================================

const std::size_t bit::memory::virtual_memory::page_size = bit::memory::virtual_memory_page_size;

//----------------------------------------------------------------------------
// Constructors / Destructors
//----------------------------------------------------------------------------

bit::memory::virtual_memory::virtual_memory( std::size_t pages )
  : m_data(virtual_memory_reserve(pages)),
    m_pages(pages)
{

}

bit::memory::virtual_memory::virtual_memory( virtual_memory&& other )
  noexcept
  : m_data(other.m_data),
    m_pages(other.m_pages)
{
  m_data  = nullptr;
  m_pages = 0;
}

bit::memory::virtual_memory::~virtual_memory()
{
  if(m_data) {
    virtual_memory_release(m_data,m_pages);
  }
}

//----------------------------------------------------------------------------

bit::memory::virtual_memory&
  bit::memory::virtual_memory::operator=( virtual_memory&& other )
  noexcept
{
  m_data  = other.m_data;
  m_pages = other.m_pages;
  other.m_data  = nullptr;
  other.m_pages = 0;

  return (*this);
}

//----------------------------------------------------------------------------
// Modifiers
//----------------------------------------------------------------------------

void bit::memory::virtual_memory::commit( std::ptrdiff_t n )
  noexcept
{
  auto ptr = static_cast<char*>(m_data) + (n * virtual_memory_page_size);

  virtual_memory_commit( ptr, 1 );
}


void bit::memory::virtual_memory::decommit( std::ptrdiff_t n )
  noexcept
{
  auto ptr = static_cast<char*>(m_data) + (n * virtual_memory_page_size);

  virtual_memory_decommit( ptr, 1 );
}

void* bit::memory::virtual_memory::release()
  noexcept
{
  auto copy = m_data;
  m_data = nullptr;

  return copy;
}

//----------------------------------------------------------------------------
// Element Access
//----------------------------------------------------------------------------

bit::memory::memory_block bit::memory::virtual_memory::at( std::ptrdiff_t n )
  const
{
  if( n < 0 || n >= m_pages ) throw std::out_of_range("virtual_memory::at: index out of bounds");

  auto ptr = static_cast<char*>(m_data) + (n * page_size);
  return memory_block{ m_data, page_size };
}

bit::memory::memory_block
  bit::memory::virtual_memory::operator[]( std::ptrdiff_t n )
  const noexcept
{
  auto ptr = static_cast<char*>(m_data) + (n * page_size);
  return memory_block{ m_data, page_size };
}

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
    m_cache.store_block( memory_block(ptr, block_size) );
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
