#ifndef BIT_MEMORY_ALLOCATORS_DETAIL_STACK_ALLOCATOR_INL
#define BIT_MEMORY_ALLOCATORS_DETAIL_STACK_ALLOCATOR_INL

//-----------------------------------------------------------------------------
// Constructors / Destructor
//-----------------------------------------------------------------------------

template<std::size_t Size, std::size_t Align>
inline bit::memory::stack_allocator<Size,Align>::stack_allocator()
  noexcept
  : m_current(&m_storage[0])
{

}

//-----------------------------------------------------------------------------

template<std::size_t Size, std::size_t Align>
inline bit::memory::stack_allocator<Size,Align>::~stack_allocator()
{
  // This destructor should be trivial -- but instead calls
  // 'deallocate_all' so that the memory trackers can accurately reflect
  // that the memory has accurately been deallocated on destruction
  deallocate_all();
}

//-----------------------------------------------------------------------------
// Allocation / Deallocation
//-----------------------------------------------------------------------------

template<std::size_t Size, std::size_t Align>
inline void* bit::memory::stack_allocator<Size,Align>
  ::try_allocate( std::size_t size, std::size_t align, std::size_t offset )
  noexcept
{
  assert( size && "cannot allocate 0 bytes");
  assert( align && "cannot allocate with 0 alignment");
  assert( is_power_of_two(align) && "alignment must be a power of two" );

  auto adjust = std::size_t{};
  auto* p = offset_align_forward(m_current,align,offset+1,&adjust);
  auto* byte_ptr = static_cast<byte*>(p);

  // store the adjustment required
  *byte_ptr = static_cast<byte>(adjust);
  byte_ptr += 1;

  // determine the end pointer
  auto* p_end = byte_ptr + size;

  // If allocated outside the range, return nullptr
  if( BIT_MEMORY_UNLIKELY( p_end > static_cast<void*>(&m_storage[Size]) ) )
    return nullptr;

  // bump the pointer
  m_current = p_end;

  return byte_ptr;
}

template<std::size_t Size, std::size_t Align>
inline void bit::memory::stack_allocator<Size,Align>
  ::deallocate( void* p, std::size_t size )
{
  assert( owns(p) && "Pointer must be owned by this allocator" );
  assert( m_current > p && "Allocations occurred out-of-order" );

  BIT_MEMORY_UNUSED(size);

  // Adjust the pointer
  auto* byte_ptr = static_cast<byte*>(p);
  byte_ptr -= 1;

  auto adjust = static_cast<std::size_t>(*byte_ptr);
  byte_ptr -= adjust;

  m_current = byte_ptr;
}

template<std::size_t Size, std::size_t Align>
inline void bit::memory::stack_allocator<Size,Align>::deallocate_all()
{
  m_current = static_cast<void*>(&m_storage[0]);
}

//-----------------------------------------------------------------------------
// Observers
//-----------------------------------------------------------------------------

template<std::size_t Size, std::size_t Align>
inline bool bit::memory::stack_allocator<Size,Align>::owns( void* p )
  const noexcept
{
  return static_cast<const void*>(&m_storage[0]) <= p && p < static_cast<const void*>(&m_storage[Size]);
}

//-----------------------------------------------------------------------------
// Comparisons
//-----------------------------------------------------------------------------

template<std::size_t S, std::size_t A>
inline bool bit::memory::operator==( const stack_allocator<S,A>& lhs,
                                     const stack_allocator<S,A>& rhs )
  noexcept
{
  return lhs.m_current == rhs.m_current &&
         (&lhs.m_storage[0]) == (&rhs.m_storage[0]);
}

template<std::size_t S, std::size_t A>
inline bool bit::memory::operator!=( const stack_allocator<S,A>& lhs,
                                     const stack_allocator<S,A>& rhs )
  noexcept
{
  return !(lhs==rhs);
}

#endif /* BIT_MEMORY_ALLOCATORS_DETAIL_STACK_ALLOCATOR_INL */
