#ifndef BIT_MEMORY_ALLOCATORS_DETAIL_NAMED_ALLOCATOR_INL
#define BIT_MEMORY_ALLOCATORS_DETAIL_NAMED_ALLOCATOR_INL

//=============================================================================
// named_allocator
//=============================================================================

//-----------------------------------------------------------------------------
// Allocator
//-----------------------------------------------------------------------------

template<typename Allocator>
template<typename...Args, typename>
inline bit::memory::detail::named_allocator<Allocator>
  ::named_allocator( const char* str, Args&&...args )
  : Allocator( std::forward<Args>(args)...),
    m_name(str)
{

}

//-----------------------------------------------------------------------------
// Observers
//-----------------------------------------------------------------------------

template<typename Allocator>
bit::memory::allocator_info
  bit::memory::detail::named_allocator<Allocator>::info()
  const noexcept
{
  return {m_name,this};
}

#endif /* BIT_MEMORY_ALLOCATORS_DETAIL_NAMED_ALLOCATOR_INL */
