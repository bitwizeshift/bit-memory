#ifndef BIT_MEMORY_TRAITS_DETAIL_EXTENDED_ALLOCATOR_TRAITS_INL
#define BIT_MEMORY_TRAITS_DETAIL_EXTENDED_ALLOCATOR_TRAITS_INL

namespace bit { namespace memory { namespace detail {

  template<typename Allocator>
  struct extended_allocator_traits_impl
  {
    //-------------------------------------------------------------------------
    // Public Types
    //-------------------------------------------------------------------------

    using traits_type   = extended_allocator_traits<Allocator>;
    using pointer       = typename traits_type::pointer;
    using const_pointer = typename traits_type::const_pointer;

    using size_type = typename traits_type::size_type;

    template<typename T>
    using pointer_rebind = typename traits_type::template pointer_rebind<T>;

    //-------------------------------------------------------------------------
    // Private Allocations
    //-------------------------------------------------------------------------

    static pointer do_extended_try_allocate_hint( std::true_type,
                                                  Allocator& alloc,
                                                  const_pointer p,
                                                  size_type size,
                                                  size_type align,
                                                  size_type offset );
    static pointer do_extended_try_allocate_hint( std::false_type,
                                                  Allocator& alloc,
                                                  const_pointer p,
                                                  size_type size,
                                                  size_type align,
                                                  size_type offset );

    //-------------------------------------------------------------------------

    static pointer do_extended_allocate( std::true_type,
                                         Allocator& alloc,
                                         size_type size,
                                         size_type align,
                                         size_type offset );
    static pointer do_extended_allocate( std::false_type,
                                         Allocator& alloc,
                                         size_type size,
                                         size_type align,
                                         size_type offset );


    //-------------------------------------------------------------------------

    static pointer do_extended_allocate_hint( std::true_type,
                                              Allocator& alloc,
                                              const_pointer p,
                                              size_type size,
                                              size_type align,
                                              size_type offset );
    static pointer do_extended_allocate_hint( std::false_type,
                                              Allocator& alloc,
                                              const_pointer p,
                                              size_type size,
                                              size_type align,
                                              size_type offset );

  };

} } } // namespace bit::memory::detail

//-----------------------------------------------------------------------------
// Allocations
//-----------------------------------------------------------------------------

template<typename Allocator>
inline typename bit::memory::extended_allocator_traits<Allocator>::pointer
  bit::memory::extended_allocator_traits<Allocator>
  ::try_allocate( Allocator& alloc,
                  size_type size,
                  size_type align,
                  size_type offset )
  noexcept
{
  return alloc.try_allocate( size, align, offset );
}

template<typename Allocator>
inline typename bit::memory::extended_allocator_traits<Allocator>::pointer
  bit::memory::extended_allocator_traits<Allocator>
  ::try_allocate( Allocator& alloc,
                  const_pointer hint,
                  size_type size,
                  size_type align,
                  size_type offset )
  noexcept
{
  using impl_type = detail::extended_allocator_traits_impl<Allocator>;

  static constexpr auto tag = allocator_has_extended_try_allocate_hint<Allocator>{};

  return impl_type::do_extended_try_allocate_hint( tag, size, align, offset );
}


//-----------------------------------------------------------------------------

template<typename Allocator>
inline typename bit::memory::extended_allocator_traits<Allocator>::pointer
  bit::memory::extended_allocator_traits<Allocator>
  ::allocate( Allocator& alloc,
              size_type size,
              size_type align,
              size_type offset )
{
  using impl_type = detail::extended_allocator_traits_impl<Allocator>;

  static constexpr auto tag = allocator_has_extended_allocate<Allocator>{};

  return impl_type::do_extended_allocate( tag, alloc, size, align, offset );
}

template<typename Allocator>
inline typename bit::memory::extended_allocator_traits<Allocator>::pointer
  bit::memory::extended_allocator_traits<Allocator>
  ::allocate( Allocator& alloc,
              const_pointer p,
              size_type size,
              size_type align,
              size_type offset )
{
  using impl_type = detail::extended_allocator_traits_impl<Allocator>;

  static constexpr auto tag = allocator_has_extended_allocate_hint<Allocator>{};

  return impl_type::do_extended_allocate_hint( tag, alloc, size, align, offset );
}

//-----------------------------------------------------------------------------
// Private Allocations
//-----------------------------------------------------------------------------

template<typename Allocator>
inline typename bit::memory::extended_allocator_traits<Allocator>::pointer
  bit::memory::detail::extended_allocator_traits_impl<Allocator>
  ::do_extended_try_allocate_hint( std::true_type,
                                   Allocator& alloc,
                                   const_pointer hint,
                                   size_type size,
                                   size_type align,
                                   size_type offset )
{
  return alloc.try_allocate( hint, size, align, offset );
}

template<typename Allocator>
inline typename bit::memory::extended_allocator_traits<Allocator>::pointer
  bit::memory::detail::extended_allocator_traits_impl<Allocator>
  ::do_extended_try_allocate_hint( std::false_type,
                                   Allocator& alloc,
                                   const_pointer hint,
                                   size_type size,
                                   size_type align,
                                   size_type offset )
{
  BIT_MEMORY_UNUSED(hint);

  return traits_type::try_allocate( alloc, size, align, offset );
}

//-----------------------------------------------------------------------------

template<typename Allocator>
inline typename bit::memory::extended_allocator_traits<Allocator>::pointer
  bit::memory::detail::extended_allocator_traits_impl<Allocator>
  ::do_extended_allocate( std::true_type,
                          Allocator& alloc,
                          size_type size,
                          size_type align,
                          size_type offset )
{
  return alloc.allocate( size, align, offset );
}

template<typename Allocator>
inline typename bit::memory::extended_allocator_traits<Allocator>::pointer
  bit::memory::detail::extended_allocator_traits_impl<Allocator>
  ::do_extended_allocate( std::false_type,
                          Allocator& alloc,
                          size_type size,
                          size_type align,
                          size_type offset )
{
  auto p = traits_type::try_allocate( alloc, size, align, offset );

  // Assume null allocations are unlikely, since they are the expensive
  // code-path to manage
  if( BIT_MEMORY_UNLIKELY(p == nullptr) ) {
    const auto info = traits_type::info( alloc );

    (*get_out_of_memory_handler())(info, size);

    // Invoking the out-of-memory handler must not return
    BIT_MEMORY_UNREACHABLE();
  }

  return p;
}

//-----------------------------------------------------------------------------

template<typename Allocator>
inline typename bit::memory::extended_allocator_traits<Allocator>::pointer
  bit::memory::detail::extended_allocator_traits_impl<Allocator>
  ::do_extended_allocate_hint( std::true_type,
                               Allocator& alloc,
                               const_pointer hint,
                               size_type size,
                               size_type align,
                               size_type offset )
{
  return alloc.allocate( hint, size, align, offset );
}

template<typename Allocator>
inline typename bit::memory::extended_allocator_traits<Allocator>::pointer
  bit::memory::detail::extended_allocator_traits_impl<Allocator>
  ::do_extended_allocate_hint( std::false_type,
                               Allocator& alloc,
                               const_pointer hint,
                               size_type size,
                               size_type align,
                               size_type offset )
{
  BIT_MEMORY_UNUSED(hint);

  return traits_type::allocate( alloc, size, align, offset );
}


#endif /* BIT_MEMORY_TRAITS_DETAIL_EXTENDED_ALLOCATOR_TRAITS_INL */
