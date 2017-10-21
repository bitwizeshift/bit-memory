#include <bit/memory/errors.hpp>

#include <atomic>
#include <iostream>
#include <cassert>

#ifdef BIT_MEMORY_BREAKPOINT
#error BIT_MEMORY_BREAKPOINT cannot be redefined
#endif

#if defined(NDEBUG)
# define BIT_MEMORY_BREAKPOINT() ((void)0)
#else
#  if defined(_MSC_VER) && (_MSC_VER >= 1300)
#    define BIT_MEMORY_BREAKPOINT() __debugbreak()   // MSVC-specific breakpoint
#  elif defined(__clang__) || defined(__GNUC__)
#    define BIT_MEMORY_BREAKPOINT() __builtin_trap() // __builtin_trap for compilers that support it
#  else // Backup option for breakpoint creation
#    include <csignal>
#    if defined(SIGTRAP)
#      define BIT_MEMORY_BREAKPOINT() std::raise(SIGTRAP)
#    else
#      define BIT_MEMORY_BREAKPOINT() std::raise(SIGABRT)
#    endif
#  endif
#endif

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


  void default_buffer_overflow_handler( const bit::memory::allocator_info& info,
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

  using atomic_leak_handler_t            = std::atomic<bit::memory::leak_handler_t>;
  using atomic_buffer_overflow_handler_t = std::atomic<bit::memory::buffer_overflow_handler_t>;
  using atomic_double_delete_handler_t   = std::atomic<bit::memory::double_delete_handler_t>;
  using atomic_out_of_memory_handler_t   = std::atomic<bit::memory::out_of_memory_handler_t>;

  atomic_leak_handler_t            g_leak_handler(&default_leak_handler);
  atomic_buffer_overflow_handler_t g_buffer_overflow_handler(&default_buffer_overflow_handler);
  atomic_double_delete_handler_t   g_double_delete_handler(&default_double_delete_handler);
  atomic_out_of_memory_handler_t   g_out_of_memory_handler(&default_out_of_memory_handler);
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

bit::memory::buffer_overflow_handler_t
  bit::memory::set_buffer_overflow_handler( buffer_overflow_handler_t f )
  noexcept
{
  return g_buffer_overflow_handler.exchange( f ? f : &default_buffer_overflow_handler );
}

//-----------------------------------------------------------------------------

bit::memory::buffer_overflow_handler_t
  bit::memory::get_buffer_overflow_handler()
  noexcept
{
  return g_buffer_overflow_handler;
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
    std::cerr << "memory leak occurred with allocator {\""
              << info.name()
              << "\", " << info.address() << "}.\n"
              << size << " bytes leaked at address " << ptr << "\n";

    BIT_MEMORY_BREAKPOINT();
  }


  void default_buffer_overflow_handler( const bit::memory::allocator_info& info,
                                        const void* ptr,
                                        std::ptrdiff_t size )
  {
    std::cerr << "buffer overflow occurred with allocator {\""
              << info.name()
              << "\", " << info.address() << "}.\n"
              << size << " bytes overwritten at address "
              << ptr << ".\n";

    BIT_MEMORY_BREAKPOINT();
  }

  void default_double_delete_handler( const bit::memory::allocator_info& info,
                                      const void* ptr,
                                      std::ptrdiff_t size )
  {
    std::cerr << "double delete occurred with allocator {\""
              << info.name()
              << "\", " << info.address() << "}.\n"
              << size << " bytes double-deleted at address "
              << ptr << ".\n";

    BIT_MEMORY_BREAKPOINT();
  }

  void default_out_of_memory_handler( const bit::memory::allocator_info& info,
                                      std::size_t size )
  {
    std::cerr << "out of memory occurred with allocator {\""
              << info.name()
              << "\", " << info.address() << "}.\n"
              << "Requested allocation size: " << size << " bytes\n";

    BIT_MEMORY_BREAKPOINT();
  }

} // anonymous namespace
