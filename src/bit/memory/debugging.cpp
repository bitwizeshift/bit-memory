#include <bit/memory/debugging.hpp>

#include <algorithm> // std::fill

//----------------------------------------------------------------------------

void bit::memory::debug_tag_bytes( void* p,
                                   std::size_t n,
                                   debug_tag tag )
{
  using byte_t = unsigned char;

  // Tag all the bytes
  auto* start = static_cast<byte_t*>(p);
  auto* end   = start + n;

  std::fill( start, end, static_cast<byte_t>(tag) );
}

//----------------------------------------------------------------------------

void* bit::memory::debug_untag_bytes( void* p,
                                      std::size_t n,
                                      debug_tag tag,
                                      std::size_t* stomped )
{
  using byte_t = unsigned char;

  auto* byte_ptr   = static_cast<byte_t*>(p);
  void* stomp_ptr  = nullptr;
  auto  stomp_size = std::size_t(0);

  while( --n ) {
    if( *byte_ptr != static_cast<byte_t>(tag) ) {
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
