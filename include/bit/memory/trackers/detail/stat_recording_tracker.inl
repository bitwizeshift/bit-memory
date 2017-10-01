#ifndef BIT_MEMORY_TRACKERS_DETAIL_STAT_RECORDING_TRACKER_INL
#define BIT_MEMORY_TRACKERS_DETAIL_STAT_RECORDING_TRACKER_INL

//-----------------------------------------------------------------------------
// Tracking
//-----------------------------------------------------------------------------

template<typename MemoryTracker>
inline void bit::memory::detail::stat_recording_tracker<MemoryTracker>
  ::on_allocate( void* p, std::size_t bytes )
  noexcept
{
  if( bytes > m_largest_request ) m_largest_request = bytes;
  if( bytes < m_smallest_request ) m_smallest_request = bytes;
  else if( bytes == 0 ) m_smallest_request = bytes;

  m_total_allocated += bytes;
  m_running_total   += bytes;

  if( m_running_total > m_peak_size ) {
    m_peak_size = m_running_total;
  }
  ++m_total_allocations;

  MemoryTracker::on_allocate(p,bytes);
}

template<typename MemoryTracker>
inline void bit::memory::detail::stat_recording_tracker<MemoryTracker>
  ::on_deallocate( void* p, std::size_t bytes )
  noexcept
{
  m_running_total -= bytes;

  ++m_total_deallocations;
  MemoryTracker::on_deallocate(p,bytes);
}

template<typename MemoryTracker>
inline void bit::memory::detail::stat_recording_tracker<MemoryTracker>
  ::on_deallocate_all()
  noexcept
{
  m_running_total = 0;
  MemoryTracker::on_deallocate_all();
}

//-----------------------------------------------------------------------------
// Element Access
//-----------------------------------------------------------------------------

template<typename MemoryTracker>
inline std::size_t
  bit::memory::detail::stat_recording_tracker<MemoryTracker>::largest_request()
  const noexcept
{
  return m_largest_request;
}

template<typename MemoryTracker>
inline std::size_t
  bit::memory::detail::stat_recording_tracker<MemoryTracker>::smallest_request()
   const noexcept
{
  return m_smallest_request;
}

template<typename MemoryTracker>
inline std::size_t
  bit::memory::detail::stat_recording_tracker<MemoryTracker>::peak_size()
  const noexcept
{
  return static_cast<std::size_t>(m_peak_size);
}

template<typename MemoryTracker>
inline std::size_t
  bit::memory::detail::stat_recording_tracker<MemoryTracker>::total_allocated()
  const noexcept
{
  return m_total_allocated;
}

template<typename MemoryTracker>
inline std::size_t
  bit::memory::detail::stat_recording_tracker<MemoryTracker>::allocations()
  const noexcept
{
  return m_total_allocations;
}

template<typename MemoryTracker>
inline std::size_t
  bit::memory::detail::stat_recording_tracker<MemoryTracker>::deallocations()
  const noexcept
{
  return m_total_deallocations;
}

#endif /* BIT_MEMORY_TRACKERS_DETAIL_STAT_RECORDING_TRACKER_INL */
