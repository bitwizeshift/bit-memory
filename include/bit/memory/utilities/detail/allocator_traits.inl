#ifndef BIT_MEMORY_UTILITIES_DETAIL_ALLOCATOR_TRAITS_INL
#define BIT_MEMORY_UTILITIES_DETAIL_ALLOCATOR_TRAITS_INL

namespace bit { namespace memory { namespace detail {

  template<typename Allocator>
  struct allocator_traits_impl
  {
    //-------------------------------------------------------------------------
    // Public Types
    //-------------------------------------------------------------------------

    using traits_type   = allocator_traits<Allocator>;
    using pointer       = typename traits_type::pointer;
    using const_pointer = typename traits_type::const_pointer;

    using size_type = typename traits_type::size_type;

    template<typename T>
    using typed_pointer = typename traits_type::template typed_pointer<T>;

    //-------------------------------------------------------------------------
    // Private Allocation
    //-------------------------------------------------------------------------

    static pointer do_try_allocate_hint( std::true_type,
                                         Allocator& alloc,
                                         const_pointer hint,
                                         size_type size,
                                         size_type align );
    static pointer do_try_allocate_hint( std::false_type,
                                         Allocator& alloc,
                                         const_pointer hint,
                                         size_type size,
                                         size_type align );

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

    static pointer do_allocate( std::true_type,
                                Allocator& alloc,
                                size_type size,
                                size_type align );
    static pointer do_allocate( std::false_type,
                                Allocator& alloc,
                                size_type size,
                                size_type align );

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

    static pointer do_allocate_hint( std::true_type,
                                     Allocator& alloc,
                                     const_pointer p,
                                     size_type size,
                                     size_type align );
    static pointer do_allocate_hint( std::false_type,
                                     Allocator& alloc,
                                     const_pointer p,
                                     size_type size,
                                     size_type align );

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

    //-------------------------------------------------------------------------

    static bool do_expand( std::true_type,
                           Allocator& alloc,
                           pointer p,
                           size_type new_size );
    static bool do_expand( std::false_type,
                           Allocator& alloc,
                           pointer p,
                           size_type new_size );

    //-------------------------------------------------------------------------
    // Observers
    //-------------------------------------------------------------------------

    size_type do_recommended_allocation_size( std::true_type,
                                              const Allocator& alloc,
                                              size_type requested );
    size_type do_recommended_allocation_size( std::false_type,
                                              const Allocator& alloc,
                                              size_type requested );

    static allocator_info do_info( std::true_type, const Allocator& alloc );
    static allocator_info do_info( std::false_type, const Allocator& alloc );

    //-------------------------------------------------------------------------
    // Constructions
    //-------------------------------------------------------------------------

    template<typename T, typename...Args>
    static void do_construct( std::true_type,
                              Allocator& alloc,
                              void* p,
                              Args&&...args );
    template<typename T, typename...Args>
    static void do_construct( std::false_type,
                              Allocator& alloc,
                              void* p,
                              Args&&...args );

    //-------------------------------------------------------------------------

    template<typename T, typename...Args>
    static typename std::pointer_traits<pointer>::template rebind<T>
      do_make( std::true_type,
               Allocator& alloc,
               Args&&...args );
    template<typename T, typename...Args>
    static typename std::pointer_traits<pointer>::template rebind<T>
      do_make( std::false_type,
               Allocator& alloc,
               Args&&...args );


    template<typename T, typename...Args>
    static typename std::pointer_traits<pointer>::template rebind<T>
      do_make_nothrow( std::true_type,
                       Allocator& alloc,
                       Args&&...args )
      noexcept;
    template<typename T, typename...Args>
    static typename std::pointer_traits<pointer>::template rebind<T>
      do_make_nothrow( std::false_type,
                       Allocator& alloc,
                       Args&&...args );

    //-------------------------------------------------------------------------

    template<typename T, typename...Args>
    static typename std::pointer_traits<pointer>::template rebind<T>
      do_make_array( std::true_type,
                     Allocator& alloc,
                     std::size_t n,
                     Args&&...args );
    template<typename T, typename...Args>
    static typename std::pointer_traits<pointer>::template rebind<T>
      do_make_array( std::false_type,
                     Allocator& alloc,
                     std::size_t n,
                     Args&&...args );

