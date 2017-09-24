#include <bit/memory/errors.hpp>

#include <atomic>
#include <iostream>
#include <cassert>

//-----------------------------------------------------------------------------
// Forward Declarations
//-----------------------------------------------------------------------------

namespace {

  //---------------------------------------------------------------------------
  // Default Handlers
  //---------------------------------------------------------------------------

  void default_leak_handler( const bit::memory::allocator_info& info,
                             const void* ptr,
                             std::ptrdiff_t size );


  void default_stomp_handler( const bit::memory::allocator_info& info,
                              const void* ptr,
                              std::ptrdiff_t size );

  void default_double_delete_handler( const bit::memory::allocator_info& info,
                                      const void* ptr,
                                      std::ptrdiff_t size );

  void default_out_of_memory_handler( const bit::memory::allocator_info& info,
                                      std::size_t size );

  //---------------------------------------------------------------------------
  // Static Entries
  //---------------------------------------------------------------------------

  using atomic_leak_handler_t          = std::atomic<bit::memory::leak_handler_t>;
  using atomic_stomp_handler_t         = std::atomic<bit::memory::stomp_handler_t>;
  using atomic_double_delete_handler_t = std::atomic<bit::memory::double_delete_handler_t>;
  using atomic_out_of_memory_handler_t = std::atomic<bit::memory::out_of_memory_handler_t>;

  atomic_leak_handler_t          g_leak_handler(&default_leak_handler);
  atomic_stomp_handler_t         g_stomp_handler(&default_stomp_handler);
  atomic_double_delete_handler_t g_double_delete_handler(&default_double_delete_handler);
  atomic_out_of_memory_handler_t g_out_of_memory_handler(&default_out_of_memory_handler);
}

//-----------------------------------------------------------------------------
// Leak Handler
//-----------------------------------------------------------------------------

bit::memory::leak_handler_t bit::memory::set_leak_handler( leak_handler_t f )
  noexcept
{
  return g_leak_handler.exchange( f ? f : &default_leak_handler );
}

//-----------------------------------------------------------------------------

bit::memory::leak_handler_t bit::memory::get_leak_handler()
  noexcept
{
  return g_leak_handler;
}

//-----------------------------------------------------------------------------
// Stomp Handler
//-----------------------------------------------------------------------------

bit::memory::stomp_handler_t bit::memory::set_stomp_handler( stomp_handler_t f )
  noexcept
{
  return g_stomp_handler.exchange( f ? f : &default_stomp_handler );
}

//-----------------------------------------------------------------------------

bit::memory::stomp_handler_t bit::memory::get_stomp_handler()
  noexcept
{
  return g_stomp_handler;
}

//-----------------------------------------------------------------------------
// Double Delete handlers
//-----------------------------------------------------------------------------

bit::memory::double_delete_handler_t
  bit::memory::set_double_delete_handler(double_delete_handler_t f)
  noexcept
{
  return g_double_delete_handler.exchange( f ? f : &default_double_delete_handler );
}

bit::memory::double_delete_handler_t bit::memory::get_double_delete_handler()
  noexcept
{
  return g_double_delete_handler;
}

//-----------------------------------------------------------------------------
// Out-of-memory handlers
//-----------------------------------------------------------------------------

bit::memory::out_of_memory_handler_t
  bit::memory::set_out_of_memory_handler(out_of_memory_handler_t f)
  noexcept
{
  return g_out_of_memory_handler.exchange( f ? f : &default_out_of_memory_handler );
}

bit::memory::out_of_memory_handler_t bit::memory::get_out_of_memory_handler()
  noexcept
{
  return g_out_of_memory_handler;
}

//-----------------------------------------------------------------------------
// Anonymous Definitions
//-----------------------------------------------------------------------------

namespace {

  void default_leak_handler( const bit::memory::allocator_info& info,
                             const void* ptr,
                             std::ptrdiff_t size )
  {
    std::cerr << "leak discovered with allocator '"
              << info.name() << "': "
              << size << " bytes leaked at address " << ptr << "\n";

    assert(false);
  }


  void default_stomp_handler( const bit::memory::allocator_info& info,
                              const void* ptr,
                              std::ptrdiff_t size )
  {
    std::cerr << "stomp detected at address " << ptr << "\n"
              << size << " bytes overwritten.\n";

    assert(false);
  }

  void default_double_delete_handler( const bit::memory::allocator_info& info,
                                      const void* ptr,
                                      std::ptrdiff_t size )
  {
    std::cerr << "double delete detected at address " << ptr << "\n"
              << size << " bytes double-deleted.\n";

    assert(false);
  }

  void default_out_of_memory_handler( const bit::memory::allocator_info& info,
                                      std::size_t size )
  {
    std::cerr << "out of memory with allocator '"
              << info.name()
              << "' at address " << info.address() << ".\n"
              << "requested allocation size: " << size << " bytes\n";

    assert(false);
  }

} // anonymous namespace
