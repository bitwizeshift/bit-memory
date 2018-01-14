#include <bit/memory/regions/aligned_heap_memory.hpp>
#include <malloc.h> // ::_aligned_malloc, ::_aligned_free

void* bit::memory::aligned_malloc( std::size_t size, std::size_t align )
  noexcept
{
  return ::_aligned_malloc(size,align);
}

//--------------------------------------------------------------------------

void bit::memory::aligned_free( void* ptr )
{
  ::_aligned_free( ptr );
}

//--------------------------------------------------------------------------

void* bit::memory::aligned_offset_malloc( std::size_t size,
                                          std::size_t align,
                                          std::size_t offset )
  noexcept
{
  return ::_aligned_offset_malloc( size, align, offset );
}

//--------------------------------------------------------------------------

void bit::memory::aligned_offset_free( void* ptr )
{
  ::_aligned_free( ptr );
}

#define BIT_ALIGNED_MALLOC_DEFINED