    template<typename T, typename...Args>
    static typename std::pointer_traits<pointer>::template rebind<T>
      do_make_array_nothrow( std::true_type,
                             Allocator& alloc,
                             std::size_t n,
                             Args&&...args )
      noexcept;
    template<typename T, typename...Args>
    static typename std::pointer_traits<pointer>::template rebind<T>
      do_make_array_nothrow( std::false_type,
                             Allocator& alloc,
                             std::size_t n,
                             Args&&...args );

    //-------------------------------------------------------------------------
    // Destruction
    //-------------------------------------------------------------------------

    template<typename T>
    static void do_destroy( std::true_type,
                            Allocator& alloc,
                            T* p );
    template<typename T>
    static void do_destroy( std::false_type,
                            Allocator& alloc,
                            T* p );

    //-------------------------------------------------------------------------

    template<typename T>
    static void do_dispose( std::true_type,
                            Allocator& alloc,
                            typed_pointer<T> p );
    template<typename T>
    static void do_dispose( std::false_type,
                            Allocator& alloc,
                            typed_pointer<T> p );


    template<typename T>
    static void do_dispose_array( std::true_type,
                                  Allocator& alloc,
                                  typed_pointer<T> p,
                                  size_type n );
    template<typename T>
    static void do_dispose_array( std::false_type,
                                  Allocator& alloc,
                                  typed_pointer<T> p,
                                  size_type n );

    //-------------------------------------------------------------------------
    // Private Capacity
    //-------------------------------------------------------------------------

    static bool do_is_unbounded( std::true_type, const Allocator& alloc );
    static bool do_is_unbounded( std::false_type, const Allocator& alloc );


    static size_type do_max_size( std::true_type, const Allocator& alloc );
    static size_type do_max_size( std::false_type, const Allocator& alloc );

    //-----------------------------------------------------------------------

    static size_type do_min_size( std::true_type, const Allocator& alloc );
    static size_type do_min_size( std::false_type, const Allocator& alloc );
  };

} } } // namespace bit::memory::detail

//=============================================================================
// allocator_traits
//=============================================================================

//-----------------------------------------------------------------------------
// Allocation
//-----------------------------------------------------------------------------

template<typename Allocator>
inline typename bit::memory::allocator_traits<Allocator>::pointer
  bit::memory::allocator_traits<Allocator>::try_allocate( Allocator& alloc,
                                                          size_type size,
                                                          size_type align )
  noexcept
{
  // try_allocate *must* be defined
  return alloc.try_allocate( size, align );
}

template<typename Allocator>
inline typename bit::memory::allocator_traits<Allocator>::pointer
  bit::memory::allocator_traits<Allocator>::try_allocate( Allocator& alloc,
                                                          const_pointer hint,
                                                          size_type size,
                                                          size_type align )
  noexcept
{
  static constexpr auto tag = allocator_has_try_allocate_hint<Allocator>{};
  using impl_type = detail::allocator_traits_impl<Allocator>;

  return impl_type::do_try_allocate_hint( tag, hint, size, align );
}

//-----------------------------------------------------------------------------

template<typename Allocator>
template<typename U,typename>
inline typename bit::memory::allocator_traits<Allocator>::pointer
  bit::memory::allocator_traits<Allocator>::try_allocate( Allocator& alloc,
                                                          size_type size,
                                                          size_type align,
                                                          size_type offset )
  noexcept
{
  // try_allocate *must* be defined for ExtendedAllocator
  return alloc.try_allocate( size, align, offset );
}

template<typename Allocator>
template<typename U,typename>
inline typename bit::memory::allocator_traits<Allocator>::pointer
  bit::memory::allocator_traits<Allocator>::try_allocate( Allocator& alloc,
                                                          const_pointer hint,
                                                          size_type size,
                                                          size_type align,
                                                          size_type offset )
  noexcept
{
  static constexpr auto tag = allocator_has_extended_try_allocate_hint<Allocator>{};
  using impl_type = detail::allocator_traits_impl<Allocator>;

  // try_allocate *must* be defined for ExtendedAllocator
  return impl_type::do_extended_try_allocate_hint( tag, size, align, offset );
}

//-----------------------------------------------------------------------------

