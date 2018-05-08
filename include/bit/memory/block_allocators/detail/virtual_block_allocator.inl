#ifndef BIT_MEMORY_BLOCK_ALLOCATORS_DETAIL_VIRTUAL_BLOCK_ALLOCATOR_INL
#define BIT_MEMORY_BLOCK_ALLOCATORS_DETAIL_VIRTUAL_BLOCK_ALLOCATOR_INL

//-----------------------------------------------------------------------------
// Constructors
//-----------------------------------------------------------------------------

template<std::size_t Pages, typename GrowthMultiplier>
template<std::size_t UPages, typename>
inline bit::memory::virtual_block_allocator<Pages,GrowthMultiplier>
  ::virtual_block_allocator( GrowthMultiplier growth )
  : base_type( std::forward_as_tuple(std::move(growth)) ),
    m_memory( virtual_memory_reserve( Pages ) ),
    m_active_page(0)
{

}

template<std::size_t Pages, typename GrowthMultiplier>
template<std::size_t UPages, typename>
inline bit::memory::virtual_block_allocator<Pages,GrowthMultiplier>
  ::virtual_block_allocator( std::size_t pages, GrowthMultiplier growth )
  : pages_member(pages),
    base_type( std::forward_as_tuple(std::move(growth)) ),
    m_memory( virtual_memory_reserve( pages ) ),
    m_active_page(0)
{

}

template<std::size_t Pages, typename GrowthMultiplier>
inline bit::memory::virtual_block_allocator<Pages,GrowthMultiplier>
  ::virtual_block_allocator( virtual_block_allocator&& other )
  noexcept
  : m_memory( other.m_memory ),
    m_active_page( other.m_active_page ),
    m_cache( std::move(other.m_cache) )
{
  other.m_memory      = nullptr;
  other.m_active_page = 0;
}

template<std::size_t Pages, typename GrowthMultiplier>
inline bit::memory::virtual_block_allocator<Pages,GrowthMultiplier>
  ::~virtual_block_allocator()
{
  // Releasing also decommits memory
  if( m_memory ) {
    virtual_memory_release( m_memory, pages_member::value() );
  }
}

//-----------------------------------------------------------------------------
// Block Allocations
//-----------------------------------------------------------------------------

template<std::size_t Pages, typename GrowthPolicy>
inline bit::memory::owner<bit::memory::memory_block>
  bit::memory::virtual_block_allocator<Pages,GrowthPolicy>::allocate_block()
  noexcept
{
  using ::bit::memory::get;
  using byte_t = unsigned char;

  const auto total_pages = pages_member::value();

  if( !m_cache.empty() ) {
    return m_cache.request_block();
  }

  if( (static_cast<std::size_t>(m_active_page) >= total_pages) ) {
    return nullblock;
  }

  auto& growth = get<0>(*this);
  const auto multiplier = growth.multiplier();
  const auto remaining  = total_pages - m_active_page;
  const auto pages      = (multiplier < remaining) ? multiplier : remaining;
  const auto page_size  = virtual_memory_page_size();
  const auto block_size = page_size * pages;

  auto v = static_cast<byte_t*>(m_memory) + (m_active_page * page_size);
  auto p = virtual_memory_commit( v, pages );

  m_active_page += pages;

  if( static_cast<std::size_t>(m_active_page) < total_pages ) {
    growth.grow();
  }

  return {p, block_size};
}

template<std::size_t Pages, typename GrowthPolicy>
inline void bit::memory::virtual_block_allocator<Pages,GrowthPolicy>
  ::deallocate_block( owner<memory_block> block )
  noexcept
{
  m_cache.store_block( block );
}

//-----------------------------------------------------------------------------
// Observers
//-----------------------------------------------------------------------------

template<std::size_t Pages, typename GrowthPolicy>
inline std::size_t bit::memory::virtual_block_allocator<Pages,GrowthPolicy>
  ::next_block_size()
  const noexcept
{
  using ::bit::memory::get;

  if( !m_cache.empty() ) {
    return m_cache.peek().size();
  }

  const auto total_pages = pages_member::value();

  if( (static_cast<std::size_t>(m_active_page) >= total_pages) ) {
    return 0;
  }

  const auto remaining  = total_pages - m_active_page;
  const auto multiplier = get<0>(*this).multiplier();
  const auto pages      = (multiplier < remaining) ? multiplier : remaining;
  const auto page_size  = virtual_memory_page_size();

  return page_size * pages;
}

template<std::size_t Pages, typename GrowthPolicy>
inline bit::memory::allocator_info
  bit::memory::virtual_block_allocator<Pages,GrowthPolicy>::info()
  const noexcept
{
  return {"virtual_block_allocator",this};
}

#endif /* BIT_MEMORY_BLOCK_ALLOCATORS_DETAIL_VIRTUAL_BLOCK_ALLOCATOR_INL */
