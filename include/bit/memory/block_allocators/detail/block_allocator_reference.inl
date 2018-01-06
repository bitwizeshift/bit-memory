#ifndef BIT_MEMORY_BLOCK_ALLOCATORS_DETAIL_BLOCK_ALLOCATOR_REFERENCE_INL
#define BIT_MEMORY_BLOCK_ALLOCATORS_DETAIL_BLOCK_ALLOCATOR_REFERENCE_INL

//=============================================================================
// detail::any_block_allocator_vtable
//=============================================================================

struct bit::memory::detail::block_allocator_reference_vtable
{
  //---------------------------------------------------------------------------
  // Public Member Types
  //---------------------------------------------------------------------------

  using deallocate_block_fn_t = void(*)( void*, memory_block );
  using allocate_block_fn_t   = memory_block(*)( void* );
  using info_fn_t             = allocator_info(*)( const void* );
  using next_block_size_fn_t  = std::size_t(*)( const void* );

  //---------------------------------------------------------------------------
  // Public Members
  //---------------------------------------------------------------------------

  deallocate_block_fn_t deallocate_fn;
  allocate_block_fn_t   allocate_fn;
  info_fn_t             info_fn;
  next_block_size_fn_t  next_block_fn;

  //---------------------------------------------------------------------------
  // Static Creation
  //---------------------------------------------------------------------------

  template<typename BlockAllocator>
  static block_allocator_reference_vtable* get_vtable();
};

//-----------------------------------------------------------------------------
// Static Creation
//-----------------------------------------------------------------------------

template<typename BlockAllocator>
bit::memory::detail::block_allocator_reference_vtable*
  bit::memory::detail::block_allocator_reference_vtable::get_vtable()

{
  using traits_type = block_allocator_traits<BlockAllocator>;

  static auto s_vtable = []()
  {
    block_allocator_reference_vtable table;

    table.allocate_fn = [](void* p) -> bit::memory::memory_block
    {
      auto* instance = static_cast<BlockAllocator*>(p);

      return traits_type::allocate_block( *instance );
    };

    table.deallocate_fn = [](void* p, memory_block block)
    {
      auto* instance = static_cast<BlockAllocator*>(p);

      traits_type::deallocate_block( *instance, block );
    };

    table.info_fn = []( const void* p ) -> allocator_info
    {
      auto* instance = static_cast<const BlockAllocator*>(p);

      return traits_type::info( *instance );
    };

    table.next_block_fn = []( const void* p ) -> std::size_t
    {
      auto* instance = static_cast<const BlockAllocator*>(p);

      return traits_type::next_block_size( *instance );
    };

    return table;
  }();

  return &s_vtable;
}

//=============================================================================
// any_block_allocator
//=============================================================================

//-----------------------------------------------------------------------------
// Constructors
//-----------------------------------------------------------------------------

template<typename BlockAllocator, typename>
inline bit::memory::block_allocator_reference
  ::block_allocator_reference( BlockAllocator& allocator )
  noexcept
  : m_ptr( std::addressof(allocator) ),
    m_vtable( vtable_type::get_vtable<std::decay_t<BlockAllocator>>() )
{

}

//-----------------------------------------------------------------------------
// Block Allocation
//-----------------------------------------------------------------------------

inline bit::memory::owner<bit::memory::memory_block>
  bit::memory::block_allocator_reference::allocate_block()
{
  return m_vtable->allocate_fn( m_ptr );
}

//-----------------------------------------------------------------------------

inline void bit::memory::block_allocator_reference
  ::deallocate_block( owner<memory_block> block )
{
  m_vtable->deallocate_fn( m_ptr, block );
}

//-----------------------------------------------------------------------------
// Observers
//-----------------------------------------------------------------------------

inline bit::memory::allocator_info
  bit::memory::block_allocator_reference::info()
  const noexcept
{
  return m_vtable->info_fn( m_ptr );
}

inline std::size_t bit::memory::block_allocator_reference::next_block_size()
  const noexcept
{
  return m_vtable->next_block_fn( m_ptr );
}

#endif /* BIT_MEMORY_BLOCK_ALLOCATORS_DETAIL_BLOCK_ALLOCATOR_REFERENCE_INL */
