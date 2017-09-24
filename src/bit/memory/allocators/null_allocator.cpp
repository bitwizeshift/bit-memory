#include <bit/memory/allocators/null_allocator.hpp>

bit::memory::null_allocator& bit::memory::null_allocator_instance()
  noexcept
{
  static null_allocator s_instance;

  return s_instance;
}
