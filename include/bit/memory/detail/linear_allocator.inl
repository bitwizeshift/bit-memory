#ifndef BIT_MEMORY_DETAIL_LINEAR_ALLOCATOR_INL
#define BIT_MEMORY_DETAIL_LINEAR_ALLOCATOR_INL

//============================================================================
// linear_allocator
//============================================================================

//----------------------------------------------------------------------------
// Allocation / Deallocation
//----------------------------------------------------------------------------

inline void* bit::memory::linear_allocator::allocate( std::size_t size,
                                                      std::size_t align,
                                                      std::size_t n )
{
  assert( n && "cannot allocate 0 instances");

  auto p = align_forward( m_current, size, align );
  m_current = (static_cast<unsigned char*>(p) + size*n);

  assert( m_block.contains( m_current ) );

  return p;
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
  auto p = allocate( sizeof(T), alignof(T), 1 );

  return uninitialized_construct_at<T>( p, std::forward<Args>(args)... );
}

template<typename T>
inline T* bit::memory::linear_allocator::construct_array( std::size_t n )
{
  auto p = allocate( sizeof(T), alignof(T), n );

  return uninitialized_construct_array_at<T>( p, n );
}

//----------------------------------------------------------------------------

template<typename T>
inline void bit::memory::linear_allocator::destruct( T* p )
{
  destroy_at( p );
}

template<typename T>
inline void bit::memory::linear_allocator::destruct_array( T* p, std::size_t n )
{
  destroy_array_at( p, n );
}

#endif /* BIT_MEMORY_DETAIL_LINEAR_ALLOCATOR_INL */
