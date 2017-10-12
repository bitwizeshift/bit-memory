#ifndef BIT_MEMORY_TRACKERS_DETAIL_STDOUT_TRACKER_INL
#define BIT_MEMORY_TRACKERS_DETAIL_STDOUT_TRACKER_INL

inline void bit::memory::stdout_tracker::on_allocate( void* p,
                                                      std::size_t bytes,
                                                      std::size_t align )
  noexcept
{
  std::printf("Memory Allocated:\n  Address=%p\n  Size=%zu\n", p, bytes );
}

inline void bit::memory::stdout_tracker
  ::on_deallocate( const allocator_info& info, void* p, std::size_t bytes )
  noexcept
{
  BIT_MEMORY_UNUSED(info);

  std::printf("Memory Deallocated:\n  Address=%p\n  Size=%zu\n", p, bytes );
}

inline void bit::memory::stdout_tracker::on_deallocate_all()
  noexcept
{
  std::printf("Memory Truncate-Deallocated\n");
}

inline void bit::memory::stdout_tracker::finalize( const allocator_info& info )
  noexcept
{
  BIT_MEMORY_UNUSED(info);
}

#endif /* BIT_MEMORY_TRACKERS_DETAIL_STDOUT_TRACKER_INL */
