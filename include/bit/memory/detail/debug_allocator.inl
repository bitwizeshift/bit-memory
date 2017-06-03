#ifndef BIT_MEMORY_DETAIL_DEBUG_ALLOCATOR_INL
#define BIT_MEMORY_DETAIL_DEBUG_ALLOCATOR_INL

//============================================================================
// debug_allocator
//============================================================================

//----------------------------------------------------------------------------
// Constructors / Destructor / Assignment
//----------------------------------------------------------------------------

template<typename Allocator>
template<typename...Args, decltype(Allocator( std::declval<Args>()... ), void())*>
bit::memory::debug_allocator<Allocator>
  ::debug_allocator( std::size_t start, std::size_t end, Args&&...args )
  : m_storage(start,end,std::forward<Args>(args)...)
{

}

template<typename Allocator>
bit::memory::debug_allocator<Allocator>::~debug_allocator()
{
  if( m_storage.size ) {
    leak_handler( {"debug",this},nullptr,m_storage.size );
  }
}

template<typename Allocator>
void* bit::memory::debug_allocator<Allocator>
  ::allocate( std::size_t bytes, std::size_t align, std::size_t offset,
              std::ptrdiff_t* padding )
{
  using byte_t = unsigned char;

  const auto new_size    = bytes + m_storage.fence_start_size + m_storage.fence_end_size;
  const auto new_offset  = offset + m_storage.fence_start_size;
  const auto new_padding = std::ptrdiff_t();

  auto ptr = (byte_t*) m_storage.allocate(new_size, align, new_offset, &new_padding);

  // This could be factored into better terms, but I have faith that the
  // optimizer can clean this up. I leave it this way for readability and
  // maintainability.

  const auto p_start  = ptr - new_padding;
  const auto p_end    = ptr;
  const auto f1_start = ptr + offset;
  const auto f1_end   = ptr + offset + m_storage.fence_start_size;
  const auto a_start  = ptr + offset + m_storage.fence_start_size;
  const auto a_end    = ptr + offset + m_storage.fence_start_size + bytes;
  const auto f2_start = ptr + offset + m_storage.fence_start_size + bytes;
  const auto f2_end   = ptr + offset + m_storage.fence_start_size + bytes + m_storage.fence_end_size;

  // Fill debugging bytes
  std::fill( p_start,  p_end,  static_cast<byte_t>(debug_tag::padding_byte) );
  std::fill( f1_start, f1_end, static_cast<byte_t>(debug_tag::fence_start_byte) );
  std::fill( a_start,  a_end,  static_cast<byte_t>(debug_tag::allocated_byte) );
  std::fill( f2_start, f2_end, static_cast<byte_t>(debug_tag::fence_end_byte) );

  // Store padding, if requested
  if( padding ) *padding = new_padding;

  ++m_storage.allocations;
  m_storage.size += new_size;

  return a_start;
}

template<typename Allocator>
void* bit::memory::debug_allocator<Allocator>
  ::deallocate( void* ptr, std::size_t bytes )
{
  using byte_t = unsigned char;

  auto start_ptr      = ((byte_t*) ptr) - m_storage.fence_start_size;
  const auto new_size = bytes + m_storage.fence_start_size + m_storage.fence_end_size;
  const auto new_ptr  = (byte_t*) deallocate( start_ptr, bytes );

  // This could be factored into better terms, but I have faith that the
  // optimizer can clean this up. I leave it this way for readability and
  // maintainability.

  const auto f1_start = new_ptr - m_storage.fence_start_size;
  const auto f1_end   = new_ptr;
  const auto a_start  = new_ptr;
  const auto a_end    = new_ptr + bytes;
  const auto f2_start = new_ptr + bytes;
  const auto f2_end   = new_ptr + bytes + m_storage.fence_end_size;

  auto stomp = 0;

  // check beginning fence
  std::for_each( f1_start, f1_end, [&](const byte_t& byte)
  {
    if(byte != static_cast<byte_t>(debug_tag::fence_start_byte)){
      ++stomp;
    }
  });

  // check ending fence
  std::for_each( f2_start, f2_end + m_storage.fence_end_size, [&](const byte_t& byte)
  {
    if(byte != static_cast<byte_t>(debug_tag::fence_end_byte)){
      ++stomp;
    }
  });

  if( stomp ) {
    // call stomp_handler
    stomp_handler({"debug",&m_storage}, ptr, stomp );
  }

  std::fill(a_start, a_end, static_cast<byte_t>(debug_tag::freed_byte) );

  --m_storage.allocations;
  m_storage.size -= new_size;

}

#endif /* BIT_MEMORY_DETAIL_DEBUG_ALLOCATOR_INL */