template<typename Allocator>
inline typename bit::memory::allocator_traits<Allocator>::pointer
  bit::memory::allocator_traits<Allocator>::allocate( Allocator& alloc,
                                                      size_type size,
                                                      size_type align )
{
  static constexpr auto tag = allocator_has_allocate<Allocator>{};
  using impl_type = detail::allocator_traits_impl<Allocator>;

  return impl_type::do_allocate( tag , alloc, size, align );
}

template<typename Allocator>
inline typename bit::memory::allocator_traits<Allocator>::pointer
  bit::memory::allocator_traits<Allocator>::allocate( Allocator& alloc,
                                                      const_pointer hint,
                                                      size_type size,
                                                      size_type align )
{
  static constexpr auto tag = allocator_has_allocate_hint<Allocator>{};
  using impl_type = detail::allocator_traits_impl<Allocator>;

  return impl_type::do_allocate_hint( tag , alloc, size, align );
}

//-----------------------------------------------------------------------------

template<typename Allocator>
template<typename U,typename>
inline typename bit::memory::allocator_traits<Allocator>::pointer
  bit::memory::allocator_traits<Allocator>::allocate( Allocator& alloc,
                                                      size_type size,
                                                      size_type align,
                                                      size_type offset )
{
  static constexpr auto tag = allocator_has_extended_allocate<Allocator>{};
  using impl_type = detail::allocator_traits_impl<Allocator>;

  return impl_type::do_extended_allocate( tag, alloc, size, align, offset );
}

template<typename Allocator>
template<typename U,typename>
inline typename bit::memory::allocator_traits<Allocator>::pointer
  bit::memory::allocator_traits<Allocator>::allocate( Allocator& alloc,
                                                      const_pointer p,
                                                      size_type size,
                                                      size_type align,
                                                      size_type offset )
{
  static constexpr auto tag = allocator_has_extended_allocate_hint<Allocator>{};
  using impl_type = detail::allocator_traits_impl<Allocator>;

  return impl_type::do_extended_allocate_hint( tag, alloc, size, align, offset );
}

//-----------------------------------------------------------------------------

template<typename Allocator>
inline bool bit::memory::allocator_traits<Allocator>
  ::expand( Allocator& alloc,
            pointer p,
            size_type new_size )
{
  static constexpr auto tag = allocator_has_expand<Allocator>{};
  using impl_type = detail::allocator_traits_impl<Allocator>;

  return impl_type::do_expand( tag, alloc, p, new_size );
}

//-----------------------------------------------------------------------------
// Deallocation
//-----------------------------------------------------------------------------

template<typename Allocator>
inline void bit::memory::allocator_traits<Allocator>
  ::deallocate( Allocator& alloc, pointer p, size_type size )
{
  // deallocate *must* be defined to be considered 'Allocator'
  alloc.deallocate( p, size );
}

template<typename Allocator>
template<typename U,typename>
inline void bit::memory::allocator_traits<Allocator>
  ::deallocate_all( Allocator& alloc )
{
  alloc.deallocate_all();
}

//-----------------------------------------------------------------------------
// Observers
//-----------------------------------------------------------------------------

template<typename Allocator>
typename bit::memory::allocator_traits<Allocator>::size_type
  bit::memory::allocator_traits<Allocator>
  ::recommended_allocation_size( const Allocator& alloc, size_type requested )
{
  static constexpr auto tag = allocator_has_recommended_allocation_size<Allocator>{};
  using impl_type = detail::allocator_traits_impl<Allocator>;

  impl_type::do_recommended_allocation_size( tag, alloc, requested );
}

template<typename Allocator>
inline bool bit::memory::allocator_traits<Allocator>
  ::owns( const Allocator& alloc, const_pointer p )
  noexcept
{
  return alloc.owns(p);
}

template<typename Allocator>
inline bit::memory::allocator_info
  bit::memory::allocator_traits<Allocator>::info( const Allocator& alloc )
  noexcept
{
  using impl_type = detail::allocator_traits_impl<Allocator>;

  return impl_type::do_info( allocator_has_info<Allocator>{}, alloc );
}

//-----------------------------------------------------------------------------
// Construction
//-----------------------------------------------------------------------------

