#include <bit/memory/aligned_heap_memory.hpp>

//----------------------------------------------------------------------------
// Aligned new
//----------------------------------------------------------------------------

void* operator new( std::size_t size, bit::memory::align_val_t alignment )
{
  void* result = bit::memory::aligned_malloc(size,static_cast<std::size_t>(alignment));
  if( !result ) throw std::bad_alloc();
  return result;
}

//----------------------------------------------------------------------------

void* operator new( std::size_t size,
                    bit::memory::align_val_t alignment,
                    const std::nothrow_t&)
  noexcept
{
  return bit::memory::aligned_malloc(size,static_cast<std::size_t>(alignment));
}

//----------------------------------------------------------------------------
// Aligned delete
//----------------------------------------------------------------------------

void operator delete( void* ptr, bit::memory::align_val_t alignment)
  noexcept
{
  bit::memory::aligned_free(ptr);
}

//----------------------------------------------------------------------------

void operator delete( void* ptr,
                      bit::memory::align_val_t alignment,
                      std::nothrow_t& )
  noexcept
{
  bit::memory::aligned_free(ptr);
}

//----------------------------------------------------------------------------

void operator delete( void* ptr,
                      bit::memory::align_val_t alignment,
                      std::size_t size )
  noexcept
{
  bit::memory::aligned_free(ptr);
}

//----------------------------------------------------------------------------
// Aligned new[]
//----------------------------------------------------------------------------

void* operator new[]( std::size_t size, bit::memory::align_val_t alignment )
{
  void* result = bit::memory::aligned_malloc(size,static_cast<std::size_t>(alignment));
  if( !result ) throw std::bad_alloc();
  return result;
}

//----------------------------------------------------------------------------

void* operator new[]( std::size_t size,
                      bit::memory::align_val_t alignment,
                      const std::nothrow_t& )
  noexcept
{
  return bit::memory::aligned_malloc(size,static_cast<std::size_t>(alignment));
}

//----------------------------------------------------------------------------
// Aligned delete[]
//----------------------------------------------------------------------------

void operator delete[]( void* ptr, bit::memory::align_val_t alignment )
  noexcept
{
  bit::memory::aligned_free(ptr);
}

//----------------------------------------------------------------------------

void operator delete[]( void* ptr,
                        bit::memory::align_val_t alignment,
                        const std::nothrow_t& )
  noexcept
{
  bit::memory::aligned_free(ptr);
}

//----------------------------------------------------------------------------

void operator delete[]( void* ptr,
                        bit::memory::align_val_t alignment,
                        std::size_t size )
  noexcept
{
  bit::memory::aligned_free(ptr);
}
