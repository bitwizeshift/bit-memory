#ifndef BIT_MEMORY_BLOCK_ALLOCATORS_DETAIL_ANY_BLOCK_ALLOCATOR_INL
#define BIT_MEMORY_BLOCK_ALLOCATORS_DETAIL_ANY_BLOCK_ALLOCATOR_INL

//=============================================================================
// any_block_allocator
//=============================================================================

//-----------------------------------------------------------------------------
// Constructors
//-----------------------------------------------------------------------------

template<typename BlockAllocator, typename>
inline bit::memory::any_block_allocator::any_block_allocator( BlockAllocator&& allocator )
  noexcept
  : m_ptr( std::addressof(allocator) ),
    m_vtable( vtable_type::get_vtable<std::decay_t<BlockAllocator>>() )
{

}

//-----------------------------------------------------------------------------
// Block Allocation
//-----------------------------------------------------------------------------

inline bit::memory::owner<bit::memory::memory_block>
  bit::memory::any_block_allocator::allocate_block()
{
  return m_vtable->allocate_fn( m_ptr );
}

//-----------------------------------------------------------------------------

inline void bit::memory::any_block_allocator
  ::deallocate_block( owner<memory_block> block )
{
  m_vtable->deallocate_fn( m_ptr, block );
}

//-----------------------------------------------------------------------------
// Observers
//-----------------------------------------------------------------------------

inline bit::memory::allocator_info bit::memory::any_block_allocator::info()
  const noexcept
{
  return m_vtable->info_fn( m_ptr );
}

inline std::size_t bit::memory::any_block_allocator::next_block_size()
  const noexcept
{
  return m_vtable->next_block_fn( m_ptr );
}

#endif /* BIT_MEMORY_BLOCK_ALLOCATORS_DETAIL_ANY_BLOCK_ALLOCATOR_INL */