template<typename Allocator>
template<typename T, typename...Args>
inline void  bit::memory::allocator_traits<Allocator>
  ::construct( Allocator& alloc, void* p, Args&&...args )
{
  static constexpr auto tag = allocator_has_construct<Allocator,T,Args...>{};
  using impl_type = detail::allocator_traits_impl<Allocator>;

  impl_type::template do_construct<T>( tag, alloc, p, std::forward<Args>(args)... );
}

template<typename Allocator>
template<typename T, typename...Args>
inline typename std::pointer_traits<typename bit::memory::allocator_traits<Allocator>::pointer>::template rebind<T>
  bit::memory::allocator_traits<Allocator>
  ::make( Allocator& alloc, Args&&...args )
{
  static constexpr auto tag = allocator_has_make<Allocator,T,Args...>{};
  using impl_type = detail::allocator_traits_impl<Allocator>;

  return impl_type::template do_make<T>( tag, alloc, std::forward<Args>(args)... );
}


template<typename Allocator>
template<typename T>
inline typename std::pointer_traits<typename bit::memory::allocator_traits<Allocator>::pointer>::template rebind<T>
  bit::memory::allocator_traits<Allocator>
  ::make_array( Allocator& alloc, size_type n )
{
  static constexpr auto tag = allocator_has_make_array<Allocator,T,size_type>{};
  using impl_type = detail::allocator_traits_impl<Allocator>;

  return impl_type::template do_make_array<T>( tag, alloc, n );
}

template<typename Allocator>
template<typename T>
inline typename std::pointer_traits<typename bit::memory::allocator_traits<Allocator>::pointer>::template rebind<T>
  bit::memory::allocator_traits<Allocator>
  ::make_array( Allocator& alloc, size_type n, const T& copy )
{
  static constexpr auto tag = allocator_has_make_array<Allocator,T,size_type,const T&>{};
  using impl_type = detail::allocator_traits_impl<Allocator>;

  return impl_type::template do_make_array<T>( tag, alloc, n, copy );
}

//-----------------------------------------------------------------------------
// Destruction
//-----------------------------------------------------------------------------

template<typename Allocator>
template<typename T>
void bit::memory::allocator_traits<Allocator>::destroy( Allocator& alloc, T* p )
{
  static constexpr auto tag = allocator_has_destroy<Allocator,T*>{};
  using impl_type = detail::allocator_traits_impl<Allocator>;

  impl_type::do_destroy( tag, alloc, p );
}


template<typename Allocator>
template<typename T>
void bit::memory::allocator_traits<Allocator>
  ::dispose( Allocator& alloc,
             typed_pointer<T> p )
{
  static constexpr auto tag = allocator_has_dispose<Allocator,typed_pointer<T>>{};
  using impl_type = detail::allocator_traits_impl<Allocator>;

  impl_type::do_dispose( tag, alloc, std::move(p) );
}

template<typename Allocator>
template<typename T>
void bit::memory::allocator_traits<Allocator>
  ::dispose_array( Allocator& alloc,
                   typed_pointer<T> p,
                   size_type n )
{
  static constexpr auto tag = allocator_has_dispose_array<Allocator,typed_pointer<T>>{};
  using impl_type = detail::allocator_traits_impl<Allocator>;

  impl_type::do_dispose_array( tag, alloc, std::move(p), n );
}

//-----------------------------------------------------------------------------
// Capacity
//-----------------------------------------------------------------------------

template<typename Allocator>
inline bool
  bit::memory::allocator_traits<Allocator>::is_unbounded( const Allocator& alloc )
  noexcept
{
  using impl_type = detail::allocator_traits_impl<Allocator>;

  return impl_type::do_is_unbounded( allocator_has_is_unbounded<Allocator>{}, alloc );
}

template<typename Allocator>
inline typename bit::memory::allocator_traits<Allocator>::size_type
  bit::memory::allocator_traits<Allocator>::max_size( const Allocator& alloc )
  noexcept
{
  using impl_type = detail::allocator_traits_impl<Allocator>;

  return impl_type::do_max_size( allocator_has_max_size<Allocator>{}, alloc );
}

template<typename Allocator>
inline typename bit::memory::allocator_traits<Allocator>::size_type
  bit::memory::allocator_traits<Allocator>::min_size( const Allocator& alloc )
  noexcept
{
  using impl_type = detail::allocator_traits_impl<Allocator>;

  return impl_type::do_min_size( allocator_has_min_size<Allocator>{}, alloc );
}

