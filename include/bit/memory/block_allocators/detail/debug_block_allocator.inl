#ifndef BIT_MEMORY_BLOCK_ALLOCATORS_DETAIL_DEBUG_BLOCK_ALLOCATOR_INL
#define BIT_MEMORY_BLOCK_ALLOCATORS_DETAIL_DEBUG_BLOCK_ALLOCATOR_INL

//============================================================================
// debug_block_allocator
//============================================================================

//----------------------------------------------------------------------------
// Constructors
//----------------------------------------------------------------------------

template<typename BlockAllocator>
template<typename...Args, decltype(BlockAllocator( std::declval<Args>()... ), void())*>
bit::memory::debug_block_allocator<BlockAllocator>
  ::debug_block_allocator( Args&&... args )
  : m_storage( std::forward<Args>(args)... )
{

}

template<typename BlockAllocator>
bit::memory::debug_block_allocator<BlockAllocator>::~debug_block_allocator()
{
  if( m_storage.size ) {
    leak_handler({"debug",&m_storage},nullptr,m_storage.size);
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

  const auto tag  = static_cast<byte_t>(debug_tag::freed_block_byte);
  const auto size = block.size();
  auto first      = (byte_t*) block.data();
  auto last       = first + block.size();
  auto i          = 0;

  // Check for double-deletion
  std::for_each( first, last, [&]( const byte_t& byte ){
    if( byte == tag ) {
      ++i;
    }
  });

  if( i == size ) {
    double_delete_handler( {"debug",&m_storage}, block.data(), block.size() );
  }

  std::fill( first, last, tag );

  --m_storage.allocations;
  m_storage.size -= block_size;

  m_storage.deallocate_block( block );
}

#endif /* BIT_MEMORY_BLOCK_ALLOCATORS_DETAIL_DEBUG_BLOCK_ALLOCATOR_INL */
