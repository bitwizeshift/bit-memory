#ifndef BIT_MEMORY_ALLOCATORS_DETAIL_ANY_ALLOCATOR_INL
#define BIT_MEMORY_ALLOCATORS_DETAIL_ANY_ALLOCATOR_INL

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
  static allocator_vtable* get_vtable();
};

//-----------------------------------------------------------------------------
// Static Creation
//-----------------------------------------------------------------------------

template<typename Allocator>
inline bit::memory::detail::allocator_vtable*
  bit::memory::detail::allocator_vtable::get_vtable()
{
  using traits_type = allocator_traits<Allocator>;

  static auto s_vtable = []()
  {
    allocator_vtable table;

    table.allocate_fn = +[](void* p, std::size_t size, std::size_t align )
    {
      auto* instance = static_cast<Allocator*>(p);
      return traits_type::allocate( *instance, size, align );
    };

    table.try_allocate_fn = +[](void* p, std::size_t size, std::size_t align )
    {
      auto* instance = static_cast<Allocator*>(p);
      return traits_type::try_allocate( *instance, size, align );
    };

    table.deallocate_fn = +[](void* p, void* ptr, std::size_t size )
    {
      auto* instance = static_cast<Allocator*>(p);
      return traits_type::deallocate( *instance, ptr, size );
    };

    table.info_fn = +[](const void* p) -> allocator_info
    {
      auto* instance = static_cast<const Allocator*>(p);
      return traits_type::info( *instance );
    };

    return table;
  }();

  return &s_vtable;
}

//=============================================================================
// any_allocator
//=============================================================================

//-----------------------------------------------------------------------------
// Constructors
//-----------------------------------------------------------------------------

template<typename Allocator, typename>
inline bit::memory::any_allocator::any_allocator( Allocator& any_allocator )
  noexcept
  : m_ptr( std::addressof(any_allocator) ),
    m_vtable( vtable_type::get_vtable<std::decay_t<Allocator>>() )
{
  using traits_type = allocator_traits<std::decay_t<Allocator>>;
}

//-----------------------------------------------------------------------------
// Block Allocation
//-----------------------------------------------------------------------------

inline bit::memory::owner<void*> bit::memory::any_allocator
  ::allocate( std::size_t size, std::size_t align )
{
  return m_vtable->allocate_fn( m_ptr, size, align );
}

inline bit::memory::owner<void*> bit::memory::any_allocator
  ::try_allocate( std::size_t size, std::size_t align )
  noexcept
{
  return m_vtable->try_allocate_fn( m_ptr, size, align );
}

//-----------------------------------------------------------------------------

inline void bit::memory::any_allocator::deallocate( owner<void*> p,
                                                    std::size_t size )
{
  m_vtable->deallocate_fn( m_ptr, p, size );
}

//-----------------------------------------------------------------------------
// Capacity
//-----------------------------------------------------------------------------

inline bit::memory::allocator_info bit::memory::any_allocator::info()
  const noexcept
{
  return m_vtable->info_fn( m_ptr );
}

//-----------------------------------------------------------------------------
// Equality Comparison
//-----------------------------------------------------------------------------

inline bool bit::memory::operator==( const any_allocator& lhs,
                                     const any_allocator& rhs )
  noexcept
{
  return lhs.m_vtable == rhs.m_vtable &&
         lhs.m_ptr == rhs.m_ptr;
}

inline bool bit::memory::operator!=( const any_allocator& lhs,
                                     const any_allocator& rhs )
  noexcept
{
  return !(lhs==rhs);
}

#endif /* BIT_MEMORY_ALLOCATORS_DETAIL_ANY_ALLOCATOR_INL */