//=============================================================================
// detail::allocator_traits_impl
//=============================================================================

//-----------------------------------------------------------------------------
// Try Allocation With Hints
//-----------------------------------------------------------------------------

template<typename Allocator>
inline typename bit::memory::allocator_traits<Allocator>::pointer
  bit::memory::detail::allocator_traits_impl<Allocator>
  ::do_try_allocate_hint( std::true_type,
                          Allocator& alloc,
                          const_pointer hint,
                          size_type size,
                          size_type align )
{
  return alloc.try_allocate( hint, size, align );
}

template<typename Allocator>
inline typename bit::memory::allocator_traits<Allocator>::pointer
  bit::memory::detail::allocator_traits_impl<Allocator>
  ::do_try_allocate_hint( std::false_type,
                          Allocator& alloc,
                          const_pointer hint,
                          size_type size,
                          size_type align )
{
  BIT_MEMORY_UNUSED(hint);

  return traits_type::try_allocate( alloc, size, align );
}

//-----------------------------------------------------------------------------

template<typename Allocator>
inline typename bit::memory::allocator_traits<Allocator>::pointer
  bit::memory::detail::allocator_traits_impl<Allocator>
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
inline typename bit::memory::allocator_traits<Allocator>::pointer
  bit::memory::detail::allocator_traits_impl<Allocator>
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
// Allocation
//-----------------------------------------------------------------------------

template<typename Allocator>
inline typename bit::memory::allocator_traits<Allocator>::pointer
  bit::memory::detail::allocator_traits_impl<Allocator>
  ::do_allocate( std::true_type,
                 Allocator& alloc,
                 size_type size,
                 size_type align )
{
  return alloc.allocate( size, align );
}

template<typename Allocator>
inline typename bit::memory::allocator_traits<Allocator>::pointer
  bit::memory::detail::allocator_traits_impl<Allocator>
  ::do_allocate( std::false_type,
                 Allocator& alloc,
                 size_type size,
                 size_type align )
{
  auto p = traits_type::try_allocate(alloc,size,align);

  if( BIT_MEMORY_UNLIKELY(p == nullptr) ) {
    const auto info = allocator_traits<Allocator>::info( alloc );

    get_out_of_memory_handler()(info, size);
  }

  return p;
}

//-----------------------------------------------------------------------------

template<typename Allocator>
inline typename bit::memory::allocator_traits<Allocator>::pointer
  bit::memory::detail::allocator_traits_impl<Allocator>
  ::do_extended_allocate( std::true_type,
                          Allocator& alloc,
                          size_type size,
                          size_type align,
                          size_type offset )
{
  return alloc.allocate( size, align, offset );
}

template<typename Allocator>
inline typename bit::memory::allocator_traits<Allocator>::pointer
  bit::memory::detail::allocator_traits_impl<Allocator>
  ::do_extended_allocate( std::false_type,
                          Allocator& alloc,
                          size_type size,
                          size_type align,
                          size_type offset )
{
  auto p = traits_type::try_allocate( alloc, size, align, offset );

  if( p == nullptr ) {
    const auto info = allocator_traits<Allocator>::info( alloc );

    get_out_of_memory_handler()(info, size);
  }

  return p;
}

//-----------------------------------------------------------------------------
// Allocation With Hints
//-----------------------------------------------------------------------------

template<typename Allocator>
inline typename bit::memory::allocator_traits<Allocator>::pointer
  bit::memory::detail::allocator_traits_impl<Allocator>
  ::do_allocate_hint( std::true_type,
                      Allocator& alloc,
                      const_pointer hint,
                      size_type size,
                      size_type align )
{
  return alloc.allocate( hint, size, align );
}

template<typename Allocator>
inline typename bit::memory::allocator_traits<Allocator>::pointer
  bit::memory::detail::allocator_traits_impl<Allocator>
  ::do_allocate_hint( std::false_type,
                      Allocator& alloc,
                      const_pointer hint,
                      size_type size,
                      size_type align )
{
  BIT_MEMORY_UNUSED(hint);

  return traits_type::allocate( alloc, size, align );
}

//-----------------------------------------------------------------------------

