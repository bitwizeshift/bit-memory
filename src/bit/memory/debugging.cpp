#include <bit/memory/debugging.hpp>

#include <atomic>
#include <iostream>

//----------------------------------------------------------------------------
// Forward Declarations
//----------------------------------------------------------------------------

namespace {

  void default_leak_handler( const bit::memory::allocator_info& info,
                             const void* ptr,
                             std::ptrdiff_t size );


  void default_stomp_handler( const bit::memory::allocator_info& info,
                              const void* ptr,
                              std::ptrdiff_t size );

  //--------------------------------------------------------------------------
  // Static Entries
  //--------------------------------------------------------------------------

  std::atomic<bit::memory::leak_handler_t>  g_leak_handler(default_leak_handler);
  std::atomic<bit::memory::stomp_handler_t> g_stomp_handler(default_stomp_handler);
}

//----------------------------------------------------------------------------
// Leak Handler
//----------------------------------------------------------------------------

bit::memory::leak_handler_t bit::memory::set_leak_handler( leak_handler_t f )
  noexcept
{
  return g_leak_handler.exchange( f ? f : default_leak_handler );
}

//----------------------------------------------------------------------------

bit::memory::leak_handler_t bit::memory::get_leak_handler()
  noexcept
{
  return g_leak_handler;
}

//----------------------------------------------------------------------------

void bit::memory::leak_handler( const bit::memory::allocator_info& info,
                                const void* ptr,
                                std::ptrdiff_t size )
{
  (*g_leak_handler)( info, ptr, size );
}

//----------------------------------------------------------------------------
// Stomp Handler
//----------------------------------------------------------------------------

bit::memory::stomp_handler_t bit::memory::set_stomp_handler( stomp_handler_t f )
  noexcept
{
  return g_stomp_handler.exchange( f ? f : default_stomp_handler );
}

//----------------------------------------------------------------------------

bit::memory::stomp_handler_t bit::memory::get_stomp_handler()
  noexcept
{
  return g_stomp_handler;
}

//----------------------------------------------------------------------------

void bit::memory::stomp_handler( const bit::memory::allocator_info& info,
                                 const void* ptr,
                                 std::ptrdiff_t size )
{
  (*g_stomp_handler)( info, ptr, size );
}

//----------------------------------------------------------------------------
// Anonymous Definitions
//----------------------------------------------------------------------------

namespace {

  void default_leak_handler( const bit::memory::allocator_info& info,
                             const void* ptr,
                             std::ptrdiff_t size )
  {
    std::cerr << "leak detected at address " << ptr << "\n"
              << size << " bytes leaked.\n";
  }


  void default_stomp_handler( const bit::memory::allocator_info& info,
                              const void* ptr,
                              std::ptrdiff_t size )
  {
    std::cerr << "stomp detected at address " << ptr << "\n"
              << size << " bytes overwritten.\n";
  }

} // anonymous namespace
