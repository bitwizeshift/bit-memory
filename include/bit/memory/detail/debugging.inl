#ifndef BIT_MEMORY_DETAIL_DEBUGGING_INL
#define BIT_MEMORY_DETAIL_DEBUGGING_INL


//============================================================================
// allocator_info
//============================================================================

//----------------------------------------------------------------------------
// Constructors
//----------------------------------------------------------------------------

inline bit::memory::allocator_info
  ::allocator_info( const char* name, const void* allocator )
  : m_name(name),
    m_allocator(allocator)
{

}

//----------------------------------------------------------------------------
// Observers
//----------------------------------------------------------------------------

inline const char* bit::memory::allocator_info::name()
  const noexcept
{
  return m_name;
}

inline const void* bit::memory::allocator_info::allocator()
  const noexcept
{
  return m_allocator;
}

//----------------------------------------------------------------------------
// Comparisons
//----------------------------------------------------------------------------

inline bool bit::memory::operator==( const allocator_info& lhs,
                                     const allocator_info& rhs )
  noexcept
{
  return lhs.name() == rhs.name() && lhs.allocator() == rhs.allocator();
}

inline bool bit::memory::operator!=( const allocator_info& lhs,
                                     const allocator_info& rhs )
  noexcept
{
  return !(lhs==rhs);
}


//============================================================================
// debug_block_allocator
//============================================================================

//----------------------------------------------------------------------------
// Constructors
//----------------------------------------------------------------------------

template<typename BlockAllocator>
template<typename...Args>
bit::memory::debug_block_allocator<BlockAllocator>
  ::debug_block_allocator( Args&&... args )
  : m_storage( std::forward<Args>(args)... )
{

}

template<typename BlockAllocator>
bit::memory::debug_block_allocator<BlockAllocator>::~debug_block_allocator()
{
  if( m_storage.size ) {
    leak_handler({"debug",this},nullptr,m_storage.size);
  }
}

//----------------------------------------------------------------------------
// Block Allocations
//----------------------------------------------------------------------------

template<typename BlockAllocator>
bit::memory::owner<bit::memory::memory_block>
  bit::memory::debug_block_allocator<BlockAllocator>::allocate_block()
{
  using byte_t = unsigned char;

  const auto tag = static_cast<byte_t>(debug_tag::allocated_block_byte);
  auto block     = m_storage.allocate_block();
  auto first     = (byte_t*) block.data();
  auto last      = ((byte_t*) block.data()) + block.size();

  std::fill( first, last, tag );

  ++m_storage.size;
  m_storage.allocations += block_size;

  return block;
}

template<typename BlockAllocator>
void bit::memory::debug_block_allocator<BlockAllocator>
  ::deallocate_block( owner<memory_block> block )
{
  using byte_t = unsigned char;

  const auto tag = static_cast<byte_t>(debug_tag::freed_block_byte);
  auto first     = (byte_t*) block.data();
  auto last      = ((byte_t*) block.data()) + block.size();

  std::fill( first, last, tag );

  --m_storage.size;
  m_storage.allocations -= block_size;

  m_storage.deallocate_block( block );
}

//============================================================================
// debug_allocator
//============================================================================

//----------------------------------------------------------------------------
// Constructors / Destructor / Assignment
//----------------------------------------------------------------------------

template<typename Allocator>
template<typename...Args>
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

//----------------------------------------------------------------------------
// Block Allocations
//----------------------------------------------------------------------------

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
void bit::memory::debug_allocator<Allocator>
  ::deallocate( void* ptr, std::size_t bytes )
{
  using byte_t = unsigned char;

  const auto new_ptr = (byte_t*) ptr;
  const auto new_size = bytes + m_storage.fence_start_size + m_storage.fence_end_size;

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
    stomp_handler({"debug",this}, ptr, stomp );
    // call stomp_handler
  }

  // Give the underlying allocator a pointer to the start of the known
  // allocated region.
  m_storage.deallocate( f1_start, new_size );

  std::fill(a_start, a_end, static_cast<byte_t>(debug_tag::freed_byte) );

  --m_storage.allocations;
  m_storage.size -= new_size;

}
#endif /* BIT_MEMORY_DETAIL_DEBUGGING_INL */
