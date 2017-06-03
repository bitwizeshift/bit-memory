#ifndef BIT_MEMORY_DETAIL_LINEAR_ALLOCATOR_INL
#define BIT_MEMORY_DETAIL_LINEAR_ALLOCATOR_INL

//============================================================================
// linear_allocator
//============================================================================

//----------------------------------------------------------------------------
// Constructors
//----------------------------------------------------------------------------

inline bit::memory::linear_allocator::linear_allocator( memory_block block )
  noexcept
  : m_block(block),
    m_current(m_block.data())
{
  assert( m_block && "Block must not be null" );
}

//----------------------------------------------------------------------------
// Allocation / Deallocation
//----------------------------------------------------------------------------

inline void* bit::memory::linear_allocator
  ::allocate( std::size_t size, std::size_t align, std::size_t offset )
{
  assert( size && "cannot allocate 0 bytes");

//  auto p = offset_align_forward( m_current, size, align, offset );
//  m_current = (static_cast<unsigned char*>(p) + size);

//  assert( m_block.contains( m_current ) );
//
//  return p;
}

inline void bit::memory::linear_allocator::deallocate( void* p, std::size_t )
{
  assert( m_block.contains( p ) );
}

//----------------------------------------------------------------------------
// Construction / Destruction
//----------------------------------------------------------------------------

template<typename T, typename...Args>
inline T* bit::memory::linear_allocator::construct( Args&&...args )
{
  auto p = allocate( sizeof(T), alignof(T), 0 );

  return uninitialized_construct_at<T>( p, std::forward<Args>(args)... );
}

template<typename T>
inline T* bit::memory::linear_allocator::construct_array( std::size_t n )
{
  auto p = allocate( sizeof(T) * n, alignof(T), 0 );

  return uninitialized_construct_array_at<T>( p, n );
}

//----------------------------------------------------------------------------

template<typename T>
inline void bit::memory::linear_allocator::destruct( T* p )
{
  destroy_at( p );
  deallocate( p );
}

template<typename T>
inline void bit::memory::linear_allocator::destruct_array( T* p, std::size_t n )
{
  destroy_array_at( p, n );
  deallocate( p , n );
}

#endif /* BIT_MEMORY_DETAIL_LINEAR_ALLOCATOR_INL */
