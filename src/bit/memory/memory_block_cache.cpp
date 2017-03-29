#include <bit/memory/memory_block_cache.hpp>


//============================================================================
// memory_block_cache_iterator
//============================================================================

//----------------------------------------------------------------------------
// Constructors
//----------------------------------------------------------------------------

bit::memory::memory_block_cache_iterator::memory_block_cache_iterator()
{

}

bit::memory::memory_block_cache_iterator
  ::memory_block_cache_iterator( memory_block block )
  : m_block(block)
{

}

//----------------------------------------------------------------------------
// Member Functions
//----------------------------------------------------------------------------

bit::memory::memory_block_cache_iterator::reference
  bit::memory::memory_block_cache_iterator::operator*()
  noexcept
{
  return m_block;
}

typename bit::memory::memory_block_cache_iterator::pointer
  bit::memory::memory_block_cache_iterator::operator->()
  noexcept
{
  return &m_block;
}

//----------------------------------------------------------------------------

bit::memory::memory_block_cache_iterator&
  bit::memory::memory_block_cache_iterator::operator++()
  noexcept
{
  m_block = (*static_cast<memory_block*>(m_block.data()));
  return (*this);
}


bit::memory::memory_block_cache_iterator
  bit::memory::memory_block_cache_iterator::operator++(int)
  noexcept
{
  auto copy = (*this);
  ++(*this);
  return copy;
}

//----------------------------------------------------------------------------
// Comparison
//----------------------------------------------------------------------------

bool bit::memory::operator==( const memory_block_cache_iterator& lhs,
                              const memory_block_cache_iterator& rhs )
  noexcept
{
  return lhs.m_block == rhs.m_block;
}

bool bit::memory::operator!=( const memory_block_cache_iterator& lhs,
                              const memory_block_cache_iterator& rhs )
  noexcept
{
  return !(lhs==rhs);
}


//============================================================================
// memory_block_cache
//============================================================================

//----------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------

bit::memory::memory_block_cache::memory_block_cache()
  noexcept
  : m_head()
{

}

//----------------------------------------------------------------------------
// Observers
//----------------------------------------------------------------------------

bool bit::memory::memory_block_cache::empty()
  const noexcept
{
  return m_head.data();
}

//----------------------------------------------------------------------------

std::size_t bit::memory::memory_block_cache::size()
  const noexcept
{
  auto result = std::size_t{0};

  for( auto b = &m_head; b->data(); b = (const memory_block*) b->data() )
  {
    ++result;
  }
  return result;
}

//----------------------------------------------------------------------------

std::size_t bit::memory::memory_block_cache::size_bytes()
  const noexcept
{
  auto result = std::size_t{0};

  for( auto b = &m_head; b->data(); b = (const memory_block*) b->data() )
  {
    result += b->size();
  }
  return result;
}

//----------------------------------------------------------------------------

bool bit::memory::memory_block_cache::contains( const void* ptr )
  const noexcept
{
  for( auto b = &m_head; b->data(); b = (const memory_block*) b->data() )
  {
    if( b->contains(ptr) ) return true;
  }
  return false;
}

//----------------------------------------------------------------------------
// Caching
//----------------------------------------------------------------------------

bit::memory::owner<bit::memory::memory_block>
  bit::memory::memory_block_cache::request_block()
  noexcept
{
  auto block = m_head;
  if( m_head.data() ) {
    m_head = (*static_cast<memory_block*>(block.data()));
  }
  return block;
}

void bit::memory::memory_block_cache::steal_block( memory_block_cache& other )
  noexcept
{
  store_block( other.request_block() );
}

//----------------------------------------------------------------------------

void bit::memory::memory_block_cache::store_block( owner<memory_block> block )
  noexcept
{
  if( m_head.data() ) {
    (*static_cast<memory_block*>(block.data())) = m_head;
    m_head = block;
  } else {
    (*static_cast<memory_block*>(block.data())) = memory_block();
    m_head = block;
  }
}

//----------------------------------------------------------------------------
// Modifiers
//----------------------------------------------------------------------------

void bit::memory::memory_block_cache::swap( memory_block_cache& other )
  noexcept
{
  m_head.swap(other.m_head);
}

//----------------------------------------------------------------------------
// Iterators
//----------------------------------------------------------------------------

bit::memory::memory_block_cache::iterator
  bit::memory::memory_block_cache::begin()
  noexcept
{
  return iterator( m_head );
}

bit::memory::memory_block_cache::const_iterator
  bit::memory::memory_block_cache::begin()
  const noexcept
{
  return const_iterator( m_head );
}

bit::memory::memory_block_cache::const_iterator
  bit::memory::memory_block_cache::cbegin()
  const noexcept
{
  return const_iterator( m_head );
}

//----------------------------------------------------------------------------

bit::memory::memory_block_cache::iterator
  bit::memory::memory_block_cache::end()
  noexcept
{
  return iterator();
}

bit::memory::memory_block_cache::const_iterator
  bit::memory::memory_block_cache::end()
  const noexcept
{
  return const_iterator();
}

bit::memory::memory_block_cache::const_iterator
  bit::memory::memory_block_cache::cend()
  const noexcept
{
  return const_iterator();
}

//----------------------------------------------------------------------------
// Free Functions
//----------------------------------------------------------------------------

void bit::memory::swap( memory_block_cache& lhs, memory_block_cache& rhs )
  noexcept
{
  lhs.swap(rhs);
}
