#include <bit/memory/null_block_allocator.hpp>

bit::memory::null_block_allocator& bit::memory::null_block_allocator_instance()
  noexcept
{
  static null_block_allocator s_instance;

  return s_instance;
}