template<typename Allocator>
inline typename bit::memory::allocator_traits<Allocator>::pointer
  bit::memory::detail::allocator_traits_impl<Allocator>
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
inline typename bit::memory::allocator_traits<Allocator>::pointer
  bit::memory::detail::allocator_traits_impl<Allocator>
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

//-----------------------------------------------------------------------------

template<typename Allocator>
inline bool
  bit::memory::detail::allocator_traits_impl<Allocator>
  ::do_expand( std::true_type,
               Allocator& alloc,
               pointer p,
               size_type new_size)
{
  return alloc.expand( p, new_size );
}

template<typename Allocator>
inline bool
  bit::memory::detail::allocator_traits_impl<Allocator>
  ::do_expand( std::false_type,
               Allocator& alloc,
               pointer p,
               size_type new_size)
{
  BIT_MEMORY_UNUSED(alloc);
  BIT_MEMORY_UNUSED(p);
  BIT_MEMORY_UNUSED(new_size);

  return false;
}

//-----------------------------------------------------------------------------
// Observers
//-----------------------------------------------------------------------------

template<typename Allocator>
inline typename bit::memory::allocator_traits<Allocator>::size_type
  bit::memory::detail::allocator_traits_impl<Allocator>
  ::do_recommended_allocation_size( std::true_type,
                                    const Allocator& alloc,
                                    size_type requested )
{
  return alloc.recommended_allocation_size( requested );
}

template<typename Allocator>
inline typename bit::memory::allocator_traits<Allocator>::size_type
  bit::memory::detail::allocator_traits_impl<Allocator>
  ::do_recommended_allocation_size( std::false_type,
                                    const Allocator& alloc,
                                    size_type requested )
{
  BIT_MEMORY_UNUSED(alloc);

  static constexpr auto multiple = traits_type::default_alignment::value;

  return ((requested + multiple - 1) & ~multiple);
}

//-----------------------------------------------------------------------------

template<typename Allocator>
inline bit::memory::allocator_info
  bit::memory::detail::allocator_traits_impl<Allocator>
  ::do_info( std::true_type,
             const Allocator& alloc )
{
  return alloc.info();
}

template<typename Allocator>
inline bit::memory::allocator_info
  bit::memory::detail::allocator_traits_impl<Allocator>
  ::do_info( std::false_type,
             const Allocator& alloc )
{
  BIT_MEMORY_UNUSED(alloc);

  return {typeid(Allocator).name(),std::addressof(alloc)};
}

//-----------------------------------------------------------------------------
// Construction
//-----------------------------------------------------------------------------

template<typename Allocator>
template<typename T, typename...Args>
inline void bit::memory::detail::allocator_traits_impl<Allocator>
  ::do_construct( std::true_type,
                  Allocator& alloc,
                  void* p,
                  Args&&...args )
{
  alloc.template construct<T>( p, std::forward<Args>(args)... );
}

template<typename Allocator>
template<typename T, typename...Args>
inline void bit::memory::detail::allocator_traits_impl<Allocator>
  ::do_construct( std::false_type,
                  Allocator& alloc,
                  void* p,
                  Args&&...args )
{
  uninitialized_construct_at<T>( p, std::forward<Args>(args)... );
}

//-----------------------------------------------------------------------------

template<typename Allocator>
template<typename T, typename...Args>
inline typename std::pointer_traits<typename bit::memory::allocator_traits<Allocator>::pointer>::template rebind<T>
  bit::memory::detail::allocator_traits_impl<Allocator>
  ::do_make( std::true_type,
             Allocator& alloc,
             Args&&...args )
{
  return alloc.template make<T>( std::forward<Args>(args)... );
}

template<typename Allocator>
template<typename T, typename...Args>
inline typename std::pointer_traits<typename bit::memory::allocator_traits<Allocator>::pointer>::template rebind<T>
  bit::memory::detail::allocator_traits_impl<Allocator>
  ::do_make( std::false_type,
             Allocator& alloc,
             Args&&...args )
{
  static const auto tag = std::is_nothrow_constructible<T,Args...>{};

  return do_make_nothrow<T>( tag, alloc, std::forward<Args>(args)... );
}

