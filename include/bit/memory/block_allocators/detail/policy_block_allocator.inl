#ifndef BIT_MEMORY_BLOCK_ALLOCATORS_DETAIL_POLICY_BLOCK_ALLOCATOR_INL
#define BIT_MEMORY_BLOCK_ALLOCATORS_DETAIL_POLICY_BLOCK_ALLOCATOR_INL

//=============================================================================
// policy_block_allocator
//=============================================================================

//-----------------------------------------------------------------------------
// Constructors / Destructor
//-----------------------------------------------------------------------------

template<typename BlockAllocator, typename Tagger, typename Tracker, typename Lock>
template<typename Arg0, typename...Args,typename>
inline bit::memory::policy_block_allocator<BlockAllocator,Tagger,Tracker,Lock>
  ::policy_block_allocator( Arg0&& arg0, Args&&...args )
  : base_type( std::forward_as_tuple( std::forward<Arg0>(arg0),
                                      std::forward<Args>(args)... ),
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
  auto& allocator = get<0>(*this);
  auto& tracker   = get<2>(*this);

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
  auto& allocator = get<0>(*this);
  auto& tagger    = get<1>(*this);
  auto& tracker   = get<2>(*this);
  auto& lock      = get<3>(*this);

  auto block = memory_block();

  { // critical section
    std::lock_guard<lock_type> scope(lock);

    block = traits_type::allocate_block(allocator);

    if( BIT_MEMORY_UNLIKELY( block == nullblock ) ) return nullblock;

    tracker.on_allocate( block.data(), block.size(), traits_type::next_block_alignment(allocator) );
  }

  tagger.tag_allocation( block.data(), block.size() );

  return block;
}

//-----------------------------------------------------------------------------

template<typename BlockAllocator, typename Tagger, typename Tracker, typename Lock>
inline void bit::memory::policy_block_allocator<BlockAllocator,Tagger,Tracker,Lock>
  ::deallocate_block( owner<memory_block> block )
{
  auto& allocator = get<0>(*this);
  auto& tagger    = get<1>(*this);
  auto& tracker   = get<2>(*this);
  auto& lock      = get<3>(*this);

  const auto info = traits_type::info(allocator);

  // Tag the deallocation
  tagger.tag_deallocation( block.data(), block.size() );

  { // critical section
    std::lock_guard<lock_type> scope(lock);

    // Untrack the deallocation
    tracker.on_deallocate( info, block.data(), block.size() );

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
  return get<2>(*this);
}

//-----------------------------------------------------------------------------

template<typename BlockAllocator, typename Tagger, typename Tracker, typename Lock>
inline bit::memory::allocator_info
  bit::memory::policy_block_allocator<BlockAllocator,Tagger,Tracker,Lock>
  ::info()
  const noexcept
{
  return traits_type::info( get<0>(*this) );
}

template<typename BlockAllocator, typename Tagger, typename Tracker, typename Lock>
inline std::size_t
  bit::memory::policy_block_allocator<BlockAllocator,Tagger,Tracker,Lock>
  ::next_block_size()
  const noexcept
{
  return traits_type::next_block_size( get<0>(*this) );
}

#endif /* BIT_MEMORY_BLOCK_ALLOCATORS_DETAIL_POLICY_BLOCK_ALLOCATOR_INL */
