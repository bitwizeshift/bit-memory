#ifndef BIT_MEMORY_BLOCK_ALLOCATORS_DETAIL_NAMED_BLOCK_ALLOCATOR_INL
#define BIT_MEMORY_BLOCK_ALLOCATORS_DETAIL_NAMED_BLOCK_ALLOCATOR_INL

//=============================================================================
// named_allocator
//=============================================================================

//-----------------------------------------------------------------------------
// Allocator
//-----------------------------------------------------------------------------

template<typename BlockAllocator>
template<typename...Args, typename>
inline bit::memory::detail::named_block_allocator<BlockAllocator>
  ::named_block_allocator( const char* str, Args&&...args )
  : BlockAllocator( std::forward<Args>(args)...),
    m_name(str)
{

}

//-----------------------------------------------------------------------------
// Observers
//-----------------------------------------------------------------------------

template<typename BlockAllocator>
bit::memory::allocator_info
  bit::memory::detail::named_block_allocator<BlockAllocator>::info()
  const noexcept
{
  return {m_name,this};
}

#endif /* BIT_MEMORY_BLOCK_ALLOCATORS_DETAIL_NAMED_BLOCK_ALLOCATOR_INL */
