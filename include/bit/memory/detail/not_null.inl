#ifndef BIT_MEMORY_DETAIL_NOT_NULL_INL
#define BIT_MEMORY_DETAIL_NOT_NULL_INL

//----------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------

template<typename Ptr>
template <typename U, std::enable_if_t<std::is_convertible<U, Ptr>::value>*>
inline constexpr bit::memory::not_null<Ptr>::not_null(U&& u)
  : m_pointer(std::forward<U>(u))
{
  assert(m_pointer != nullptr);
}

template<typename Ptr>
template <typename U, std::enable_if_t<std::is_convertible<U, Ptr>::value>*>
inline constexpr bit::memory::not_null<Ptr>::not_null(const not_null<U>& other)
  : not_null(other.get())
{

}

//----------------------------------------------------------------------------
// Observers
//----------------------------------------------------------------------------

template<typename Ptr>
inline constexpr typename bit::memory::not_null<Ptr>::pointer
  bit::memory::not_null<Ptr>::get()
  const noexcept
{
  assert(m_pointer != nullptr);
  return m_pointer;
}

template<typename Ptr>
inline constexpr bit::memory::not_null<Ptr>::operator Ptr()
  const noexcept
{
  return get();
}

template<typename Ptr>
inline constexpr typename bit::memory::not_null<Ptr>::pointer
  bit::memory::not_null<Ptr>::operator->()
  const noexcept
{
  return get();
}

template<typename Ptr>
inline constexpr typename bit::memory::not_null<Ptr>::reference
  bit::memory::not_null<Ptr>::operator*()
  const noexcept
{
  return *get();
}

template<typename Ptr>
inline constexpr bit::memory::not_null<Ptr>
  bit::memory::make_not_null(Ptr&& ptr)
{
  return not_null<Ptr>( std::forward<Ptr>(ptr) );
}


#endif /* BIT_MEMORY_DETAIL_NOT_NULL_INL */
