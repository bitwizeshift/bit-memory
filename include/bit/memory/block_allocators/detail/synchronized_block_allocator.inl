#ifndef BIT_MEMORY_BLOCK_ALLOCATORS_DETAIL_THREAD_SAFE_BLOCK_ALLOCATOR_INL
#define BIT_MEMORY_BLOCK_ALLOCATORS_DETAIL_THREAD_SAFE_BLOCK_ALLOCATOR_INL

//----------------------------------------------------------------------------
// Constructors
//----------------------------------------------------------------------------


template<typename BlockAllocator, typename BasicLockable>
template<typename Tuple0, typename Tuple1>
bit::memory::synchronized_block_allocator<BlockAllocator,BasicLockable>
  ::synchronized_block_allocator( std::piecewise_construct_t,
                                  Tuple0&& allocator_args,
                                  Tuple1&& lock_args )
  : base_type{ std::forward<Tuple0>(allocator_args),
               std::forward<Tuple1>(lock_args) }
{

}

//----------------------------------------------------------------------------
// Block Allocations
//----------------------------------------------------------------------------

template<typename BlockAllocator, typename BasicLockable>
bit::memory::owner<bit::memory::memory_block>
  bit::memory::synchronized_block_allocator<BlockAllocator,BasicLockable>
  ::allocate_block()
{
  std::lock_guard<BasicLockable> guard(detail::get<1>(*this));

  return detail::get<0>(*this).allocate_block();
}

//----------------------------------------------------------------------------

template<typename BlockAllocator, typename BasicLockable>
void bit::memory::synchronized_block_allocator<BlockAllocator,BasicLockable>
  ::deallocate_block( owner<memory_block> block )
{
  std::lock_guard<BasicLockable> guard(detail::get<1>(*this));

  return detail::get<0>(*this).deallocate_block( block );
}

#endif /* BIT_MEMORY_BLOCK_ALLOCATORS_DETAIL_THREAD_SAFE_BLOCK_ALLOCATOR_INL */
