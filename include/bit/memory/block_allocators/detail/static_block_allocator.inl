#ifndef BIT_MEMORY_BLOCK_ALLOCATORS_DETAIL_STATIC_BLOCK_ALLOCATOR_INL
#define BIT_MEMORY_BLOCK_ALLOCATORS_DETAIL_STATIC_BLOCK_ALLOCATOR_INL

template<std::size_t Size, typename Tag>
bool bit::memory::static_block_allocator<Size,Tag>::s_is_allocated = false;

//----------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------

template<std::size_t Size, typename Tag>
bit::memory::static_block_allocator<Size,Tag>::static_block_allocator()
  noexcept
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
  if( !s_is_allocated ) {
    s_is_allocated = true;
    return { static_cast<void*>(s_storage), Size };
  }
  return nullblock;
}

//----------------------------------------------------------------------------

template<std::size_t Size, typename Tag>
void bit::memory::static_block_allocator<Size,Tag>
  ::deallocate_block( owner<memory_block> block )
  noexcept
{
  const auto original_block = memory_block{ static_cast<void*>(s_storage), Size };
  assert( original_block == block );

  // Only consider this deallocated if the deallocated block is the same block
  // that was originally allocated
  s_is_allocated = false;
}

#endif /* BIT_MEMORY_BLOCK_ALLOCATORS_DETAIL_STATIC_BLOCK_ALLOCATOR_INL */
