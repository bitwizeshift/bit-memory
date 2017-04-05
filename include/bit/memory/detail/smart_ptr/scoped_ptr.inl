#ifndef BIT_MEMORY_DETAIL_SMART_PTR_SCOPED_PTR_INL
#define BIT_MEMORY_DETAIL_SMART_PTR_SCOPED_PTR_INL

//============================================================================
// scoped_ptr<T>
//============================================================================

//----------------------------------------------------------------------------
// Constructors
//----------------------------------------------------------------------------

template<typename T, typename Deleter>
inline constexpr bit::memory::scoped_ptr<T,Deleter>::scoped_ptr()
  noexcept
  : scoped_ptr(nullptr)
{

}

template<typename T, typename Deleter>
inline constexpr bit::memory::scoped_ptr<T,Deleter>
  ::scoped_ptr( std::nullptr_t )
  noexcept
  : m_storage(nullptr)
{

}

template<typename T, typename Deleter>
inline constexpr bit::memory::scoped_ptr<T,Deleter>::scoped_ptr( pointer ptr )
  noexcept
  : m_storage(ptr)
{

}

template<typename T, typename Deleter>
inline constexpr bit::memory::scoped_ptr<T,Deleter>
  ::scoped_ptr( pointer ptr, const Deleter& deleter )
  noexcept
  : m_storage(ptr, deleter)
{

}

template<typename T, typename Deleter>
inline constexpr bit::memory::scoped_ptr<T,Deleter>
  ::scoped_ptr( pointer ptr, Deleter&& deleter )
  noexcept
  : m_storage(ptr, std::move(deleter))
{

}

//----------------------------------------------------------------------------

template<typename T, typename Deleter>
inline bit::memory::scoped_ptr<T,Deleter>::~scoped_ptr()
{
  reset();
}

//----------------------------------------------------------------------------

template<typename T, typename Deleter>
inline bit::memory::scoped_ptr<T,Deleter>&
  bit::memory::scoped_ptr<T,Deleter>::operator=( std::nullptr_t )
{
  reset();

  return (*this);
}

//----------------------------------------------------------------------------
// Modifiers
//----------------------------------------------------------------------------

template<typename T, typename Deleter>
inline void bit::memory::scoped_ptr<T,Deleter>::reset( pointer ptr )
{
  if( m_storage.ptr ) {
    m_storage( m_storage.ptr );
  }
  m_storage.ptr = ptr;
}

template<typename T, typename Deleter>
inline void bit::memory::scoped_ptr<T,Deleter>::reset( std::nullptr_t )
{
  if( m_storage.ptr ) {
    m_storage( m_storage.ptr );
  }
  m_storage.ptr = nullptr;
}

template<typename T, typename Deleter>
inline void bit::memory::scoped_ptr<T,Deleter>::swap( scoped_ptr& other )
  noexcept
{
  using std::swap;

  swap(m_storage, other.m_storage);
}

template<typename T, typename Deleter>
inline typename bit::memory::scoped_ptr<T,Deleter>::pointer
  bit::memory::scoped_ptr<T,Deleter>::release()
  noexcept
{
  auto ptr = m_storage.ptr;
  m_storage.ptr = nullptr;
  return ptr;
}

//----------------------------------------------------------------------------
// Modifiers
//----------------------------------------------------------------------------

template<typename T, typename Deleter>
inline constexpr typename bit::memory::scoped_ptr<T,Deleter>::pointer
  bit::memory::scoped_ptr<T,Deleter>::get()
  const noexcept
{
  return m_storage.ptr;
}

template<typename T, typename Deleter>
inline constexpr bit::memory::scoped_ptr<T,Deleter>::operator bool()
  const noexcept
{
  return m_storage.ptr;
}

template<typename T, typename Deleter>
inline constexpr typename bit::memory::scoped_ptr<T,Deleter>::element_type*
  bit::memory::scoped_ptr<T,Deleter>::operator->()
  const noexcept
{
  return m_storage.ptr;
}

template<typename T, typename Deleter>
constexpr typename bit::memory::scoped_ptr<T,Deleter>::element_type&
  bit::memory::scoped_ptr<T,Deleter>::operator*()
  const noexcept
{
  return *m_storage.ptr;
}

template<typename T, typename Deleter>
Deleter& bit::memory::scoped_ptr<T,Deleter>::get_deleter()
  noexcept
{
  return m_storage;
}

template<typename T, typename Deleter>
const Deleter& bit::memory::scoped_ptr<T,Deleter>::get_deleter()
  const noexcept
{
  return m_storage;
}

//----------------------------------------------------------------------------
// Private : Constructors
//----------------------------------------------------------------------------

template<typename T, typename Deleter>
template<typename U>
inline bit::memory::scoped_ptr<T,Deleter>
  ::scoped_ptr( scoped_ptr<U,Deleter>&& other )
  noexcept
  : m_storage( other.release(), other.get_deleter() )
{

}

