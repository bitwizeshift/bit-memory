#ifndef BIT_MEMORY_DETAIL_SMART_PTR_OBSERVER_PTR_INL
#define BIT_MEMORY_DETAIL_SMART_PTR_OBSERVER_PTR_INL

//----------------------------------------------------------------------------
// Constructors / Assignment
//----------------------------------------------------------------------------

template<typename T>
inline constexpr bit::memory::observer_ptr<T>::observer_ptr()
  noexcept
  : observer_ptr(nullptr)
{

}

template<typename T>
inline constexpr bit::memory::observer_ptr<T>::observer_ptr( std::nullptr_t )
  noexcept
  : m_ptr(nullptr)
{

}

template<typename T>
inline constexpr bit::memory::observer_ptr<T>::observer_ptr( pointer ptr )
  noexcept
  : m_ptr(ptr)
{

}

template<typename T>
template<typename U, std::enable_if<std::is_convertible<U*,T*>::value>*>
inline constexpr bit::memory::observer_ptr<T>::observer_ptr( const observer_ptr<U>& other )
  noexcept
  : observer_ptr( other.get() )
{

}

template<typename T>
template<typename U, std::enable_if<std::is_convertible<U*,T*>::value>*>
inline bit::memory::observer_ptr<T>::observer_ptr( observer_ptr<U>&& other )
  noexcept
  : observer_ptr( other.get() )
{
  other.reset();
}

//----------------------------------------------------------------------------

template<typename T>
inline bit::memory::observer_ptr<T>&
  bit::memory::observer_ptr<T>::operator=( std::nullptr_t )
  noexcept
{
  m_ptr = nullptr;

  return (*this);
}

//----------------------------------------------------------------------------
// Modifiers
//----------------------------------------------------------------------------

template<typename T>
inline void bit::memory::observer_ptr<T>::reset( pointer ptr )
  noexcept
{
  m_ptr = ptr;
}

template<typename T>
inline void bit::memory::observer_ptr<T>::reset( std::nullptr_t )
  noexcept
{
  m_ptr = nullptr;
}

template<typename T>
inline void bit::memory::observer_ptr<T>::swap( observer_ptr& other )
  noexcept
{
  using std::swap;
  swap(m_ptr, other.m_ptr);
}

//----------------------------------------------------------------------------
// Observers
//----------------------------------------------------------------------------

template<typename T>
inline constexpr typename bit::memory::observer_ptr<T>::pointer
  bit::memory::observer_ptr<T>::get()
  const noexcept
{
  return m_ptr;
}

template<typename T>
inline constexpr bit::memory::observer_ptr<T>::operator bool()
  const noexcept
{
  return (bool) m_ptr;
}

template<typename T>
inline constexpr typename bit::memory::observer_ptr<T>::element_type*
  bit::memory::observer_ptr<T>::operator->()
  const noexcept
{
  return m_ptr;
}

template<typename T>
inline constexpr typename bit::memory::observer_ptr<T>::element_type&
  bit::memory::observer_ptr<T>::operator*()
  const noexcept
{
  return *m_ptr;
}

//----------------------------------------------------------------------------
// Conversions
//----------------------------------------------------------------------------

template<typename T>
inline constexpr bit::memory::observer_ptr<T>::operator element_type*()
  const noexcept
{
  return m_ptr;
}


//----------------------------------------------------------------------------
// Free Functions
//----------------------------------------------------------------------------

template<typename T>
inline void bit::memory::swap( observer_ptr<T>& lhs, observer_ptr<T>& rhs )
  noexcept
{
  lhs.swap(rhs);
}



//----------------------------------------------------------------------------
// Comparisons
//----------------------------------------------------------------------------

template<typename T, typename U>
inline constexpr bool bit::memory::operator==( const observer_ptr<T>& lhs,
                                               const observer_ptr<U>& rhs )
  noexcept
{
  return lhs.get() == rhs.get();
}

