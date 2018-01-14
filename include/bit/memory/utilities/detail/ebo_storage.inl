#ifndef BIT_MEMORY_UTILITIES_DETAIL_EBO_STORAGE_INL
#define BIT_MEMORY_UTILITIES_DETAIL_EBO_STORAGE_INL

//-----------------------------------------------------------------------------
// Utilities
//-----------------------------------------------------------------------------

template<std::size_t Idx,typename...Types>
inline std::tuple_element_t<Idx,std::tuple<Types...>>&
  bit::memory::get( ebo_storage<Types...>& ebo )
{
  return ebo.get( std::integral_constant<std::size_t,Idx>{} );
}

template<std::size_t Idx,typename...Types>
inline std::tuple_element_t<Idx,const std::tuple<Types...>>&
  bit::memory::get( const ebo_storage<Types...>& ebo )
{
  return ebo.get( std::integral_constant<std::size_t,Idx>{} );
}

template<std::size_t Idx,typename...Types>
inline std::tuple_element_t<Idx,std::tuple<Types...>>&&
  bit::memory::get( ebo_storage<Types...>&& ebo )
{
  return ebo.get( std::integral_constant<std::size_t,Idx>{} );
}

template<std::size_t Idx,typename...Types>
inline std::tuple_element_t<Idx,const std::tuple<Types...>>&&
  bit::memory::get( const ebo_storage<Types...>&& ebo )
{
  return ebo.get( std::integral_constant<std::size_t,Idx>{} );
}

//=============================================================================
// ebo_storage_impl<Idx,T0,true,T1,Ts...>
//=============================================================================

//-----------------------------------------------------------------------------
// Protected Constructors
//-----------------------------------------------------------------------------

template<std::size_t Idx, typename T0, typename T1, typename...Ts>
template<typename Tuple, typename...Tuples, typename>
inline bit::memory::detail::ebo_storage_impl<Idx,T0,true,T1,Ts...>
  ::ebo_storage_impl( Tuple&& tuple, Tuples&&...tuples )
  : ebo_storage_impl( std::forward<Tuple>(tuple),
                      std::make_index_sequence<std::tuple_size<std::decay_t<Tuple>>::value>{},
                      std::forward<Tuples>(tuples)... )
{

}


//-----------------------------------------------------------------------------
// Protected Observers
//-----------------------------------------------------------------------------

template<std::size_t Idx, typename T0, typename T1, typename...Ts>
inline T0& bit::memory::detail::ebo_storage_impl<Idx,T0,true,T1,Ts...>
  ::get( std::integral_constant<std::size_t,Idx> )
  &
{
  return *this;
}

template<std::size_t Idx, typename T0, typename T1, typename...Ts>
inline const T0& bit::memory::detail::ebo_storage_impl<Idx,T0,true,T1,Ts...>
  ::get( std::integral_constant<std::size_t,Idx> )
  const &
{
  return *this;
}

template<std::size_t Idx, typename T0, typename T1, typename...Ts>
inline T0&& bit::memory::detail::ebo_storage_impl<Idx,T0,true,T1,Ts...>
  ::get( std::integral_constant<std::size_t,Idx> )
  &&
{
  return static_cast<T0&&>(*this);
}

template<std::size_t Idx, typename T0, typename T1, typename...Ts>
inline const T0&& bit::memory::detail::ebo_storage_impl<Idx,T0,true,T1,Ts...>
  ::get( std::integral_constant<std::size_t,Idx> )
  const &&
{
  return static_cast<const T0&&>(*this);
}

//-----------------------------------------------------------------------------
// Private Constructors
//-----------------------------------------------------------------------------

