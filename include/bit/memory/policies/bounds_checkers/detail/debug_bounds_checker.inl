#ifndef BIT_MEMORY_POLICIES_BOUNDS_CHECKERS_DETAIL_DEBUG_BOUNDS_CHECKER_INL
#define BIT_MEMORY_POLICIES_BOUNDS_CHECKERS_DETAIL_DEBUG_BOUNDS_CHECKER_INL

template<std::size_t FrontSize, std::size_t BackSize>
inline void bit::memory::debug_bounds_checker<FrontSize,BackSize>
  ::prepare_front_fence( void* p, std::size_t size )
  noexcept
{
  debug_tag_fence_start_bytes( p, size );
}

template<std::size_t FrontSize, std::size_t BackSize>
inline void bit::memory::debug_bounds_checker<FrontSize,BackSize>
  ::prepare_back_fence( void* p, std::size_t size )
  noexcept
{
  debug_tag_fence_end_bytes( p, size );
}

template<std::size_t FrontSize, std::size_t BackSize>
inline void bit::memory::debug_bounds_checker<FrontSize,BackSize>
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

template<std::size_t FrontSize, std::size_t BackSize>
inline void bit::memory::debug_bounds_checker<FrontSize,BackSize>
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

#endif /* BIT_MEMORY_POLICIES_BOUNDS_CHECKERS_DETAIL_DEBUG_BOUNDS_CHECKER_INL */
