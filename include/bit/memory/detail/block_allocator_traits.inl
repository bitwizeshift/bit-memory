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

template<typename BlockAllocator>
inline constexpr std::size_t bit::memory::block_allocator_traits<BlockAllocator>
  ::block_alignment( BlockAllocator& alloc )
  noexcept
{
  static_assert( has_block_alignment::value, "Only valid for BlockAllocators that know their alignment" );

  return BlockAllocator::block_alignment::value;
}

template<typename BlockAllocator>
inline constexpr std::size_t bit::memory::block_allocator_traits<BlockAllocator>
  ::block_size( BlockAllocator& alloc )
  noexcept
{
  static_assert( has_block_size::value, "Only valid for BlockAllocators that know their size" );

  return BlockAllocator::block_size::value;
}

#endif /* BIT_MEMORY_BLOCK_ALLOCATORS_DETAIL_BLOCK_ALLOCATOR_TRAITS_INL */
