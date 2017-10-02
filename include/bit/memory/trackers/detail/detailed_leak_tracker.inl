#ifndef BIT_MEMORY_TRACKERS_DETAIL_DETAILED_LEAK_TRACKER_INL
#define BIT_MEMORY_TRACKERS_DETAIL_DETAILED_LEAK_TRACKER_INL


inline void bit::memory::detailed_leak_tracker::on_allocate( void* p,
                                                             std::size_t bytes,
                                                             std::size_t align )
  noexcept
{
  BIT_MEMORY_UNUSED(align);

  // Technically this function can throw, due to the std::map. However, this is
  // called from a noexcept context in arena_allocator::try_allocate anyway;
  // and that couldn't be handled from that context either.

  m_allocated += bytes;
  m_allocations[p] = bytes;
}

inline void bit::memory::detailed_leak_tracker::on_deallocate( void* p,
                                                               std::size_t bytes )
  noexcept
{
  m_allocated -= bytes;
  m_allocations.erase(p);
}

inline void bit::memory::detailed_leak_tracker::on_deallocate_all()
  noexcept
{
  m_allocated = 0;
  m_allocations.clear();
}

inline void bit::memory::detailed_leak_tracker::finalize( const allocator_info& info )
{
  if( m_allocated != 0 ) {
    // Call the leak handler on every missed allocation
    for( auto& allocation : m_allocations ) {
      get_leak_handler()(info, allocation.first, allocation.second);
    }
  }
}

#endif /* BIT_MEMORY_TRACKERS_DETAIL_DETAILED_LEAK_TRACKER_INL */
