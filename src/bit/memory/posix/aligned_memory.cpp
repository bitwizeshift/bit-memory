#include <bit/memory/aligned_memory.hpp>

#include <bit/memory/unaligned_memory.hpp>  // store_unaligned, load_unaligned
#include <bit/memory/pointer_utilities.hpp> // offset_align_forward

#include <cstddef> // std::size_t, std::ptrdiff_t
#include <cstdint> // std::uint16_t
#include <cstdlib> // std::free

#include <unistd.h> // posix_memalign

void* bit::memory::aligned_malloc( std::size_t size, std::size_t align )
  noexcept
{
  void* result_ptr = nullptr;
  if( ::posix_memalign(&result_ptr, align, size) )
  {
    return nullptr;
  }
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
  assert( is_power_of_two(align) && "Alignment must be power of 2" );

  // Add the amount adjusted for the offset, and
  // Step offset up to a multiple of the alignment
  offset += sizeof(std::uint16_t);
  size   += (offset + (offset & align));

  // Don't bother using ::posix_memalign, since we need to manually control
  // the offset anyway.
  auto* allocated_ptr = std::malloc(size);

  // Adjust the pointer forward to the alignment, and store the adjustment
  auto p      = static_cast<char*>(allocated_ptr);
  auto adjust = std::size_t{};
  p           = static_cast<char*>(offset_align_forward(p, align, offset, &adjust));

  store_unaligned( p, static_cast<std::uint16_t>(adjust) );

  return p + sizeof(std::uint16_t);
}

//------------------------------------------------------------------------

void bit::memory::aligned_offset_free( void* ptr )
{
  // Load the previously stored adjustment, then adjust pointer back to
  // allocated state
  auto p = static_cast<char*>(ptr);
  p -= sizeof(std::uint16_t);

  auto adjust = load_unaligned_uint16(p);
  p -= adjust;

  std::free( p );
}

#define BIT_ALIGNED_MALLOC_DEFINED
