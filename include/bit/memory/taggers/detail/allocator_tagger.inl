#ifndef BIT_MEMORY_TAGGERS_DETAIL_DEBUG_MEMORY_TAGGER_INL
#define BIT_MEMORY_TAGGERS_DETAIL_DEBUG_MEMORY_TAGGER_INL

inline void bit::memory::allocator_tagger::tag_allocation( void* p,
                                                           std::size_t size )
  noexcept
{
  debug_tag_allocated_bytes(p,size);
}

inline void bit::memory::allocator_tagger::tag_deallocation( void* p,
                                                             std::size_t size )
  noexcept
{
  debug_tag_freed_bytes(p,size);
}

#endif /* BIT_MEMORY_TAGGERS_DETAIL_DEBUG_MEMORY_TAGGER_INL */
