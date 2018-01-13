#ifndef BIT_MEMORY_UTILITIES_DETAIL_DEBUGGING_INL
#define BIT_MEMORY_UTILITIES_DETAIL_DEBUGGING_INL

//-----------------------------------------------------------------------------
// Tagging
//-----------------------------------------------------------------------------

inline void bit::memory::debug_tag_block_allocated_bytes( void* p,
                                                          std::size_t n )
{
  debug_tag_bytes( p, n, debug_tag::allocated_block_byte );
}

inline void bit::memory::debug_tag_block_freed_bytes( void* p,
                                                      std::size_t n )
{
  debug_tag_bytes( p, n, debug_tag::freed_block_byte );
}

inline void bit::memory::debug_tag_fence_start_bytes( void* p,
                                                      std::size_t n )
{
  debug_tag_bytes( p, n, debug_tag::fence_start_byte );
}

inline void bit::memory::debug_tag_fence_end_bytes( void* p,
                                                    std::size_t n )
{
  debug_tag_bytes( p, n, debug_tag::fence_end_byte );
}

inline void bit::memory::debug_tag_allocated_bytes( void* p,
                                                    std::size_t n )
{
  debug_tag_bytes( p, n, debug_tag::allocated_byte );
}

inline void bit::memory::debug_tag_freed_bytes( void* p,
                                                std::size_t n )
{
  debug_tag_bytes( p, n, debug_tag::freed_byte );
}

//-----------------------------------------------------------------------------
// Untag
//-----------------------------------------------------------------------------

inline void* bit::memory::debug_untag_fence_start_bytes( void* p,
                                                         std::size_t n,
                                                         std::size_t* stomped )
{
  return debug_untag_bytes( p, n, debug_tag::fence_start_byte, stomped );
}

inline void* bit::memory::debug_untag_fence_end_bytes( void* p,
                                                       std::size_t n,
                                                       std::size_t* stomped )
{
  return debug_untag_bytes( p, n, debug_tag::fence_end_byte, stomped );
}

#endif /* BIT_MEMORY_UTILITIES_DETAIL_DEBUGGING_INL */
