#ifndef BIT_MEMORY_DETAIL_POINTER_UTILITIES_INL
#define BIT_MEMORY_DETAIL_POINTER_UTILITIES_INL

//-----------------------------------------------------------------------------
// Alignment Checking
//-----------------------------------------------------------------------------

inline constexpr bool bit::memory::is_power_of_two( std::size_t x )
  noexcept
{
  return (x && !(x & (x-1)));
}

inline std::size_t bit::memory::align_of( const void* ptr )
  noexcept
{
  auto address = reinterpret_cast<std::uintptr_t>(ptr);

  auto align   = std::size_t{1};

  while( (address & 1) == 0 ) {
    address >>= 1;
    align <<= 1;
  }

  return align;
}

//-----------------------------------------------------------------------------
// Offset Calculation
//-----------------------------------------------------------------------------

inline std::size_t bit::memory::align_forward_offset( void* ptr,
                                                      std::size_t align )
  noexcept
{
  assert( is_power_of_two(align) && "Alignment must be a power of 2");

  const auto address    = reinterpret_cast<std::uintptr_t>(ptr);
  const auto adjustment = align - (address & (align - 1u));

  return (adjustment == align) ? 0u : adjustment;

}

inline std::size_t bit::memory::align_forward_offset( void* ptr,
                                                      std::size_t align,
                                                      std::size_t offset )
  noexcept
{
  auto p = advance(ptr, static_cast<std::ptrdiff_t>(offset));

  return align_forward_offset( p, align ) - offset;
}

//-----------------------------------------------------------------------------

inline std::size_t bit::memory::align_backward_offset( void* ptr,
                                                       std::size_t align )
  noexcept
{
  assert( is_power_of_two(align) && "Alignment must be a power of 2");

  const auto adjustment = (reinterpret_cast<std::uintptr_t>(ptr) & (align - 1));

  return (adjustment == align) ? 0u : adjustment;
}

inline std::size_t bit::memory::align_backward_offset( void* ptr,
                                                       std::size_t align,
                                                       std::size_t offset )
  noexcept
{
  auto p = advance(ptr, -static_cast<std::ptrdiff_t>(offset));

  return align_backward_offset( p, align );
}

//-----------------------------------------------------------------------------
// Align
//-----------------------------------------------------------------------------

inline void* bit::memory::align_forward( void* ptr,
                                         std::size_t alignment,
                                         std::size_t* adjust )
  noexcept
{
  assert( is_power_of_two(alignment) && "Alignment must be a power of 2");

  const auto address = reinterpret_cast<std::uintptr_t>(ptr);
  void* new_ptr      = reinterpret_cast<void*>( ((address + static_cast<std::uintptr_t>(alignment-1)) & static_cast<std::uintptr_t>(~(alignment-1))) );
  if( adjust ) {
    *adjust = static_cast<std::size_t>( reinterpret_cast<std::uintptr_t>(new_ptr) - address );
  }
  return new_ptr;
}

//-----------------------------------------------------------------------------

inline void* bit::memory::align_backward( void* ptr,
                                          std::size_t alignment,
                                          std::size_t* adjust )
  noexcept
{
  assert( is_power_of_two(alignment) && "Alignment must be a power of 2");

  const auto address = reinterpret_cast<std::uintptr_t>(ptr);
  void* new_ptr      = reinterpret_cast<void*>(((address) & (~static_cast<std::uintptr_t>(alignment-1))));
  if( adjust ) {
    *adjust = static_cast<std::size_t>(address & static_cast<std::uintptr_t>(alignment-1));
  }
  return new_ptr;
}

//-----------------------------------------------------------------------------
// Align with Offset
//-----------------------------------------------------------------------------

inline void* bit::memory::offset_align_forward( void* ptr,
                                                std::size_t alignment,
                                                std::size_t offset,
                                                std::size_t* adjust )
  noexcept
{
  assert( is_power_of_two(alignment) && "Alignment must be a power of 2");

  const auto address = reinterpret_cast<std::uintptr_t>(ptr);
  void* new_ptr      = reinterpret_cast<void*>( ((address + offset + static_cast<std::uintptr_t>(alignment-1)) & static_cast<std::uintptr_t>(~(alignment-1))) - offset );
  if( adjust ) {
    *adjust = static_cast<std::size_t>( reinterpret_cast<std::uintptr_t>(new_ptr) - address );
  }
  return new_ptr;
}

//-----------------------------------------------------------------------------

inline void* bit::memory::offset_align_backward( void* ptr,
                                                 std::size_t alignment,
                                                 std::size_t offset,
                                                 std::size_t* adjust )
  noexcept
{
  assert( is_power_of_two(alignment) && "Alignment must be a power of 2");

  const auto address = reinterpret_cast<std::uintptr_t>(ptr);
  void* new_ptr      = reinterpret_cast<void*>(((address) & (~static_cast<std::uintptr_t>(alignment-1))) - offset);
  if( adjust ) {
    *adjust = static_cast<std::size_t>(address & static_cast<std::uintptr_t>(alignment-1));
  }
  return new_ptr;
}

//-----------------------------------------------------------------------------
// Pointer Manipulation
//-----------------------------------------------------------------------------

inline std::uintptr_t bit::memory::to_address( void* ptr )
noexcept
{
  return reinterpret_cast<std::uintptr_t>(ptr);
}

inline void* bit::memory::from_address( std::uintptr_t address )
noexcept
{
  return reinterpret_cast<void*>(address);
}

//-----------------------------------------------------------------------------
// Nullability
//-----------------------------------------------------------------------------

template<typename Ptr>
inline constexpr auto bit::memory::is_null( Ptr&& ptr )
  noexcept -> decltype( ptr==nullptr, bool() )
{
  return ptr == nullptr;
}

inline constexpr bool bit::memory::is_null( std::nullptr_t )
  noexcept
{
  return true;
}

//-----------------------------------------------------------------------------
// Deltas
//-----------------------------------------------------------------------------

inline std::size_t bit::memory::distance( const void* lhs, const void* rhs )
  noexcept
{
  return (lhs > rhs) ? difference(lhs,rhs) : difference(rhs,lhs);
}

inline std::ptrdiff_t bit::memory::difference( const void* lhs, const void* rhs )
  noexcept
{
  return static_cast<const byte*>(lhs) - static_cast<const byte*>(rhs);
}

inline void* bit::memory::advance( void* p, std::ptrdiff_t bytes )
  noexcept
{
  return static_cast<byte*>(p) + bytes;
}

inline const void* bit::memory::advance( const void* p, std::ptrdiff_t bytes )
  noexcept
{
  return static_cast<const byte*>(p) + bytes;
}

#endif /* BIT_MEMORY_DETAIL_POINTER_UTILITIES_INL */
