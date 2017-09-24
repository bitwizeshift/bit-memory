#ifndef BIT_MEMORY_BLOCK_ALLOCATORS_DETAIL_ALIGNED_OFFSET_BLOCK_ALLOCATOR_INL
#define BIT_MEMORY_BLOCK_ALLOCATORS_DETAIL_ALIGNED_OFFSET_BLOCK_ALLOCATOR_INL

//============================================================================
// aligned_offset_block_allocator
//============================================================================

//----------------------------------------------------------------------------
// Block Allocations
//----------------------------------------------------------------------------

template<std::size_t Size, std::size_t Align, std::size_t Offset>
inline bit::memory::memory_block
  bit::memory::aligned_offset_block_allocator<Size,Align,Offset>::
  allocate_block()
  noexcept
{
  return memory_block{ aligned_offset_malloc(Size,Align,Offset), Size, this };
}

//----------------------------------------------------------------------------

template<std::size_t Size, std::size_t Align, std::size_t Offset>
inline void
  bit::memory::aligned_offset_block_allocator<Size,Align,Offset>::
  deallocate_block( memory_block block )
  noexcept
{
  aligned_offset_free( block.data() );
}

#endif /* BIT_MEMORY_BLOCK_ALLOCATORS_DETAIL_ALIGNED_OFFSET_BLOCK_ALLOCATOR_INL */
