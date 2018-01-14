#ifndef BIT_MEMORY_TRAITS_DETAIL_BLOCK_ALLOCATOR_TRAITS_INL
#define BIT_MEMORY_TRAITS_DETAIL_BLOCK_ALLOCATOR_TRAITS_INL

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
  return do_info( block_allocator_has_info<BlockAllocator>{}, alloc );
}

template<typename BlockAllocator>
inline std::size_t bit::memory::block_allocator_traits<BlockAllocator>
  ::next_block_size( const BlockAllocator& alloc )
  noexcept
{
  return alloc.next_block_size();
}

template<typename BlockAllocator>
inline std::size_t bit::memory::block_allocator_traits<BlockAllocator>
  ::next_block_alignment( const BlockAllocator& alloc )
  noexcept
{
  return do_next_block_align_from_type( has_block_alignment{}, alloc );
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

  return {typeid(BlockAllocator).name(),std::addressof(alloc)};
}

//-----------------------------------------------------------------------------

template<typename BlockAllocator>
inline std::size_t bit::memory::block_allocator_traits<BlockAllocator>
  ::do_next_block_align_from_type( std::true_type,
                                   const BlockAllocator& alloc )
{
  BIT_MEMORY_UNUSED(alloc);

  return BlockAllocator::default_block_alignment::value;
}

template<typename BlockAllocator>
inline std::size_t bit::memory::block_allocator_traits<BlockAllocator>
  ::do_next_block_align_from_type( std::false_type,
                                   const BlockAllocator& alloc)
{
  return do_next_block_align_from_fn( block_allocator_has_next_block_alignment<BlockAllocator>{},
                                      alloc );
}

template<typename BlockAllocator>
inline std::size_t bit::memory::block_allocator_traits<BlockAllocator>
  ::do_next_block_align_from_fn( std::true_type,
                                 const BlockAllocator& alloc )
{
  return alloc.next_block_alignment();
}

template<typename BlockAllocator>
inline std::size_t bit::memory::block_allocator_traits<BlockAllocator>
  ::do_next_block_align_from_fn( std::false_type,
                                 const BlockAllocator& alloc )
{
  BIT_MEMORY_UNUSED(alloc);

  return 1;
}


#endif /* BIT_MEMORY_TRAITS_DETAIL_BLOCK_ALLOCATOR_TRAITS_INL */
