#include <bit/memory/debugging.hpp>

#include <algorithm> // std::fill

//----------------------------------------------------------------------------

void bit::memory::debug_tag_bytes( void* p,
                                   std::size_t n,
                                   debug_tag tag )
{
  // Tag all the bytes
  auto* start = static_cast<byte*>(p);
  auto* end   = start + n;

  std::fill( start, end, static_cast<byte>(tag) );
}

//----------------------------------------------------------------------------

void* bit::memory::debug_untag_bytes( void* p,
                                      std::size_t n,
                                      debug_tag tag,
                                      std::size_t* stomped )
{
  auto* byte_ptr   = static_cast<byte*>(p);
  auto* stomp_ptr  = (void*){};
  auto  stomp_size = std::size_t(0);

  while( --n ) {
    if( *byte_ptr != static_cast<byte>(tag) ) {
      // If a stomp occurs, record the start of th estomp, and determine
      // the size
      if( !stomp_ptr ) stomp_ptr = byte_ptr;
      ++stomp_size;
    }
    ++byte_ptr;
  }
  if( stomp_ptr ) {
    *stomped = stomp_size;
  }

  return stomp_ptr;

}

//============================================================================
// byte_range
//============================================================================

//----------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------

bit::memory::byte_range::byte_range( const byte* first, const byte* last )
  noexcept
  : m_first(first),
    m_last(last)
{

}

//----------------------------------------------------------------------------
// Iterators
//----------------------------------------------------------------------------

const bit::memory::byte* bit::memory::byte_range::begin()
  const noexcept
{
  return m_first;
}

const bit::memory::byte* bit::memory::byte_range::end()
  const noexcept
{
  return m_last;
}

bit::memory::byte_range bit::memory::make_byte_range(memory_block block)
  noexcept
{
  auto b = static_cast<const byte*>(block.data());

  return { b, b + block.size() };
}

bit::memory::byte_range bit::memory::make_byte_range( const void* p,
                                                      std::size_t n )
  noexcept
{
  auto b = static_cast<const byte*>(p);

  return { b, b + n };
}
