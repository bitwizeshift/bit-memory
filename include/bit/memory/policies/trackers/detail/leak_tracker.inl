#ifndef BIT_MEMORY_POLICIES_TRACKERS_DETAIL_LEAK_TRACKER_INL
#define BIT_MEMORY_POLICIES_TRACKERS_DETAIL_LEAK_TRACKER_INL

//-----------------------------------------------------------------------------
// Tracking
//-----------------------------------------------------------------------------

inline void bit::memory::leak_tracker::on_allocate( void* p,
                                                    std::size_t bytes,
                                                    std::size_t align )
  noexcept
{
  BIT_MEMORY_UNUSED(p);
  BIT_MEMORY_UNUSED(align);

  m_allocated += bytes;
}

inline void bit::memory::leak_tracker::on_deallocate( const allocator_info& info,
                                                      void* p,
                                                      std::size_t bytes )
  noexcept
{
  BIT_MEMORY_UNUSED(info);
  BIT_MEMORY_UNUSED(p);

  m_allocated -= bytes;
}

inline void bit::memory::leak_tracker::on_deallocate_all()
  noexcept
{
  m_allocated = 0;
}

inline void bit::memory::leak_tracker::finalize( const allocator_info& info )
{
  if( m_allocated != 0 ) {
    get_leak_handler()(info, nullptr, m_allocated);
  }
}

#endif /* BIT_MEMORY_POLICIES_TRACKERS_DETAIL_LEAK_TRACKER_INL */
