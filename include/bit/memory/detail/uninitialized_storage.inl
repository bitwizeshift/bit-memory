#ifndef BIT_MEMORY_DETAIL_UNINITIALIZED_STORAGE_INL
#define BIT_MEMORY_DETAIL_UNINITIALIZED_STORAGE_INL

//----------------------------------------------------------------------------
// Construction
//----------------------------------------------------------------------------

template<typename T, typename...Args>
inline T* bit::memory::uninitialized_construct_at( void* p, Args&&...args )
{
  return new (p) T( std::forward<Args>(args)... );
}

namespace bit { namespace memory { namespace detail {

  template<typename T, typename Tuple, std::size_t...Idxs>
  inline T* uninitialized_construct_from_tuple( void* p, Tuple&& tuple, std::index_sequence<Idxs...> )
  {
    return new (p) T( std::get<Idxs>(std::forward<Tuple>(tuple))... );
  }

} } } // namespace bit::memory::detail


template<typename T, typename Tuple>
inline T* bit::memory::uninitialized_construct_from_tuple( void* p, Tuple&& tuple )
{
  const auto seq = std::make_index_sequence<std::tuple_size<std::decay_t<Tuple>>::value>{};

  return detail::uninitialized_construct_from_tuple<T>( p, std::forward<Tuple>(tuple), seq );
}

template<typename T>
inline T* bit::memory::uninitialized_construct_array_at( void* p,
                                                         std::size_t n )
{
  auto current   = static_cast<T*>(p);
  const auto end = current + n;

  try {
    while( current != end ) {
      uninitialized_construct_at(current++);
    }
  } catch ( ... ) {
    const auto rend = static_cast<T*>(p)--;
    while( current != rend ) {
      destroy_at(current--);
    }
    throw;
  }
  return static_cast<T*>(p);
}

template<typename T>
inline T* bit::memory::uninitialized_construct_array_at( void* p,
                                                         std::size_t n,
                                                         const T& copy )
{
  auto current   = static_cast<T*>(p);
  const auto end = current + n;

  try {
    while( current != end ) {
      uninitialized_construct_at(current++, copy);
    }
  } catch ( ... ) {
    const auto rend = static_cast<T*>(p) - 1;
    while( current != rend ) {
      destroy_at(current--);
    }
    throw;
  }
  return static_cast<T*>(p);
}

namespace bit { namespace memory { namespace detail {

  template<typename T, typename Tuple, std::size_t...Idxs>
  inline T make_from_tuple( Tuple&& tuple, std::index_sequence<Idxs...> )
  {
    return T( std::get<Idxs>(std::forward<Tuple>(tuple))... );
  }

} } } // namespace bit::memory::detail

template<typename T, typename Tuple>
inline T bit::memory::make_from_tuple( Tuple&& tuple )
{
  const auto seq = std::make_index_sequence<std::tuple_size<std::decay_t<Tuple>>::value>{};

  return detail::make_from_tuple<T>( std::forward<Tuple>(tuple), seq );
}

//----------------------------------------------------------------------------
// Destruction
//----------------------------------------------------------------------------

template<typename T>
inline void bit::memory::destroy_at( T* p )
{
  p->~T();
}

template<typename T>
inline void bit::memory::destroy_array_at( T* p, std::size_t n )
{
  const auto end = static_cast<T*>(p);
  auto current   = static_cast<T*>(p) + n;

  while( current != end ) {
    destroy_at(--current);
  }
}

#endif /* BIT_MEMORY_DETAIL_UNINITIALIZED_STORAGE_INL */
