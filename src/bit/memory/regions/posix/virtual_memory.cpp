#include <bit/memory/regions/virtual_memory.hpp>

#include <cassert>

#include <sys/mman.h> // ::mmap
#include <unistd.h>   // ::sysconf

//--------------------------------------------------------------------------
// Global Constants
//--------------------------------------------------------------------------

#ifndef _SC_PAGESIZE
# error _SC_PAGESIZE must be defined to determine virtual page size
#endif

//--------------------------------------------------------------------------
// Forward Declarations
//--------------------------------------------------------------------------

namespace
{
  /// \brief Determines the virtual page size on posix
  ///
  /// \return the virtual page size
  std::size_t get_virtual_page_size() noexcept;
}

//--------------------------------------------------------------------------
// Free Functions
//--------------------------------------------------------------------------

std::size_t bit::memory::virtual_memory_page_size()
  noexcept
{
  static const std::size_t s_page_size = get_virtual_page_size();

  return s_page_size;
}

//--------------------------------------------------------------------------

void* bit::memory::virtual_memory_reserve( std::size_t n )
  noexcept
{
  auto size       = n * virtual_memory_page_size();
  auto protection = MAP_PRIVATE | MAP_ANONYMOUS;
  auto ptr        = ::mmap(nullptr, size, PROT_NONE, protection, -1, 0);

  return ptr == MAP_FAILED ? nullptr : ptr;
}

//--------------------------------------------------------------------------

void* bit::memory::virtual_memory_commit( void* memory, std::size_t n )
  noexcept
{
  auto size       = n * virtual_memory_page_size();
  auto protection = PROT_WRITE | PROT_READ;
  auto result     = ::mprotect(memory, size, protection);

  if (result != 0) return nullptr;

// advise that the memory will be needed
#if defined(MADV_WILLNEED)
  ::madvise(memory, size, MADV_WILLNEED);
#elif defined(POSIX_MADV_WILLNEED)
  ::posix_madvise(memory, size, POSIX_MADV_WILLNEED);
#endif

  return memory;
}

//--------------------------------------------------------------------------

void bit::memory::virtual_memory_decommit( void* memory, std::size_t n )
  noexcept
{
  auto size = n * virtual_memory_page_size();

#if defined(MADV_FREE)
  ::madvise(memory, size, MADV_FREE);
#elif defined(MADV_DONTNEED)
  ::madvise(memory, size, MADV_DONTNEED);
#elif defined(POSIX_MADV_DONTNEED)
  ::posix_madvise(memory, size, POSIX_MADV_DONTNEED);
#endif

  auto result = ::mprotect(memory, size, PROT_NONE);

  assert(result == 0 && "virtual_memory_decommit: unable to decommit memory");

  (void) result;
}

//--------------------------------------------------------------------------

void bit::memory::virtual_memory_release( void* memory, std::size_t n )
  noexcept
{
  auto size   = n * virtual_memory_page_size();
  auto result = ::munmap(memory, size);

  assert(result == 0 && "virtual_memory_release: unable to release memory");

  (void) result;
}

//--------------------------------------------------------------------------

namespace {

  std::size_t get_virtual_page_size()
    noexcept
  {
    return static_cast<std::size_t>(::sysconf(_SC_PAGESIZE));
  }

} // anonymous namespace