template<typename T, typename U>
inline constexpr bool bit::memory::operator!=( const observer_ptr<T>& lhs,
                                               const observer_ptr<U>& rhs )
  noexcept
{
  return lhs.get() == rhs.get();
}

template<typename T, typename U>
inline constexpr bool bit::memory::operator<( const observer_ptr<T>& lhs,
                                              const observer_ptr<U>& rhs )
  noexcept
{
  return lhs.get() < rhs.get();
}

template<typename T, typename U>
inline constexpr bool bit::memory::operator>( const observer_ptr<T>& lhs,
                                              const observer_ptr<U>& rhs )
  noexcept
{
  return lhs.get() > rhs.get();
}

template<typename T, typename U>
inline constexpr bool bit::memory::operator<=( const observer_ptr<T>& lhs,
                                               const observer_ptr<U>& rhs )
  noexcept
{
  return lhs.get() <= rhs.get();
}

template<typename T, typename U>
inline constexpr bool bit::memory::operator>=( const observer_ptr<T>& lhs,
                                               const observer_ptr<U>& rhs )
  noexcept
{
  return lhs.get() >= rhs.get();
}

//----------------------------------------------------------------------------

template<typename T>
inline constexpr bool bit::memory::operator==( std::nullptr_t,
                                               const observer_ptr<T>& rhs )
  noexcept
{
  return nullptr == rhs.get();
}

template<typename T>
inline constexpr bool bit::memory::operator==( const observer_ptr<T>& lhs,
                                               std::nullptr_t )
  noexcept
{
  return lhs.get() == nullptr;
}

//----------------------------------------------------------------------------

template<typename T>
inline constexpr bool bit::memory::operator!=( std::nullptr_t,
                                               const observer_ptr<T>& rhs )
  noexcept
{
  return nullptr != rhs.get();
}

template<typename T>
inline constexpr bool bit::memory::operator!=( const observer_ptr<T>& lhs,
                                               std::nullptr_t )
  noexcept
{
  return lhs.get() != nullptr;
}

//----------------------------------------------------------------------------

template<typename T>
inline constexpr bool bit::memory::operator<( std::nullptr_t,
                                              const observer_ptr<T>& rhs )
  noexcept
{
  return nullptr < rhs.get();
}

template<typename T>
inline constexpr bool bit::memory::operator<( const observer_ptr<T>& lhs,
                                              std::nullptr_t )
  noexcept
{
  return lhs.get() < nullptr;
}

//----------------------------------------------------------------------------

template<typename T>
inline constexpr bool bit::memory::operator>( std::nullptr_t,
                                              const observer_ptr<T>& rhs )
  noexcept
{
  return nullptr > rhs.get();
}

template<typename T>
inline constexpr bool bit::memory::operator>( const observer_ptr<T>& lhs,
                                              std::nullptr_t )
  noexcept
{
  return lhs.get() > nullptr;
}

//----------------------------------------------------------------------------

template<typename T>
inline constexpr bool bit::memory::operator<=( std::nullptr_t,
                                               const observer_ptr<T>& rhs )
  noexcept
{
  return nullptr <= rhs.get();
}

template<typename T>
inline constexpr bool bit::memory::operator<=( const observer_ptr<T>& lhs,
                                               std::nullptr_t )
  noexcept
{
  return lhs.get() <= nullptr;
}

//----------------------------------------------------------------------------

template<typename T>
inline constexpr bool bit::memory::operator>=( std::nullptr_t,
                                               const observer_ptr<T>& rhs )
  noexcept
{
  return nullptr >= rhs.get();
}

template<typename T>
inline constexpr bool bit::memory::operator>=( const observer_ptr<T>& lhs,
                                               std::nullptr_t )
  noexcept
{
  return lhs.get() >= nullptr;
}

#endif /* BIT_MEMORY_DETAIL_SMART_PTR_OBSERVER_PTR_INL */
