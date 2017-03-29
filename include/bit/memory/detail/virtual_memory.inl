#ifndef BIT_MEMORY_DETAIL_VIRTUAL_MEMORY_INL
#define BIT_MEMORY_DETAIL_VIRTUAL_MEMORY_INL

//============================================================================
// virtual_memory
//============================================================================

//----------------------------------------------------------------------------
// Observers
//----------------------------------------------------------------------------

inline void* bit::memory::virtual_memory::get()
  const noexcept
{
  return m_data;
}

inline std::size_t bit::memory::virtual_memory::size()
  const noexcept
{
  return m_pages * page_size;
}

inline std::size_t bit::memory::virtual_memory::pages()
  const noexcept
{
  return m_pages;
}

#endif /* BIT_MEMORY_DETAIL_VIRTUAL_MEMORY_INL */
