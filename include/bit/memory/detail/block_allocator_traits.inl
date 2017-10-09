#ifndef BIT_MEMORY_BLOCK_ALLOCATORS_DETAIL_BLOCK_ALLOCATOR_TRAITS_INL
#define BIT_MEMORY_BLOCK_ALLOCATORS_DETAIL_BLOCK_ALLOCATOR_TRAITS_INL

//-----------------------------------------------------------------------------
// Block Allocations
//-----------------------------------------------------------------------------

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

//-----------------------------------------------------------------------------
// Observers
//-----------------------------------------------------------------------------

template<typename BlockAllocator>
inline bit::memory::allocator_info
  bit::memory::block_allocator_traits<BlockAllocator>
  ::info( const BlockAllocator& alloc )
  noexcept
{
  return do_info( detail::block_allocator_has_info<BlockAllocator>{}, alloc );
}

//-----------------------------------------------------------------------------
// Capacity / Alignment
//-----------------------------------------------------------------------------

template<typename BlockAllocator>
inline constexpr std::size_t bit::memory::block_allocator_traits<BlockAllocator>
  ::block_alignment( BlockAllocator& alloc )
  noexcept
{
  BIT_MEMORY_UNUSED(alloc);

  static_assert( has_block_alignment::value, "Only valid for BlockAllocators that know their alignment" );

  return BlockAllocator::block_alignment::value;
}

template<typename BlockAllocator>
inline constexpr std::size_t bit::memory::block_allocator_traits<BlockAllocator>
  ::block_size( BlockAllocator& alloc )
  noexcept
{
  BIT_MEMORY_UNUSED(alloc);

  static_assert( has_block_size::value, "Only valid for BlockAllocators that know their size" );

  return BlockAllocator::block_size::value;
}

//-----------------------------------------------------------------------------
// Private Implementation
//-----------------------------------------------------------------------------


template<typename BlockAllocator>
inline bit::memory::allocator_info
  bit::memory::block_allocator_traits<BlockAllocator>
  ::do_info( std::true_type, const BlockAllocator& alloc )
{
  return alloc.info();
}

template<typename BlockAllocator>
inline bit::memory::allocator_info
  bit::memory::block_allocator_traits<BlockAllocator>
  ::do_info( std::false_type, const BlockAllocator& alloc )
{
  BIT_MEMORY_UNUSED(alloc);

  return {"Unnamed",std::addressof(alloc)};
}

#endif /* BIT_MEMORY_BLOCK_ALLOCATORS_DETAIL_BLOCK_ALLOCATOR_TRAITS_INL */
