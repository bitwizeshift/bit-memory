#include <bit/memory/debugging.hpp>

//============================================================================
// byte_range
//============================================================================

//----------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------

bit::memory::byte_range::byte_range( const byte* first, const byte* last )
  noexcept
  : m_first(first),
    m_last(last)
{

}

//----------------------------------------------------------------------------
// Iterators
//----------------------------------------------------------------------------

const bit::memory::byte* bit::memory::byte_range::begin()
  const noexcept
{
  return m_first;
}

const bit::memory::byte* bit::memory::byte_range::end()
  const noexcept
{
  return m_last;
}

bit::memory::byte_range bit::memory::make_byte_range(memory_block block)
  noexcept
{
  auto b = static_cast<const byte*>(block.data());

  return { b, b + block.size() };
}

bit::memory::byte_range bit::memory::make_byte_range( const void* p,
                                                      std::size_t n )
  noexcept
{
  auto b = static_cast<const byte*>(p);

  return { b, b + n };
}
