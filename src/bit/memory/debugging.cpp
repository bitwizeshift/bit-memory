#include <bit/memory/debugging.hpp>

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

  std::atomic<bit::memory::leak_handler_t>  g_leak_handler(&default_leak_handler);
  std::atomic<bit::memory::stomp_handler_t> g_stomp_handler(&default_stomp_handler);
  std::atomic<bit::memory::double_delete_handler_t> g_double_delete_handler(&default_double_delete_handler);
  std::atomic<bit::memory::out_of_memory_handler_t> g_out_of_memory_handler(&default_out_of_memory_handler);
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

void bit::memory::leak_handler( const bit::memory::allocator_info& info,
                                const void* ptr,
                                std::ptrdiff_t size )
{
  (*g_leak_handler)( info, ptr, size );
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

void bit::memory::stomp_handler( const bit::memory::allocator_info& info,
                                 const void* ptr,
                                 std::ptrdiff_t size )
{
  (*g_stomp_handler)( info, ptr, size );
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

void bit::memory::double_delete_handler( const allocator_info& info,
                                         const void* ptr,
                                         std::ptrdiff_t size )
{
  (*g_double_delete_handler)( info, ptr, size );
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

void bit::memory::out_of_memory_handler( const allocator_info& info,
                                         std::size_t size )
{
  (*g_out_of_memory_handler)( info, size );
}

//-----------------------------------------------------------------------------
// Anonymous Definitions
//-----------------------------------------------------------------------------

namespace {

  void default_leak_handler( const bit::memory::allocator_info& info,
                             const void* ptr,
                             std::ptrdiff_t size )
  {
    std::cerr << "leak detected at address " << ptr << "\n"
              << size << " bytes leaked.\n";

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
              << "' at address " << info.allocator() << ".\n"
              << "requested allocation size: " << size << " bytes\n";

    assert(false);
  }

} // anonymous namespace


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