//============================================================================
// scoped_ptr<T[]>
//============================================================================

//----------------------------------------------------------------------------
// Constructors
//----------------------------------------------------------------------------

template<typename T, typename Deleter>
inline constexpr bit::memory::scoped_ptr<T[],Deleter>::scoped_ptr()
  noexcept
  : scoped_ptr(nullptr)
{

}

template<typename T, typename Deleter>
inline constexpr bit::memory::scoped_ptr<T[],Deleter>
  ::scoped_ptr( std::nullptr_t )
  noexcept
  : m_storage(nullptr)
{

}

template<typename T, typename Deleter>
inline constexpr bit::memory::scoped_ptr<T[],Deleter>::scoped_ptr( pointer ptr )
  noexcept
  : m_storage(ptr)
{

}

template<typename T, typename Deleter>
inline constexpr bit::memory::scoped_ptr<T[],Deleter>
  ::scoped_ptr( pointer ptr, const Deleter& deleter )
  noexcept
  : m_storage(ptr, deleter)
{

}

template<typename T, typename Deleter>
inline constexpr bit::memory::scoped_ptr<T[],Deleter>
  ::scoped_ptr( pointer ptr, Deleter&& deleter )
  noexcept
  : m_storage(ptr, std::move(deleter))
{

}

//----------------------------------------------------------------------------

template<typename T, typename Deleter>
inline bit::memory::scoped_ptr<T[],Deleter>::~scoped_ptr()
{
  reset();
}

//----------------------------------------------------------------------------

template<typename T, typename Deleter>
inline bit::memory::scoped_ptr<T[],Deleter>&
  bit::memory::scoped_ptr<T[],Deleter>::operator=( std::nullptr_t )
{
  reset();

  return (*this);
}

//----------------------------------------------------------------------------
// Modifiers
//----------------------------------------------------------------------------


template<typename T, typename Deleter>
inline void bit::memory::scoped_ptr<T[],Deleter>::reset( pointer ptr )
{
  if( m_storage.ptr ) {
    m_storage( m_storage.ptr );
  }
  m_storage.ptr = ptr;
}

template<typename T, typename Deleter>
inline void bit::memory::scoped_ptr<T[],Deleter>::reset( std::nullptr_t )
{
  if( m_storage.ptr ) {
    m_storage( m_storage.ptr );
  }
  m_storage.ptr = nullptr;
}

template<typename T, typename Deleter>
inline void bit::memory::scoped_ptr<T[],Deleter>::swap( scoped_ptr& other )
  noexcept
{
  using std::swap;

  swap(m_storage, other.m_storage);
}

template<typename T, typename Deleter>
inline typename bit::memory::scoped_ptr<T[],Deleter>::pointer
  bit::memory::scoped_ptr<T[],Deleter>::release()
  noexcept
{
  auto ptr = m_storage.ptr;
  m_storage.ptr = nullptr;
  return ptr;
}

//----------------------------------------------------------------------------
// Observers
//----------------------------------------------------------------------------

template<typename T, typename Deleter>
inline constexpr typename bit::memory::scoped_ptr<T[],Deleter>::pointer
  bit::memory::scoped_ptr<T[],Deleter>::get()
  const noexcept
{
  return m_storage.ptr;
}

template<typename T, typename Deleter>
inline constexpr bit::memory::scoped_ptr<T[],Deleter>::operator bool()
  const noexcept
{
  return m_storage.ptr;
}

template<typename T, typename Deleter>
inline constexpr typename bit::memory::scoped_ptr<T[],Deleter>::element_type&
  bit::memory::scoped_ptr<T[],Deleter>::operator[]( std::ptrdiff_t n )
  const noexcept
{
  return m_storage.ptr[n];
}

template<typename T, typename Deleter>
Deleter& bit::memory::scoped_ptr<T[],Deleter>::get_deleter()
  noexcept
{
  return m_storage;
}

template<typename T, typename Deleter>
const Deleter& bit::memory::scoped_ptr<T[],Deleter>::get_deleter()
  const noexcept
{
  return m_storage;
}

//----------------------------------------------------------------------------
// Private : Constructors
//----------------------------------------------------------------------------

template<typename T, typename Deleter>
inline bit::memory::scoped_ptr<T[],Deleter>
  ::scoped_ptr( scoped_ptr&& other )
  noexcept
  : m_storage( other.release(), other.get_deleter() )
{

}

//----------------------------------------------------------------------------
// Utilities
//----------------------------------------------------------------------------

template<typename T>
struct bit::memory::detail::make_scoped_dispatch
{
  template<typename...Args>
  static scoped_ptr<T> make( Args&&...args )
  {
    return scoped_ptr<T>( new T( std::forward<Args>(args)... ) );
  }
};

