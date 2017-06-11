#include <bit/memory/alignment.hpp>

//----------------------------------------------------------------------------
// Offset Calculation
//----------------------------------------------------------------------------

std::size_t bit::memory::align_forward_offset( void* ptr,
                                               std::size_t align )
  noexcept
{
  const std::uintptr_t address    = reinterpret_cast<std::uintptr_t>(ptr);
  const std::size_t    adjustment = align - (address & (align - 1));

  return ((adjustment == align) ? 0 : adjustment);
}

std::size_t bit::memory::align_forward_offset( void* ptr,
                                               std::size_t align,
                                               std::size_t offset )
  noexcept
{
  ptr = static_cast<char*>(ptr) + offset;

  return align_forward_offset(ptr,align) - offset;
}

//----------------------------------------------------------------------------

std::size_t bit::memory::align_backward_offset( void* ptr,
                                                std::size_t align )
  noexcept
{
  const std::size_t adjustment = (reinterpret_cast<std::uintptr_t>(ptr) & (align - 1));

  return (adjustment == align) ? 0 : adjustment;
}

std::size_t bit::memory::align_backward_offset( void* ptr,
                                                std::size_t align,
                                                std::size_t offset )
  noexcept
{
  ptr = static_cast<char*>(ptr) - offset;
  return align_backward_offset(ptr,align);
}