template<typename Allocator>
template<typename T, typename...Args>
inline typename std::pointer_traits<typename bit::memory::allocator_traits<Allocator>::pointer>::template rebind<T>
  bit::memory::detail::allocator_traits_impl<Allocator>
  ::do_make_nothrow( std::true_type,
                     Allocator& alloc,
                     Args&&...args )
  noexcept
{
  auto p = traits_type::allocate( alloc, sizeof(T), alignof(T) );

  return uninitialized_construct_at<T>( to_raw_pointer(p),
                                        std::forward<Args>(args)... );
}

template<typename Allocator>
template<typename T, typename...Args>
inline typename std::pointer_traits<typename bit::memory::allocator_traits<Allocator>::pointer>::template rebind<T>
  bit::memory::detail::allocator_traits_impl<Allocator>
  ::do_make_nothrow( std::false_type,
                     Allocator& alloc,
                     Args&&...args )
{
  auto p = traits_type::allocate( alloc, sizeof(T), alignof(T) );
  auto void_ptr = to_raw_pointer(p);
  auto type_ptr = static_cast<T*>(void_ptr);

  try {
    traits_type::template construct<T>( to_raw_pointer(p),
                                        std::forward<Args>(args)... );
  } catch ( ... ) {
    traits_type::deallocate( alloc, p, sizeof(T) );
    throw;
  }
  return std::pointer_traits<typed_pointer<T>>::pointer_to(*type_ptr);
}

//-----------------------------------------------------------------------------

template<typename Allocator>
template<typename T, typename...Args>
inline typename std::pointer_traits<typename bit::memory::allocator_traits<Allocator>::pointer>::template rebind<T>
  bit::memory::detail::allocator_traits_impl<Allocator>
  ::do_make_array( std::true_type,
                   Allocator& alloc,
                   std::size_t n,
                   Args&&...args )
{
  return alloc.template make_array<T>( n, std::forward<Args>(args)...);
}

template<typename Allocator>
template<typename T, typename...Args>
inline typename std::pointer_traits<typename bit::memory::allocator_traits<Allocator>::pointer>::template rebind<T>
  bit::memory::detail::allocator_traits_impl<Allocator>
  ::do_make_array( std::false_type,
                   Allocator& alloc,
                   std::size_t n,
                   Args&&...args )
{
  static const auto tag = std::is_nothrow_constructible<T,Args...>{};

  return do_make_array_nothrow<T>( tag, alloc, n );
}

template<typename Allocator>
template<typename T, typename...Args>
inline typename std::pointer_traits<typename bit::memory::allocator_traits<Allocator>::pointer>::template rebind<T>
  bit::memory::detail::allocator_traits_impl<Allocator>
  ::do_make_array_nothrow( std::true_type,
                           Allocator& alloc,
                           std::size_t n,
                           Args&&...args )
  noexcept
{
  auto p        = traits_type::allocate( alloc, sizeof(T)*n, alignof(T) );
  auto void_ptr = to_raw_pointer(p);
  auto type_ptr = static_cast<T*>(void_ptr);
  auto current  = type_ptr;

  const auto end = current + n;

  for( ; current != end; ++current ) {
    traits_type::template construct<T>( alloc, void_ptr, std::forward<Args>(args)... );
  }

  return std::pointer_traits<typed_pointer<T>>::pointer_to(*type_ptr);
}

template<typename Allocator>
template<typename T, typename...Args>
inline typename std::pointer_traits<typename bit::memory::allocator_traits<Allocator>::pointer>::template rebind<T>
  bit::memory::detail::allocator_traits_impl<Allocator>
  ::do_make_array_nothrow( std::false_type,
                           Allocator& alloc,
                           std::size_t n,
                           Args&&...args )
{
  auto p        = traits_type::allocate( alloc, sizeof(T)*n, alignof(T) );
  auto void_ptr = to_raw_pointer(p);
  auto type_ptr = static_cast<T*>(void_ptr);
  auto current  = type_ptr;

  try {
    const auto end = current + n;

    for( ; current != end; ++current ) {
      traits_type::template construct<T>( alloc, void_ptr, std::forward<Args>(args)... );
    }

  } catch ( ... ) {
    const auto rend = type_ptr - 1;
    --current;
    for( ; current != rend; --current ) {
      traits_type::destroy( current );
    }
    traits_type::deallocate( alloc, p, sizeof(T)*n );
    throw;
  }

  return std::pointer_traits<typed_pointer<T>>::pointer_to(*type_ptr);
}