template<std::size_t Idx, typename T0, typename T1, typename...Ts>
template<typename Tuple, std::size_t...Idxs, typename...Tuples>
inline bit::memory::detail::ebo_storage_impl<Idx,T0,true,T1,Ts...>
  ::ebo_storage_impl( Tuple&& tuple, std::index_sequence<Idxs...>, Tuples&&...tuples )
  : T0( std::get<Idxs>( std::forward<Tuple>(tuple) )... ),
    base_type( std::forward<Tuples>(tuples)... )
{

}


//=============================================================================
// ebo_storage_impl<Idx,T0,true>
//=============================================================================

//-----------------------------------------------------------------------------
// Protected Constructors
//-----------------------------------------------------------------------------

template<std::size_t Idx, typename T0>
template<typename Tuple,typename>
inline bit::memory::detail::ebo_storage_impl<Idx,T0,true>
  ::ebo_storage_impl( Tuple&& tuple )
  : ebo_storage_impl( std::forward<Tuple>(tuple),
                      std::make_index_sequence<std::tuple_size<std::decay_t<Tuple>>::value>{} )
{

}

//-----------------------------------------------------------------------------
// Protected Observers
//-----------------------------------------------------------------------------

template<std::size_t Idx, typename T0>
inline T0& bit::memory::detail::ebo_storage_impl<Idx,T0,true>
  ::get( std::integral_constant<std::size_t,Idx> )
  &
{
  return *this;
}

template<std::size_t Idx, typename T0>
inline const T0& bit::memory::detail::ebo_storage_impl<Idx,T0,true>
  ::get( std::integral_constant<std::size_t,Idx> )
  const &
{
  return *this;
}

template<std::size_t Idx, typename T0>
inline T0&& bit::memory::detail::ebo_storage_impl<Idx,T0,true>
  ::get( std::integral_constant<std::size_t,Idx> )
  &&
{
  return static_cast<T0&&>(*this);
}

template<std::size_t Idx, typename T0>
inline const T0&& bit::memory::detail::ebo_storage_impl<Idx,T0,true>
  ::get( std::integral_constant<std::size_t,Idx> )
  const &&
{
  return static_cast<const T0&&>(*this);
}

//-----------------------------------------------------------------------------
// Private Constructors
//-----------------------------------------------------------------------------

template<std::size_t Idx, typename T0>
template<typename Tuple, std::size_t...Idxs>
inline bit::memory::detail::ebo_storage_impl<Idx,T0,true>
  ::ebo_storage_impl( Tuple&& tuple, std::index_sequence<Idxs...> )
  : T0( std::get<Idxs>( std::forward<Tuple>(tuple) )... )
{

}


//=============================================================================
// ebo_storage_impl<Idx,T0,false,T1,Ts...>
//=============================================================================

//-----------------------------------------------------------------------------
// Protected Constructors
//-----------------------------------------------------------------------------

template<std::size_t Idx, typename T0, typename T1, typename...Ts>
template<typename Tuple, typename...Tuples, typename>
inline bit::memory::detail::ebo_storage_impl<Idx,T0,false,T1,Ts...>
  ::ebo_storage_impl( Tuple&& tuple, Tuples&&...tuples )
  : ebo_storage_impl( std::forward<Tuple>(tuple),
                      std::make_index_sequence<std::tuple_size<std::decay_t<Tuple>>::value>{},
                      std::forward<Tuples>(tuples)... )
{

}

//-----------------------------------------------------------------------------
// Protected Observers
//-----------------------------------------------------------------------------

template<std::size_t Idx, typename T0, typename T1, typename...Ts>
inline T0& bit::memory::detail::ebo_storage_impl<Idx,T0,false,T1,Ts...>
  ::get( std::integral_constant<std::size_t,Idx> )
   &
{
  return m_storage;
}

template<std::size_t Idx, typename T0, typename T1, typename...Ts>
inline const T0& bit::memory::detail::ebo_storage_impl<Idx,T0,false,T1,Ts...>
  ::get( std::integral_constant<std::size_t,Idx> )
  const &
{
  return m_storage;
}

