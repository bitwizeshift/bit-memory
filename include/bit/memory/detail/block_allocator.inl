#ifndef BIT_MEMORY_DETAIL_BLOCK_ALLOCATOR_INL
#define BIT_MEMORY_DETAIL_BLOCK_ALLOCATOR_INL

//----------------------------------------------------------------------------
// Constructors
//----------------------------------------------------------------------------

template<typename BlockAllocator, typename>
inline bit::memory::block_allocator::block_allocator( BlockAllocator&& allocator )
  noexcept
  : m_ptr( std::addressof(allocator) )
{
  m_allocate_block_fn = +[]( void* ptr )
  {
    return static_cast<BlockAllocator*>(ptr)->allocate_block();
  };

  m_deallocate_block_fn = +[]( void* ptr, memory_block block )
  {
    static_cast<BlockAllocator*>(ptr)->deallocate_block( block );
  };
}

//----------------------------------------------------------------------------
// Block Allocation
//----------------------------------------------------------------------------

inline bit::memory::owner<bit::memory::memory_block>
  bit::memory::block_allocator::allocate_block()
{
  return (*m_allocate_block_fn)( m_ptr );
}

//----------------------------------------------------------------------------

inline void bit::memory::block_allocator::deallocate_block( owner<memory_block> block )
{
  (*m_deallocate_block_fn)( m_ptr, block );
}

#endif /* BIT_MEMORY_DETAIL_BLOCK_ALLOCATOR_INL */
