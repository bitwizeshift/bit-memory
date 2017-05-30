#ifndef BIT_MEMORY_DETAIL_BLOCK_ALLOCATOR_INL
#define BIT_MEMORY_DETAIL_BLOCK_ALLOCATOR_INL

//----------------------------------------------------------------------------
// Constructors
//----------------------------------------------------------------------------

template<typename BlockAllocator, typename>
inline bit::memory::block_allocator::block_allocator( BlockAllocator&& allocator )
  noexcept
  : m_ptr( std::addressof(allocator) ),
    m_allocate_block_fn(&allocate_block_function<BlockAllocator>),
    m_deallocate_block_fn(&deallocate_block_function<BlockAllocator>)
{

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

//----------------------------------------------------------------------------
// Public Static Functions
//----------------------------------------------------------------------------

template<typename BlockAllocator>
inline bit::memory::owner<bit::memory::memory_block>
  bit::memory::block_allocator::allocate_block_function( void* instance )
{
  return static_cast<BlockAllocator*>(instance)->allocate_block();
}

//----------------------------------------------------------------------------

template<typename BlockAllocator>
inline void bit::memory::block_allocator
  ::deallocate_block_function( void* instance, owner<memory_block> block )
{
  static_cast<BlockAllocator*>(instance)->deallocate_block( block );
}
#endif /* BIT_MEMORY_DETAIL_BLOCK_ALLOCATOR_INL */
