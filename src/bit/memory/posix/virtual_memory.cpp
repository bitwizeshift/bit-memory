#include <bit/memory/virtual_memory.hpp>

#include <cassert>

#include <sys/mman.h>
#include <unistd.h>

//--------------------------------------------------------------------------
// Global Constants
//--------------------------------------------------------------------------

#if defined(PAGESIZE)
  const std::size_t bit::memory::virtual_memory_page_size = PAGESIZE;
#elif defined(PAGE_SIZE)
  const std::size_t bit::memory::virtual_memory_page_size = PAGE_SIZE;
#else
  const std::size_t bit::memory::virtual_memory_page_size = ::sysconf(_SC_PAGESIZE);
#endif

//--------------------------------------------------------------------------
// Free Functions
//--------------------------------------------------------------------------

void* bit::memory::virtual_memory_reserve( std::size_t n )
  noexcept
{
  auto size       = n * virtual_memory_page_size;
  auto protection = MAP_PRIVATE | MAP_ANONYMOUS;
  auto ptr        = ::mmap(nullptr, size, PROT_NONE, protection, -1, 0);

  return ptr == MAP_FAILED ? nullptr : ptr;
}

void* bit::memory::virtual_memory_commit( void* memory, std::size_t n )
  noexcept
{
  auto size       = n * virtual_memory_page_size;
  auto protection = PROT_WRITE | PROT_READ;
  auto result     = ::mprotect(memory, size, protection);

  if (result != 0) return nullptr;

// advise that the memory will be needed
#if defined(MADV_WILLNEED)
  madvise(memory, size, MADV_WILLNEED);
#elif defined(POSIX_MADV_WILLNEED)
  posix_madvise(memory, size, POSIX_MADV_WILLNEED);
#endif

  return memory;
}

void bit::memory::virtual_memory_decommit( void* memory, std::size_t n )
  noexcept
{
  auto size = n * virtual_memory_page_size;

#if defined(MADV_FREE)
  madvise(memory, size, MADV_FREE);
#elif defined(MADV_DONTNEED)
  madvise(memory, size, MADV_DONTNEED);
#elif defined(POSIX_MADV_DONTNEED)
  posix_madvise(memory, size, POSIX_MADV_DONTNEED);
#endif

  auto result = ::mprotect(memory, size, PROT_NONE);

  assert(result == 0 && "virtual_memory_decommit: unable to decommit memory");

  (void) result;
}

void bit::memory::virtual_memory_release( void* memory, std::size_t n )
  noexcept
{
  auto size   = n * virtual_memory_page_size;
  auto result = ::munmap(memory, size);

  assert(result == 0 && "virtual_memory_release: unable to release memory");

  (void) result;
}
