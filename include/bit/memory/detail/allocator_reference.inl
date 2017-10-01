#ifndef BIT_MEMORY_DETAIL_ALLOCATOR_REFERENCE_INL
#define BIT_MEMORY_DETAIL_ALLOCATOR_REFERENCE_INL

//=============================================================================
// allocator_reference_base<Allocator,true>
//=============================================================================

//-----------------------------------------------------------------------------
// Element Access
//-----------------------------------------------------------------------------

template<typename Allocator>
inline bit::memory::detail::allocator_reference_base<Allocator,true>
  ::operator Allocator&()
  const noexcept
{
  return *static_cast<Allocator*>(this);
}

template<typename Allocator>
inline Allocator& bit::memory::detail::allocator_reference_base<Allocator,true>
  ::get()
  const noexcept
{
  return *static_cast<Allocator*>(this);
}

//=============================================================================
// allocator_reference_base<Allocator,false>
//=============================================================================

//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------

template<typename Allocator>
inline bit::memory::detail::allocator_reference_base<Allocator,false>
  ::allocator_reference_base( Allocator& allocator )
  noexcept
  : m_allocator( std::addressof(allocator) )
{

}

//-----------------------------------------------------------------------------
// Element Access
//-----------------------------------------------------------------------------

template<typename Allocator>
inline bit::memory::detail::allocator_reference_base<Allocator,false>
  ::operator Allocator&()
  const noexcept
{
  return *m_allocator;
}

template<typename Allocator>
inline Allocator& bit::memory::detail::allocator_reference_base<Allocator,false>
  ::get()
  const noexcept
{
  return *m_allocator;
}

//=============================================================================
// allocator_reference
//=============================================================================

//-----------------------------------------------------------------------------
// Comparison
//-----------------------------------------------------------------------------

template<typename A>
inline bool bit::memory::operator==( allocator_reference<A>& lhs,
                                     allocator_reference<A>& rhs )
  noexcept
{
  return std::addressof(lhs.get()) == std::addressof(rhs.get());
}

template<typename A>
inline bool bit::memory::operator!=( allocator_reference<A>& lhs,
                                     allocator_reference<A>& rhs )
  noexcept
{
  return std::addressof(lhs.get()) != std::addressof(rhs.get());
}

template<typename A>
inline bool bit::memory::operator<( allocator_reference<A>& lhs,
                                    allocator_reference<A>& rhs )
  noexcept
{
  return std::addressof(lhs.get()) < std::addressof(rhs.get());
}

template<typename A>
inline bool bit::memory::operator>( allocator_reference<A>& lhs,
                                    allocator_reference<A>& rhs )
  noexcept
{
  return std::addressof(lhs.get()) > std::addressof(rhs.get());
}

template<typename A>
inline bool bit::memory::operator<=( allocator_reference<A>& lhs,
                                     allocator_reference<A>& rhs )
  noexcept
{
  return std::addressof(lhs.get()) <= std::addressof(rhs.get());
}

template<typename A>
inline bool bit::memory::operator>=( allocator_reference<A>& lhs,
                                     allocator_reference<A>& rhs )
  noexcept
{
  return std::addressof(lhs.get()) >= std::addressof(rhs.get());
}

//-----------------------------------------------------------------------------
// Utilities
//-----------------------------------------------------------------------------

template<typename Allocator>
inline bit::memory::allocator_reference<Allocator>
  bit::memory::make_allocator_reference( Allocator& allocator )
  noexcept
{
  return allocator_reference<Allocator>{ allocator };
}

#endif /* BIT_MEMORY_DETAIL_ALLOCATOR_REFERENCE_INL */