//-----------------------------------------------------------------------------
// Destruction
//-----------------------------------------------------------------------------

template<typename Allocator>
template<typename T>
void bit::memory::detail::allocator_traits_impl<Allocator>
  ::do_destroy( std::true_type,
                Allocator& alloc,
                T* p )
{
  alloc.destroy( p );
}

template<typename Allocator>
template<typename T>
void bit::memory::detail::allocator_traits_impl<Allocator>
  ::do_destroy( std::false_type,
                Allocator& alloc,
                T* p )
{
  static_assert( !std::is_abstract<T>::value,
                 "Cannot dispose from base type" );

  // call the destructor for p
  destroy_at( p );
}

//-----------------------------------------------------------------------------

template<typename Allocator>
template<typename T>
void bit::memory::detail::allocator_traits_impl<Allocator>
  ::do_dispose( std::true_type,
                Allocator& alloc,
                typed_pointer<T> p )
{
  alloc.dispose( p );
}

template<typename Allocator>
template<typename T>
void bit::memory::detail::allocator_traits_impl<Allocator>
  ::do_dispose( std::false_type,
                Allocator& alloc,
                typed_pointer<T> p )
{
  static_assert( !std::is_abstract<T>::value,
                 "Cannot dispose from base type" );

  // call the destructor for p
  destroy_at( to_raw_pointer(p) );

  // convert to raw pointer first
  traits_type::deallocate( alloc, static_cast<pointer>(p), sizeof(T) );
}

//-----------------------------------------------------------------------------

template<typename Allocator>
template<typename T>
void bit::memory::detail::allocator_traits_impl<Allocator>
  ::do_dispose_array( std::true_type,
                      Allocator& alloc,
                      typed_pointer<T> p,
                      size_type n )
{
  alloc.dispose_array( p, n );
}

template<typename Allocator>
template<typename T>
void bit::memory::detail::allocator_traits_impl<Allocator>
  ::do_dispose_array( std::false_type,
                      Allocator& alloc,
                      typed_pointer<T> p,
                      size_type n )
{
  static_assert( !std::is_abstract<T>::value,
                 "Cannot dispose from base type" );

  destroy_array_at( to_raw_pointer(p), n );
  traits_type::deallocate( alloc, static_cast<pointer>(p), sizeof(T)*n );
}

//-----------------------------------------------------------------------------
// Capacity
//-----------------------------------------------------------------------------

template<typename Allocator>
inline bool
  bit::memory::detail::allocator_traits_impl<Allocator>
  ::do_is_unbounded( std::true_type, const Allocator& alloc )
{
  return alloc.is_unbounded();
}

template<typename Allocator>
inline bool
  bit::memory::detail::allocator_traits_impl<Allocator>
  ::do_is_unbounded( std::false_type, const Allocator& alloc )
{
  return std::numeric_limits<size_type>::max() == max_size( alloc );
}

//-----------------------------------------------------------------------------

template<typename Allocator>
inline typename bit::memory::allocator_traits<Allocator>::size_type
  bit::memory::detail::allocator_traits_impl<Allocator>
  ::do_max_size( std::true_type,
                 const Allocator& alloc )
{
  return alloc.max_size();
}

template<typename Allocator>
inline typename bit::memory::allocator_traits<Allocator>::size_type
  bit::memory::detail::allocator_traits_impl<Allocator>
  ::do_max_size( std::false_type,
                 const Allocator& alloc )
{
  BIT_MEMORY_UNUSED(alloc);

  return std::numeric_limits<size_type>::max();
}

//-----------------------------------------------------------------------------

template<typename Allocator>
inline typename bit::memory::allocator_traits<Allocator>::size_type
  bit::memory::detail::allocator_traits_impl<Allocator>
  ::do_min_size( std::true_type,
                 const Allocator& alloc )
{
  return alloc.min_size();
}

template<typename Allocator>
inline typename bit::memory::allocator_traits<Allocator>::size_type
  bit::memory::detail::allocator_traits_impl<Allocator>
  ::do_min_size( std::false_type,
                 const Allocator& alloc )
{
  BIT_MEMORY_UNUSED(alloc);

  return 1;
}

#endif /* BIT_MEMORY_UTILITIES_DETAIL_ALLOCATOR_TRAITS_INL */
