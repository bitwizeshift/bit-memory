#ifndef BIT_MEMORY_POLICIES_TAGGERS_DETAIL_BLOCK_ALLOCATOR_TAGGER_INL
#define BIT_MEMORY_POLICIES_TAGGERS_DETAIL_BLOCK_ALLOCATOR_TAGGER_INL

inline void bit::memory::block_allocator_tagger
  ::tag_allocation( void* p, std::size_t size )
  noexcept
{
  debug_tag_block_allocated_bytes(p,size);
}

inline void bit::memory::block_allocator_tagger
  ::tag_deallocation( void* p, std::size_t size )
  noexcept
{
  debug_tag_block_freed_bytes(p,size);
}

#endif /* BIT_MEMORY_POLICIES_TAGGERS_DETAIL_BLOCK_ALLOCATOR_TAGGER_INL */
