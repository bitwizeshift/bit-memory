#ifndef BIT_MEMORY_BOUNDS_CHECKERS_DETAIL_DEBUG_BOUNDS_CHECKER_INL
#define BIT_MEMORY_BOUNDS_CHECKERS_DETAIL_DEBUG_BOUNDS_CHECKER_INL

inline void bit::memory::debug_bounds_checker
  ::prepare_front_fence( void* p, std::size_t size )
  noexcept
{
  debug_tag_fence_start_bytes( p, size );
}

inline void bit::memory::debug_bounds_checker
  ::prepare_back_fence( void* p, std::size_t size )
  noexcept
{
  debug_tag_fence_end_bytes( p, size );
}

inline void bit::memory::debug_bounds_checker
  ::check_front_fence( const allocator_info& info,
                       void* p,
                       std::size_t size )
  noexcept
{
 auto stomped = std::size_t{};
 if( auto start = debug_untag_fence_start_bytes(p,size,&stomped) )
 {
   const auto handler = get_buffer_overflow_handler();
   (*handler)( info, start, static_cast<std::ptrdiff_t>(stomped) );
 }
}

inline void bit::memory::debug_bounds_checker
  ::check_back_fence( const allocator_info& info,
                      void* p,
                      std::size_t size ) noexcept
{
  auto stomped = std::size_t{};
  if( auto start = debug_untag_fence_end_bytes(p,size,&stomped) )
  {
    const auto handler = get_buffer_overflow_handler();
    (*handler)( info, start, static_cast<std::ptrdiff_t>(stomped) );
  }
}

#endif /* BIT_MEMORY_BOUNDS_CHECKERS_DETAIL_DEBUG_BOUNDS_CHECKER_INL */
