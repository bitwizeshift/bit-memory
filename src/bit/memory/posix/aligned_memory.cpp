#include <bit/memory/aligned_memory.hpp>

#include <unistd.h>

#include <cstdlib>
#include <cstdint>


void* bit::memory::aligned_malloc( std::size_t size, std::size_t align )
  noexcept
{
  void* result_ptr;
  ::posix_memalign(&result_ptr, align, size);
  return result_ptr;
}

//------------------------------------------------------------------------

void bit::memory::aligned_free( void* ptr )
{
  std::free(ptr);
}

//------------------------------------------------------------------------

void* bit::memory::aligned_offset_malloc( std::size_t size,
                                          std::size_t align,
                                          std::size_t offset )
  noexcept
{
  union {
    void*          aligned_ptr;
    std::uint16_t* adjust_ptr;
  } r = {};

  assert( align % 2 == 0 && "Alignment must be power of 2" );

  void* allocated_ptr;

  // Add the amount adjusted for the offset here.
  // 16 bits chosen in case an offset greater than 255 is necessary (unlikely)
  offset += sizeof(std::uint16_t);

  // Step offset up to a multiple of the alignment
  size += offset + (offset & align);

  ::posix_memalign(&allocated_ptr, align, size);

  // Find the amount adjusted and store th result
  std::size_t adjust;
  //r.aligned_ptr = Pointer_Math::align_forward( allocated_ptr, align, offset, &adjust );

  *(r.adjust_ptr++) = static_cast<std::uint16_t>(adjust);

  return r.aligned_ptr;
}

//------------------------------------------------------------------------

void bit::memory::aligned_offset_free( void* ptr )
{
  union{
    void*          aligned_ptr;
    std::uint16_t* adjust_ptr;
  } r = {};
  r.aligned_ptr = ptr;
  std::uint16_t adjust = *(--r.adjust_ptr);

  auto p = static_cast<char*>(r.adjust_ptr) - adjust;
  //  r.aligned_ptr = Pointer_Math::subtract(r.aligned_ptr, adjust);

  std::free( ptr );
}

#define BIT_ALIGNED_MALLOC_DEFINED
