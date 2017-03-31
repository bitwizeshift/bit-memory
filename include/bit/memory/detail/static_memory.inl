#ifndef BIT_MEMORY_DETAIL_STATIC_MEMORY_INL
#define BIT_MEMORY_DETAIL_STATIC_MEMORY_INL

//----------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------

template<std::size_t Size, typename Tag>
bit::memory::static_block_allocator<Size,Tag>::static_block_allocator()
  noexcept
  : m_is_allocated(false)
{

}

//----------------------------------------------------------------------------
// Block Allocation
//----------------------------------------------------------------------------

template<std::size_t Size, typename Tag>
bit::memory::owner<bit::memory::memory_block>
  bit::memory::static_block_allocator<Size,Tag>::allocate_block()
  noexcept
{
  if( !m_is_allocated ) {
    m_is_allocated = true;
    return memory_block{ (void*) storage, Size };
  }
  return nullblock;
}

//----------------------------------------------------------------------------

template<std::size_t Size, typename Tag>
void bit::memory::static_block_allocator<Size,Tag>
  ::deallocate_block( owner<memory_block> block )
  noexcept
{
  // Only consider this deallocated if the deallocated block is the same block
  // that was originally allocated
  m_is_allocated = !(memory_block{ (void*) storage, Size } == block);
}

#endif /* BIT_MEMORY_DETAIL_STATIC_MEMORY_INL */