template<typename T>
struct bit::memory::detail::make_scoped_dispatch<T[]>
{
  static scoped_ptr<T[]> make( std::size_t size )
  {
    return scoped_ptr<T[]>( new T[ size ] );
  }
};

template<typename T, typename...Args>
bit::memory::scoped_ptr<T> bit::memory::make_scoped( Args&&...args )
{
  return detail::make_scoped_dispatch<T>::make( std::forward<Args>(args)... );
}

template<typename T, typename Deleter>
void bit::memory::swap( scoped_ptr<T,Deleter>& lhs, scoped_ptr<T,Deleter>& rhs )
  noexcept
{
  lhs.swap(rhs);
}

//----------------------------------------------------------------------------
// Comparisons
//----------------------------------------------------------------------------

template<typename T, typename U>
inline constexpr bool bit::memory::operator==( const scoped_ptr<T>& lhs,
                                               const scoped_ptr<U>& rhs )
  noexcept
{
  return lhs.get() == rhs.get();
}

template<typename T, typename U>
inline constexpr bool bit::memory::operator!=( const scoped_ptr<T>& lhs,
                                               const scoped_ptr<U>& rhs )
  noexcept
{
  return lhs.get() == rhs.get();
}

template<typename T, typename U>
inline constexpr bool bit::memory::operator<( const scoped_ptr<T>& lhs,
                                              const scoped_ptr<U>& rhs )
  noexcept
{
  return lhs.get() < rhs.get();
}

template<typename T, typename U>
inline constexpr bool bit::memory::operator>( const scoped_ptr<T>& lhs,
                                              const scoped_ptr<U>& rhs )
  noexcept
{
  return lhs.get() > rhs.get();
}

template<typename T, typename U>
inline constexpr bool bit::memory::operator<=( const scoped_ptr<T>& lhs,
                                               const scoped_ptr<U>& rhs )
  noexcept
{
  return lhs.get() <= rhs.get();
}

template<typename T, typename U>
inline constexpr bool bit::memory::operator>=( const scoped_ptr<T>& lhs,
                                               const scoped_ptr<U>& rhs )
  noexcept
{
  return lhs.get() >= rhs.get();
}

//----------------------------------------------------------------------------

template<typename T>
inline constexpr bool bit::memory::operator==( std::nullptr_t,
                                               const scoped_ptr<T>& rhs )
  noexcept
{
  return nullptr == rhs.get();
}

template<typename T>
inline constexpr bool bit::memory::operator==( const scoped_ptr<T>& lhs,
                                               std::nullptr_t )
  noexcept
{
  return lhs.get() == nullptr;
}

//----------------------------------------------------------------------------

template<typename T>
inline constexpr bool bit::memory::operator!=( std::nullptr_t,
                                               const scoped_ptr<T>& rhs )
  noexcept
{
  return nullptr != rhs.get();
}

template<typename T>
inline constexpr bool bit::memory::operator!=( const scoped_ptr<T>& lhs,
                                               std::nullptr_t )
  noexcept
{
  return lhs.get() != nullptr;
}

//----------------------------------------------------------------------------

template<typename T>
inline constexpr bool bit::memory::operator<( std::nullptr_t,
                                              const scoped_ptr<T>& rhs )
  noexcept
{
  return nullptr < rhs.get();
}

template<typename T>
inline constexpr bool bit::memory::operator<( const scoped_ptr<T>& lhs,
                                              std::nullptr_t )
  noexcept
{
  return lhs.get() < nullptr;
}

//----------------------------------------------------------------------------

template<typename T>
inline constexpr bool bit::memory::operator>( std::nullptr_t,
                                              const scoped_ptr<T>& rhs )
  noexcept
{
  return nullptr > rhs.get();
}

template<typename T>
inline constexpr bool bit::memory::operator>( const scoped_ptr<T>& lhs,
                                              std::nullptr_t )
  noexcept
{
  return lhs.get() > nullptr;
}

//----------------------------------------------------------------------------

template<typename T>
inline constexpr bool bit::memory::operator<=( std::nullptr_t,
                                               const scoped_ptr<T>& rhs )
  noexcept
{
  return nullptr <= rhs.get();
}

template<typename T>
inline constexpr bool bit::memory::operator<=( const scoped_ptr<T>& lhs,
                                               std::nullptr_t )
  noexcept
{
  return lhs.get() <= nullptr;
}

//----------------------------------------------------------------------------

template<typename T>
inline constexpr bool bit::memory::operator>=( std::nullptr_t,
                                               const scoped_ptr<T>& rhs )
  noexcept
{
  return nullptr >= rhs.get();
}

template<typename T>
inline constexpr bool bit::memory::operator>=( const scoped_ptr<T>& lhs,
                                               std::nullptr_t )
  noexcept
{
  return lhs.get() >= nullptr;
}

#endif /* BIT_MEMORY_DETAIL_SMART_PTR_SCOPED_PTR_INL */
