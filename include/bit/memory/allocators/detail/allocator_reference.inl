#ifndef BIT_MEMORY_ALLOCATORS_DETAIL_ALLOCATOR_REFERENCE_INL
#define BIT_MEMORY_ALLOCATORS_DETAIL_ALLOCATOR_REFERENCE_INL

//=============================================================================
// detail::allocator_vtable
//=============================================================================

struct bit::memory::detail::allocator_vtable
{
  //---------------------------------------------------------------------------
  // Public Member Types
  //---------------------------------------------------------------------------

  using allocate_fn_t     = void*(*)( void*, std::size_t, std::size_t );
  using try_allocate_fn_t = void*(*)( void*, std::size_t, std::size_t );
  using deallocate_fn_t   = void(*)( void*, void*,std::size_t );
  using info_fn_t         = allocator_info(*)(const void*);

  //---------------------------------------------------------------------------
  // Public Members
  //---------------------------------------------------------------------------

  allocate_fn_t     allocate_fn;
  try_allocate_fn_t try_allocate_fn;
  deallocate_fn_t   deallocate_fn;
  info_fn_t         info_fn;

  //---------------------------------------------------------------------------
  // Static Creation
  //---------------------------------------------------------------------------

  template<typename Allocator>
  static allocator_vtable* get_stateful_vtable();

  //---------------------------------------------------------------------------

  template<typename Allocator>
  static allocator_vtable* get_stateless_vtable();
};

//-----------------------------------------------------------------------------
// Static Creation
//-----------------------------------------------------------------------------

template<typename Allocator>
inline bit::memory::detail::allocator_vtable*
  bit::memory::detail::allocator_vtable::get_stateful_vtable()
{
  using traits_type = allocator_traits<Allocator>;

  static auto s_vtable = []()
  {
    allocator_vtable table;

    table.try_allocate_fn = [](void* p, std::size_t size, std::size_t align )
    {
      auto* instance = static_cast<Allocator*>(p);
      return traits_type::try_allocate( *instance, size, align );
    };

    table.allocate_fn = [](void* p, std::size_t size, std::size_t align )
    {
      auto* instance = static_cast<Allocator*>(p);
      return traits_type::allocate( *instance, size, align );
    };

    table.deallocate_fn = [](void* p, void* ptr, std::size_t size )
    {
      auto* instance = static_cast<Allocator*>(p);
      return traits_type::deallocate( *instance, ptr, size );
    };

    table.info_fn = [](const void* p) -> allocator_info
    {
      auto* instance = static_cast<const Allocator*>(p);
      return traits_type::info( *instance );
    };

    return table;
  }();

  return &s_vtable;
}

//-----------------------------------------------------------------------------

template<typename Allocator>
inline bit::memory::detail::allocator_vtable*
  bit::memory::detail::allocator_vtable::get_stateless_vtable()
{
  static_assert( is_stateless_v<Allocator>,
                 "'Allocator' must be stateless" );

  using traits_type = allocator_traits<Allocator>;

  static auto s_vtable = []()
  {
    allocator_vtable table;

    table.try_allocate_fn = [](void* p, std::size_t size, std::size_t align )
    {
      BIT_MEMORY_UNUSED(p);

      auto instance = Allocator{};
      return traits_type::try_allocate( instance, size, align );
    };

    table.allocate_fn = [](void* p, std::size_t size, std::size_t align )
    {
      BIT_MEMORY_UNUSED(p);

      auto instance = Allocator{};
      return traits_type::allocate( instance, size, align );
    };

    table.deallocate_fn = [](void* p, void* ptr, std::size_t size )
    {
      BIT_MEMORY_UNUSED(p);

      auto instance = Allocator{};
      return traits_type::deallocate( instance, ptr, size );
    };

    table.info_fn = [](const void* p) -> allocator_info
    {
      BIT_MEMORY_UNUSED(p);

      auto instance = Allocator{};
      return traits_type::info( instance );
    };

    return table;
  }();

  return &s_vtable;
}

//=============================================================================
// allocator_reference
//=============================================================================

//-----------------------------------------------------------------------------
// Constructors
//-----------------------------------------------------------------------------

template<typename Allocator, typename>
inline bit::memory::allocator_reference::allocator_reference( Allocator& allocator )
  noexcept
  : m_ptr( std::addressof(allocator) ),
    m_vtable( vtable_type::get_stateful_vtable<std::decay_t<Allocator>>() )
{

}

//-----------------------------------------------------------------------------
// Block Allocation
//-----------------------------------------------------------------------------

inline bit::memory::owner<void*> bit::memory::allocator_reference
  ::try_allocate( std::size_t size, std::size_t align )
  noexcept
{
  return m_vtable->try_allocate_fn( m_ptr, size, align );
}

inline bit::memory::owner<void*> bit::memory::allocator_reference
  ::allocate( std::size_t size, std::size_t align )
{
  return m_vtable->allocate_fn( m_ptr, size, align );
}

//-----------------------------------------------------------------------------

inline void bit::memory::allocator_reference::deallocate( owner<void*> p,
                                                    std::size_t size )
{
  m_vtable->deallocate_fn( m_ptr, p, size );
}

//-----------------------------------------------------------------------------
// Capacity
//-----------------------------------------------------------------------------

inline bit::memory::allocator_info bit::memory::allocator_reference::info()
  const noexcept
{
  return m_vtable->info_fn( m_ptr );
}

//-----------------------------------------------------------------------------
// Private Constructor
//-----------------------------------------------------------------------------

template<typename Allocator>
inline bit::memory::allocator_reference
  ::allocator_reference( stateless_type<Allocator> )
  noexcept
  : m_ptr( nullptr ),
    m_vtable( vtable_type::get_stateless_vtable<std::decay_t<Allocator>>() )
{

}

//-----------------------------------------------------------------------------
// Equality Comparison
//-----------------------------------------------------------------------------

inline bool bit::memory::operator==( const allocator_reference& lhs,
                                     const allocator_reference& rhs )
  noexcept
{
  return lhs.m_vtable == rhs.m_vtable &&
         lhs.m_ptr == rhs.m_ptr;
}

inline bool bit::memory::operator!=( const allocator_reference& lhs,
                                     const allocator_reference& rhs )
  noexcept
{
  return !(lhs==rhs);
}

//-----------------------------------------------------------------------------
// Utility
//-----------------------------------------------------------------------------

template<typename StatelessAllocator>
inline std::enable_if_t<bit::memory::is_stateless<StatelessAllocator>::value &&
                        bit::memory::is_allocator<StatelessAllocator>::value,
                        bit::memory::allocator_reference>
  bit::memory::make_stateless_allocator_reference()
  noexcept
{
  using tag_type = allocator_reference::stateless_type<StatelessAllocator>;

  return allocator_reference{ tag_type{} };
}

#endif /* BIT_MEMORY_ALLOCATORS_DETAIL_ALLOCATOR_REFERENCE_INL */