template<std::size_t Idx, typename T0, typename T1, typename...Ts>
inline T0&& bit::memory::detail::ebo_storage_impl<Idx,T0,false,T1,Ts...>
  ::get( std::integral_constant<std::size_t,Idx> )
  &&
{
  return static_cast<T0&&>(m_storage);
}

template<std::size_t Idx, typename T0, typename T1, typename...Ts>
inline const T0&& bit::memory::detail::ebo_storage_impl<Idx,T0,false,T1,Ts...>
  ::get( std::integral_constant<std::size_t,Idx> )
  const &&
{
  return static_cast<const T0&&>(m_storage);
}

//-----------------------------------------------------------------------------
// Private Constructors
//-----------------------------------------------------------------------------

template<std::size_t Idx, typename T0, typename T1, typename...Ts>
template<typename Tuple, std::size_t...Idxs, typename...Tuples>
inline bit::memory::detail::ebo_storage_impl<Idx,T0,false,T1,Ts...>
  ::ebo_storage_impl( Tuple&& tuple, std::index_sequence<Idxs...>, Tuples&&...tuples)
  : base_type( std::forward<Tuples>(tuples)... ),
    m_storage( std::get<Idxs>( std::forward<Tuple>(tuple) )... )
{

}

//=============================================================================
// ebo_storage_impl<Idx,T0,false>
//=============================================================================

//-----------------------------------------------------------------------------
// Protected Constructors
//-----------------------------------------------------------------------------

template<std::size_t Idx, typename T0>
template<typename Tuple, typename>
inline bit::memory::detail::ebo_storage_impl<Idx,T0,false>
  ::ebo_storage_impl( Tuple&& tuple )
  : ebo_storage_impl( std::forward<Tuple>(tuple),
                      std::make_index_sequence<std::tuple_size<std::decay_t<Tuple>>::value>{} )
{

}

//-----------------------------------------------------------------------------
// Protected Observers
//-----------------------------------------------------------------------------

template<std::size_t Idx, typename T0>
inline T0& bit::memory::detail::ebo_storage_impl<Idx,T0,false>
  ::get( std::integral_constant<std::size_t,Idx> )
  &
{
  return m_storage;
}

template<std::size_t Idx, typename T0>
inline const T0& bit::memory::detail::ebo_storage_impl<Idx,T0,false>
  ::get( std::integral_constant<std::size_t,Idx> )
  const &
{
  return m_storage;
}

template<std::size_t Idx, typename T0>
inline T0&& bit::memory::detail::ebo_storage_impl<Idx,T0,false>
  ::get( std::integral_constant<std::size_t,Idx> )
  &&
{
  return static_cast<T0&&>(m_storage);
}

template<std::size_t Idx, typename T0>
inline const T0&& bit::memory::detail::ebo_storage_impl<Idx,T0,false>
  ::get( std::integral_constant<std::size_t,Idx> )
  const &&
{
  return static_cast<const T0&&>(m_storage);
}

//-----------------------------------------------------------------------------
// Private Constructors
//-----------------------------------------------------------------------------

template<std::size_t Idx, typename T0>
template<typename Tuple, std::size_t...Idxs>
inline bit::memory::detail::ebo_storage_impl<Idx,T0,false>
  ::ebo_storage_impl( Tuple&& tuple, std::index_sequence<Idxs...> )
  : m_storage( std::get<Idxs>( std::forward<Tuple>(tuple) )... )
{

}

//=============================================================================
// ebo_storage
//=============================================================================

//-----------------------------------------------------------------------------
// Constructors
//-----------------------------------------------------------------------------

template<typename T0, typename...Ts>
template<typename...Tuples, typename>
bit::memory::ebo_storage<T0, Ts...>::ebo_storage( Tuples&&...tuples )
 : base_type( std::forward<Tuples>(tuples)... )
{

}

#endif /* BIT_MEMORY_UTILITIES_DETAIL_EBO_STORAGE_INL */
