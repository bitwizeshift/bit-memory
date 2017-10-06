#ifndef BIT_MEMORY_BLOCK_ALLOCATORS_DETAIL_POLICY_BLOCK_ALLOCATOR_INL
#define BIT_MEMORY_BLOCK_ALLOCATORS_DETAIL_POLICY_BLOCK_ALLOCATOR_INL

//=============================================================================
// policy_block_allocator
//=============================================================================

//-----------------------------------------------------------------------------
// Constructors / Destructor
//-----------------------------------------------------------------------------

template<typename BlockAllocator, typename Tagger, typename Tracker, typename Lock>
template<typename...Args,typename>
inline bit::memory::policy_block_allocator<BlockAllocator,Tagger,Tracker,Lock>
  ::policy_block_allocator( Args&&...args )
  : base_type( std::forward_as_tuple( std::forward<Args>(args)... ),
               std::make_tuple(),
               std::make_tuple(),
               std::make_tuple() )
{

}

//-----------------------------------------------------------------------------

template<typename BlockAllocator, typename Tagger, typename Tracker, typename Lock>
inline bit::memory::policy_block_allocator<BlockAllocator,Tagger,Tracker,Lock>
  ::~policy_block_allocator()
{
  auto& allocator = detail::get<0>(*this);
  auto& tracker   = detail::get<2>(*this);

  tracker.finalize( traits_type::info(allocator) );
}

//-----------------------------------------------------------------------------
// Block Allocations
//-----------------------------------------------------------------------------

template<typename BlockAllocator, typename Tagger, typename Tracker, typename Lock>
inline bit::memory::owner<bit::memory::memory_block>
  bit::memory::policy_block_allocator<BlockAllocator,Tagger,Tracker,Lock>
  ::allocate_block()
{
  auto& allocator = detail::get<0>(*this);
  auto& tagger    = detail::get<1>(*this);
  auto& tracker   = detail::get<2>(*this);
  auto& lock      = detail::get<3>(*this);

  auto block = memory_block();

  { // critical section
    std::lock_guard<lock_type> scope(lock);

    block = traits_type::allocate_block(allocator);

    if( BIT_MEMORY_UNLIKELY( block == nullblock ) ) return nullblock;

    tracker.on_allocate( block.data(), block.size(), block_align( typename traits_type::has_block_alignment{} ) );
  }

  tagger.tag_allocation( block.data(), block.size() );

  return block;
}

//-----------------------------------------------------------------------------

template<typename BlockAllocator, typename Tagger, typename Tracker, typename Lock>
inline void bit::memory::policy_block_allocator<BlockAllocator,Tagger,Tracker,Lock>
  ::deallocate_block( owner<memory_block> block )
{
  auto& allocator = detail::get<0>(*this);
  auto& tagger    = detail::get<1>(*this);
  auto& tracker   = detail::get<2>(*this);
  auto& lock      = detail::get<3>(*this);

  // Tag the deallocation
  tagger.tag_deallocation( block.data(), block.size() );

  { // critical section
    std::lock_guard<lock_type> scope(lock);

    // Untrack the deallocation
    tracker.on_deallocate( block.data(), block.size() );

    traits_type::deallocate_block( allocator, block );
  }
}

//-----------------------------------------------------------------------------
// Observers
//-----------------------------------------------------------------------------

template<typename BlockAllocator, typename Tagger, typename Tracker, typename Lock>
inline const Tracker&
  bit::memory::policy_block_allocator<BlockAllocator,Tagger,Tracker,Lock>::tracker()
  const noexcept
{
  return detail::get<2>(*this);
}

//-----------------------------------------------------------------------------

template<typename BlockAllocator, typename Tagger, typename Tracker, typename Lock>
template<typename,typename>
inline bit::memory::allocator_info
  bit::memory::policy_block_allocator<BlockAllocator,Tagger,Tracker,Lock>
  ::info()
  const noexcept
{
  return traits_type::info( detail::get<0>(*this) );
}

//-----------------------------------------------------------------------------
// Private Members
//-----------------------------------------------------------------------------

template<typename BlockAllocator, typename Tagger, typename Tracker, typename Lock>
inline std::size_t
  bit::memory::policy_block_allocator<BlockAllocator,Tagger,Tracker,Lock>
  ::block_align( std::true_type )
  const noexcept
{
  return traits_type::block_alignment( detail::get<0>(*this) );
}

template<typename BlockAllocator, typename Tagger, typename Tracker, typename Lock>
inline std::size_t
  bit::memory::policy_block_allocator<BlockAllocator,Tagger,Tracker,Lock>
  ::block_align( std::false_type )
  const noexcept
{
  return 1;
}

#endif /* BIT_MEMORY_BLOCK_ALLOCATORS_DETAIL_POLICY_BLOCK_ALLOCATOR_INL */
