#ifndef BIT_MEMORY_UTILITIES_DETAIL_UNINITIALIZED_STORAGE_INL
#define BIT_MEMORY_UTILITIES_DETAIL_UNINITIALIZED_STORAGE_INL

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
  inline T* uninitialized_construct_from_tuple( void* p,
                                                Tuple&& tuple,
                                                std::index_sequence<Idxs...> )
  {
    return new (p) T( std::get<Idxs>( std::forward<Tuple>(tuple) )... );
  }

} } } // namespace bit::memory::detail


template<typename T, typename Tuple>
inline T* bit::memory::uninitialized_construct_from_tuple( void* p, Tuple&& tuple )
{
  using decay_type = std::decay_t<Tuple>;
  static constexpr auto tuple_size = std::tuple_size<decay_type>::value;
  const auto seq = std::make_index_sequence<tuple_size>{};

  return detail::uninitialized_construct_from_tuple<T>( p, std::forward<Tuple>(tuple), seq );
}

namespace bit { namespace memory { namespace detail {
  template<typename T, typename...Args>
  inline T* uninitialized_construct_array_at( std::true_type,
                                              void* p,
                                              std::size_t n,
                                              Args&&...args )
  {
    auto current   = static_cast<T*>(p);
    const auto end = current + n;

    while( current != end ) {
      uninitialized_construct_at( current++, std::forward<Args>(args)... );
    }

    return static_cast<T*>(p);
  }

  template<typename T, typename...Args>
  inline T* uninitialized_construct_array_at( std::false_type,
                                              void* p,
                                              std::size_t n,
                                              Args&&...args )
  {
    auto current   = static_cast<T*>(p);
    const auto end = current + n;

    try {
      while( current != end ) {
        uninitialized_construct_at( current, std::forward<Args>(args)... );
        ++current; // done as separate statement in case construction throws
      }
    } catch ( ... ) {
      const auto rend = static_cast<T*>(p) - 1;
      while( --current != rend ) {
        destroy_at(current);
      }
      throw;
    }
    return static_cast<T*>(p);
  }

} } } // namespace bit::memory::detail

template<typename T>
inline T* bit::memory::uninitialized_construct_array_at( void* p,
                                                         std::size_t n )
{
  static const auto tag = std::is_nothrow_constructible<T>{};

  return detail::uninitialized_construct_array_at<T>( tag, p, n );
}

template<typename T>
inline T* bit::memory::uninitialized_construct_array_at( void* p,
                                                         std::size_t n,
                                                         const T& copy )
{
  static const auto tag = std::is_nothrow_copy_constructible<T>{};

  return detail::uninitialized_construct_array_at<T>( tag, p, n );
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

#endif /* BIT_MEMORY_UTILITIES_DETAIL_UNINITIALIZED_STORAGE_INL */
