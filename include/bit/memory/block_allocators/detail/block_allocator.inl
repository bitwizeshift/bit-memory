#ifndef BIT_MEMORY_BLOCK_ALLOCATORS_DETAIL_BLOCK_ALLOCATOR_INL
#define BIT_MEMORY_BLOCK_ALLOCATORS_DETAIL_BLOCK_ALLOCATOR_INL

//============================================================================
// block_allocator
//============================================================================

//----------------------------------------------------------------------------
// Constructors
//----------------------------------------------------------------------------

template<typename BlockAllocator, typename>
inline bit::memory::block_allocator::block_allocator( BlockAllocator&& allocator )
  noexcept
  : m_ptr( std::addressof(allocator) ),
    m_vtable( vtable_type::get_vtable<BlockAllocator>() )
{

}

//----------------------------------------------------------------------------
// Block Allocation
//----------------------------------------------------------------------------

inline bit::memory::owner<bit::memory::memory_block>
  bit::memory::block_allocator::allocate_block()
{
  return m_vtable->allocate_fn( m_ptr );
}

//----------------------------------------------------------------------------

inline void bit::memory::block_allocator
  ::deallocate_block( owner<memory_block> block )
{
  m_vtable->deallocate_fn( m_ptr, block );
}

#endif /* BIT_MEMORY_BLOCK_ALLOCATORS_DETAIL_BLOCK_ALLOCATOR_INL */
