#include <bit/memory/regions/virtual_memory.hpp>

#include "windows.hpp"

#include <cassert>

//-----------------------------------------------------------------------------
// Forward Declarations
//-----------------------------------------------------------------------------

namespace
{
  /// \brief Determines the virtual page size on win32
  ///
  /// \return the virtual page size
  std::size_t get_virtual_page_size() noexcept;
}

//-----------------------------------------------------------------------------
// Free Functions
//-----------------------------------------------------------------------------

std::size_t bit::memory::virtual_memory_page_size()
  noexcept
{
  static const std::size_t s_page_size = get_virtual_page_size();

  return s_page_size;
}

//-----------------------------------------------------------------------------

void* bit::memory::virtual_memory_reserve( std::size_t n )
  noexcept
{
  auto size = n * virtual_memory_page_size();
  auto ptr = ::VirtualAlloc(nullptr, size, MEM_RESERVE, PAGE_NOACCESS);

  return ptr;
}

//-----------------------------------------------------------------------------

void* bit::memory::virtual_memory_commit( void* memory, std::size_t n )
  noexcept
{
  auto size = n * virtual_memory_page_size();
  auto region = ::VirtualAlloc(memory, size, MEM_COMMIT, PAGE_READWRITE);

  if(!region) return nullptr;

//  assert(region == memory && "virtual_memory_commit: unable to commit memory");

  return region;
}

//-----------------------------------------------------------------------------

void bit::memory::virtual_memory_decommit( void* memory, std::size_t n )
  noexcept
{
  auto size = n * virtual_memory_page_size();
  ::VirtualFree(memory, size, MEM_DECOMMIT);

//  assert(result == nullptr && "virtual_memory_decommit: unable to decommit memory");
}

//-----------------------------------------------------------------------------

void bit::memory::virtual_memory_release( void* memory, std::size_t n )
  noexcept
{
  (void) n;

  ::VirtualFree(memory, 0u, MEM_RELEASE);

//  assert(result != nullptr && "virtual_memory_release: unable to release memory");
}


//-----------------------------------------------------------------------------
// Free Functions
//-----------------------------------------------------------------------------

namespace {

  std::size_t get_virtual_page_size()
    noexcept
  {
    ::SYSTEM_INFO system_info;
    ::GetSystemInfo(&system_info);

    return std::size_t{ system_info.dwPageSize };
  }

} // anonymous namespace
