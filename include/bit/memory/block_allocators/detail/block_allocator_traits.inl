#ifndef BIT_MEMORY_BLOCK_ALLOCATORS_DETAIL_BLOCK_ALLOCATOR_TRAITS_INL
#define BIT_MEMORY_BLOCK_ALLOCATORS_DETAIL_BLOCK_ALLOCATOR_TRAITS_INL

template<typename BlockAllocator>
inline bit::memory::owner<bit::memory::memory_block>
  bit::memory::block_allocator_traits<BlockAllocator>
  ::allocate_block( BlockAllocator& alloc )
{
  return alloc.allocate_block();
}

template<typename BlockAllocator>
inline void bit::memory::block_allocator_traits<BlockAllocator>
  ::deallocate_block( BlockAllocator& alloc, owner<memory_block> block )
{
  return alloc.deallocate_block( block );
}

#endif /* BIT_MEMORY_BLOCK_ALLOCATORS_DETAIL_BLOCK_ALLOCATOR_TRAITS_INL */
