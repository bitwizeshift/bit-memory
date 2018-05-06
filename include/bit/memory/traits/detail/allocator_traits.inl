#ifndef BIT_MEMORY_TRAITS_DETAIL_ALLOCATOR_TRAITS_INL
#define BIT_MEMORY_TRAITS_DETAIL_ALLOCATOR_TRAITS_INL

namespace bit { namespace memory { namespace detail {

  template<typename Allocator, typename T, typename = void>
  struct allocator_has_pointer_to : std::false_type{};

  template<typename Allocator, typename T>
  struct allocator_has_pointer_to<Allocator, T,void_t<decltype(std::declval<Allocator&>().pointer_to( std::declval<T&>() ) )>>
    : std::true_type{};

  //---------------------------------------------------------------------------

  template<typename Allocator, typename T, typename = void>
  struct allocator_has_to_raw_pointer : std::false_type{};

  template<typename Allocator, typename T>
  struct allocator_has_to_raw_pointer<Allocator, T,void_t<decltype(std::declval<Allocator&>().to_raw_pointer( std::declval<const T&>() ) )>>
    : std::true_type{};

  //---------------------------------------------------------------------------

  template<typename T, typename = void>
  struct pointer_traits_has_to_address : std::false_type{};

  template<typename T>
  struct pointer_traits_has_to_address<T,void_t<decltype(std::pointer_traits<T>::address_of( std::declval<typename std::pointer_traits<T>::element_type&>() ) )>>
    : std::true_type{};

  //---------------------------------------------------------------------------

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
    using pointer_rebind = typename traits_type::template pointer_rebind<T>;

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

    static pointer do_allocate( std::true_type,
                                Allocator& alloc,
                                size_type size,
                                size_type align );
    static pointer do_allocate( std::false_type,
                                Allocator& alloc,
                                size_type size,
                                size_type align );

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

    //-----------------------------------------------------------------------
    // Conversion
    //-----------------------------------------------------------------------
  public:

    template<typename T>
    static typename std::pointer_traits<pointer>::template rebind<T>
      do_pointer_to( std::true_type,
                     Allocator& alloc,
                     T& x );
    template<typename T>
    static typename std::pointer_traits<pointer>::template rebind<T>
      do_pointer_to( std::false_type,
                     Allocator& alloc,
                     T& x );

    //-----------------------------------------------------------------------

    template<typename Pointer>
    static typename std::pointer_traits<Pointer>::element_type*
      do_to_raw_pointer( std::true_type,
                         Allocator& alloc,
                         const Pointer& p );
    template<typename Pointer>
    static typename std::pointer_traits<Pointer>::element_type*
      do_to_raw_pointer( std::false_type,
                         Allocator& alloc,
                         const Pointer& p );
    template<typename Pointer>
    static typename std::pointer_traits<Pointer>::element_type*
      do_to_raw_pointer_pointer_traits( std::true_type,
                                        Allocator& alloc,
                                        const Pointer& p );
    template<typename Pointer>
    static typename std::pointer_traits<Pointer>::element_type*
      do_to_raw_pointer_pointer_traits( std::false_type,
                                        Allocator& alloc,
                                        const Pointer& p );

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
    static typename bit::memory::detail::allocator_pointer_rebind<Allocator,T>::type
      do_make( std::true_type,
               Allocator& alloc,
               Args&&...args );
    template<typename T, typename...Args>
    static typename bit::memory::detail::allocator_pointer_rebind<Allocator,T>::type
      do_make( std::false_type,
               Allocator& alloc,
               Args&&...args );


    template<typename T, typename...Args>
    static typename bit::memory::detail::allocator_pointer_rebind<Allocator,T>::type
      do_make_nothrow( std::true_type,
                       Allocator& alloc,
                       Args&&...args )
      noexcept;
    template<typename T, typename...Args>
    static typename bit::memory::detail::allocator_pointer_rebind<Allocator,T>::type
      do_make_nothrow( std::false_type,
                       Allocator& alloc,
                       Args&&...args );

    //-------------------------------------------------------------------------

    template<typename T, typename...Args>
    static typename bit::memory::detail::allocator_pointer_rebind<Allocator,T>::type
      do_make_array( std::true_type,
                     Allocator& alloc,
                     std::size_t n,
                     Args&&...args );
    template<typename T, typename...Args>
    static typename bit::memory::detail::allocator_pointer_rebind<Allocator,T>::type
      do_make_array( std::false_type,
                     Allocator& alloc,
                     std::size_t n,
                     Args&&...args );

    template<typename T, typename...Args>
    static typename bit::memory::detail::allocator_pointer_rebind<Allocator,T>::type
      do_make_array_nothrow( std::true_type,
                             Allocator& alloc,
                             std::size_t n,
                             Args&&...args )
      noexcept;
    template<typename T, typename...Args>
    static typename bit::memory::detail::allocator_pointer_rebind<Allocator,T>::type
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

    template<typename Pointer>
    static void do_dispose( std::true_type,
                            Allocator& alloc,
                            const Pointer& p );
    template<typename Pointer>
    static void do_dispose( std::false_type,
                            Allocator& alloc,
                            const Pointer& p );


    template<typename Pointer>
    static void do_dispose_array( std::true_type,
                                  Allocator& alloc,
                                  const Pointer& p,
                                  size_type n );
    template<typename Pointer>
    static void do_dispose_array( std::false_type,
                                  Allocator& alloc,
                                  const Pointer& p,
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

template<typename Allocator>
inline typename bit::memory::allocator_traits<Allocator>::pointer
  bit::memory::allocator_traits<Allocator>::allocate( Allocator& alloc,
                                                      size_type size,
                                                      size_type align )
{
  using impl_type = detail::allocator_traits_impl<Allocator>;

  static constexpr auto tag = allocator_has_allocate<Allocator>{};

  return impl_type::do_allocate( tag , alloc, size, align );
}

template<typename Allocator>
inline typename bit::memory::allocator_traits<Allocator>::pointer
  bit::memory::allocator_traits<Allocator>::allocate( Allocator& alloc,
                                                      const_pointer hint,
                                                      size_type size,
                                                      size_type align )
{
  using impl_type = detail::allocator_traits_impl<Allocator>;

  static constexpr auto tag = allocator_has_allocate_hint<Allocator>{};

  return impl_type::do_allocate_hint( tag , alloc, size, align );
}

//-----------------------------------------------------------------------------

template<typename Allocator>
inline bool bit::memory::allocator_traits<Allocator>
  ::expand( Allocator& alloc,
            pointer p,
            size_type new_size )
{
  using impl_type = detail::allocator_traits_impl<Allocator>;

  static constexpr auto tag = allocator_has_expand<Allocator>{};

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
// Construction
//-----------------------------------------------------------------------------

template<typename Allocator>
template<typename T, typename...Args>
inline void bit::memory::allocator_traits<Allocator>
  ::construct( Allocator& alloc, void* p, Args&&...args )
{
  using impl_type = detail::allocator_traits_impl<Allocator>;

  static constexpr auto tag = allocator_has_construct<Allocator,T,Args...>{};

  impl_type::template do_construct<T>( tag, alloc, p, std::forward<Args>(args)... );
}

template<typename Allocator>
template<typename T, typename...Args>
inline typename bit::memory::detail::allocator_pointer_rebind<Allocator,T>::type
  bit::memory::allocator_traits<Allocator>
  ::make( Allocator& alloc, Args&&...args )
{
  using impl_type = detail::allocator_traits_impl<Allocator>;

  static constexpr auto tag = allocator_has_make<Allocator,T,Args...>{};

  return impl_type::template do_make<T>( tag, alloc, std::forward<Args>(args)... );
}


template<typename Allocator>
template<typename T>
inline typename std::pointer_traits<typename bit::memory::allocator_traits<Allocator>::pointer>::template rebind<T>
  bit::memory::allocator_traits<Allocator>
  ::make_array( Allocator& alloc, size_type n )
{
  using impl_type = detail::allocator_traits_impl<Allocator>;

  static constexpr auto tag = allocator_has_make_array<Allocator,T,size_type>{};

  return impl_type::template do_make_array<T>( tag, alloc, n );
}

template<typename Allocator>
template<typename T>
inline typename std::pointer_traits<typename bit::memory::allocator_traits<Allocator>::pointer>::template rebind<T>
  bit::memory::allocator_traits<Allocator>
  ::make_array( Allocator& alloc, size_type n, const T& copy )
{
  using impl_type = detail::allocator_traits_impl<Allocator>;

  static constexpr auto tag = allocator_has_make_array<Allocator,T,size_type,const T&>{};

  return impl_type::template do_make_array<T>( tag, alloc, n, copy );
}

//-----------------------------------------------------------------------------
// Destruction
//-----------------------------------------------------------------------------

template<typename Allocator>
template<typename T>
void bit::memory::allocator_traits<Allocator>::destroy( Allocator& alloc, T* p )
{
  using impl_type = detail::allocator_traits_impl<Allocator>;

  static constexpr auto tag = allocator_has_destroy<Allocator,T*>{};

  impl_type::do_destroy( tag, alloc, p );
}


template<typename Allocator>
template<typename Pointer, typename>
void bit::memory::allocator_traits<Allocator>
  ::dispose( Allocator& alloc, const Pointer& p )
{
  using type = typename std::pointer_traits<Pointer>::element_type;
  using impl_type = detail::allocator_traits_impl<Allocator>;

  static constexpr auto tag = allocator_has_dispose<Allocator,pointer_rebind<type>>{};

  impl_type::do_dispose( tag, alloc, p );
}

template<typename Allocator>
template<typename Pointer, typename>
void bit::memory::allocator_traits<Allocator>
  ::dispose_array( Allocator& alloc, const Pointer& p, size_type n )
{
  using type = typename std::pointer_traits<Pointer>::element_type;
  using impl_type = detail::allocator_traits_impl<Allocator>;

  static constexpr auto tag = allocator_has_dispose_array<Allocator,pointer_rebind<type>>{};

  impl_type::do_dispose_array( tag, alloc, p, n );
}

//-----------------------------------------------------------------------------
// Observers
//-----------------------------------------------------------------------------

template<typename Allocator>
typename bit::memory::allocator_traits<Allocator>::size_type
  bit::memory::allocator_traits<Allocator>
  ::recommended_allocation_size( const Allocator& alloc, size_type requested )
{
  using impl_type = detail::allocator_traits_impl<Allocator>;

  static constexpr auto tag = allocator_has_recommended_allocation_size<Allocator>{};

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

  static constexpr auto tag = allocator_has_info<Allocator>{};

  return impl_type::do_info( tag, alloc );
}

//-----------------------------------------------------------------------------
// Conversion
//-----------------------------------------------------------------------------

template<typename Allocator>
template<typename T>
inline typename std::pointer_traits<typename bit::memory::allocator_traits<Allocator>::pointer>::template rebind<T>
  bit::memory::allocator_traits<Allocator>::pointer_to( Allocator& alloc, T& x )
  noexcept
{
  using impl_type = detail::allocator_traits_impl<Allocator>;

  static constexpr auto tag = detail::allocator_has_pointer_to<Allocator,T>{};

  return impl_type::do_pointer_to( tag, alloc, x );
}

template<typename Allocator>
template<typename Pointer, typename>
inline typename std::pointer_traits<Pointer>::element_type*
  bit::memory::allocator_traits<Allocator>
  ::to_raw_pointer( Allocator& alloc,
                    const Pointer& p )
  noexcept
{
  using impl_type = detail::allocator_traits_impl<Allocator>;
  static constexpr auto tag = detail::allocator_has_to_raw_pointer<Allocator,Pointer>{};

  return impl_type::do_to_raw_pointer( tag, alloc, p );
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
  static constexpr auto tag = allocator_has_is_unbounded<Allocator>{};

  return impl_type::do_is_unbounded( tag, alloc );
}

template<typename Allocator>
inline typename bit::memory::allocator_traits<Allocator>::size_type
  bit::memory::allocator_traits<Allocator>::max_size( const Allocator& alloc )
  noexcept
{
  using impl_type = detail::allocator_traits_impl<Allocator>;
  static constexpr auto tag = allocator_has_max_size<Allocator>{};

  return impl_type::do_max_size( tag, alloc );
}

template<typename Allocator>
inline typename bit::memory::allocator_traits<Allocator>::size_type
  bit::memory::allocator_traits<Allocator>::min_size( const Allocator& alloc )
  noexcept
{
  using impl_type = detail::allocator_traits_impl<Allocator>;
  static constexpr auto tag = allocator_has_min_size<Allocator>{};

  return impl_type::do_min_size( tag, alloc );
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

  // Assume null allocations are unlikely, since they are the expensive
  // code-path to manage
  if( BIT_MEMORY_UNLIKELY(p == nullptr) ) {
    const auto info = allocator_traits<Allocator>::info( alloc );

    (*get_out_of_memory_handler())(info, size);

    // Invoking the out-of-memory handler must not return
    BIT_MEMORY_UNREACHABLE();
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
inline bool
  bit::memory::detail::allocator_traits_impl<Allocator>
  ::do_expand( std::true_type,
               Allocator& alloc,
               pointer p,
               size_type new_size )
{
  return alloc.expand( p, new_size );
}

template<typename Allocator>
inline bool
  bit::memory::detail::allocator_traits_impl<Allocator>
  ::do_expand( std::false_type,
               Allocator& alloc,
               pointer p,
               size_type new_size )
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

  // round to the next multiple of the alignment
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
// Conversion
//-----------------------------------------------------------------------------

template<typename Allocator>
template<typename T>
inline typename std::pointer_traits<typename bit::memory::allocator_traits<Allocator>::pointer>::template rebind<T>
  bit::memory::detail::allocator_traits_impl<Allocator>
  ::do_pointer_to( std::true_type, Allocator& alloc, T& x )
{
  return alloc.pointer_to(x);
}

template<typename Allocator>
template<typename T>
inline typename std::pointer_traits<typename bit::memory::allocator_traits<Allocator>::pointer>::template rebind<T>
  bit::memory::detail::allocator_traits_impl<Allocator>
  ::do_pointer_to( std::false_type, Allocator& alloc, T& x )
{
  using traits = std::pointer_traits<pointer_rebind<T>>;
  BIT_MEMORY_UNUSED(alloc);

  return traits::pointer_to(x);
}

//-----------------------------------------------------------------------------

template<typename Allocator>
template<typename Pointer>
inline typename std::pointer_traits<Pointer>::element_type*
  bit::memory::detail::allocator_traits_impl<Allocator>
  ::do_to_raw_pointer( std::true_type,
                       Allocator& alloc,
                       const Pointer& p )
{
  return alloc.to_raw_pointer( p );
}

template<typename Allocator>
template<typename Pointer>
inline typename std::pointer_traits<Pointer>::element_type*
  bit::memory::detail::allocator_traits_impl<Allocator>
  ::do_to_raw_pointer( std::false_type,
                       Allocator& alloc,
                       const Pointer& p )
{
  static constexpr auto tag = pointer_traits_has_to_address<Pointer>{};

  // use pointer_traits<T>::to_address if it is defined,
  // otherwise fallback to ADL dispatched to_raw_pointer
  return do_to_raw_pointer_pointer_traits( tag, alloc, p );
}

template<typename Allocator>
template<typename Pointer>
inline typename std::pointer_traits<Pointer>::element_type*
  bit::memory::detail::allocator_traits_impl<Allocator>
  ::do_to_raw_pointer_pointer_traits( std::true_type,
                                      Allocator& alloc,
                                      const Pointer& p )
{
  using traits = std::pointer_traits<Pointer>;
  BIT_MEMORY_UNUSED(alloc);

  return traits::to_address( p );
}

template<typename Allocator>
template<typename Pointer>
inline typename std::pointer_traits<Pointer>::element_type*
  bit::memory::detail::allocator_traits_impl<Allocator>
  ::do_to_raw_pointer_pointer_traits( std::false_type,
                                      Allocator& alloc,
                                      const Pointer& p )
{
  // Dispatch with intentional ADL
  return to_raw_pointer( p );
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
inline typename bit::memory::detail::allocator_pointer_rebind<Allocator,T>::type
  bit::memory::detail::allocator_traits_impl<Allocator>
  ::do_make( std::true_type,
             Allocator& alloc,
             Args&&...args )
{
  return alloc.template make<T>( std::forward<Args>(args)... );
}

template<typename Allocator>
template<typename T, typename...Args>
inline typename bit::memory::detail::allocator_pointer_rebind<Allocator,T>::type
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
inline typename bit::memory::detail::allocator_pointer_rebind<Allocator,T>::type
  bit::memory::detail::allocator_traits_impl<Allocator>
  ::do_make_nothrow( std::true_type,
                     Allocator& alloc,
                     Args&&...args )
  noexcept
{
  using traits = std::pointer_traits<pointer_rebind<T>>;

  auto p = traits_type::allocate( alloc, sizeof(T), alignof(T) );
  auto void_ptr = to_raw_pointer(p);
  auto type_ptr = static_cast<T*>(void_ptr);

  traits_type::template construct<T>( alloc,
                                      void_ptr,
                                      std::forward<Args>(args)... );

  return traits::pointer_to(*type_ptr);
}

template<typename Allocator>
template<typename T, typename...Args>
inline typename bit::memory::detail::allocator_pointer_rebind<Allocator,T>::type
  bit::memory::detail::allocator_traits_impl<Allocator>
  ::do_make_nothrow( std::false_type,
                     Allocator& alloc,
                     Args&&...args )
{
  using traits = std::pointer_traits<pointer_rebind<T>>;

  auto p = traits_type::allocate( alloc, sizeof(T), alignof(T) );
  auto void_ptr = to_raw_pointer(p);
  auto type_ptr = static_cast<T*>(void_ptr);

  try {
    traits_type::template construct<T>( alloc,
                                        void_ptr,
                                        std::forward<Args>(args)... );
  } catch ( ... ) {
    traits_type::deallocate( alloc, p, sizeof(T) );
    throw;
  }
  return traits::pointer_to(*type_ptr);
}

//-----------------------------------------------------------------------------

template<typename Allocator>
template<typename T, typename...Args>
inline typename bit::memory::detail::allocator_pointer_rebind<Allocator,T>::type
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
inline typename bit::memory::detail::allocator_pointer_rebind<Allocator,T>::type
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
inline typename bit::memory::detail::allocator_pointer_rebind<Allocator,T>::type
  bit::memory::detail::allocator_traits_impl<Allocator>
  ::do_make_array_nothrow( std::true_type,
                           Allocator& alloc,
                           std::size_t n,
                           Args&&...args )
  noexcept
{
  auto p        = traits_type::allocate( alloc, sizeof(T)*n, alignof(T) );
  auto void_ptr = traits_type::to_raw_pointer( alloc, p );
  auto type_ptr = static_cast<T*>(void_ptr);
  auto current  = type_ptr;

  const auto end = current + n;

  for( ; current != end; ++current ) {
    traits_type::template construct<T>( alloc, current, std::forward<Args>(args)... );
  }

  return traits_type::pointer_to( alloc, *type_ptr );
}

template<typename Allocator>
template<typename T, typename...Args>
inline typename bit::memory::detail::allocator_pointer_rebind<Allocator,T>::type
  bit::memory::detail::allocator_traits_impl<Allocator>
  ::do_make_array_nothrow( std::false_type,
                           Allocator& alloc,
                           std::size_t n,
                           Args&&...args )
{
  auto p        = traits_type::allocate( alloc, sizeof(T)*n, alignof(T) );
  auto void_ptr = traits_type::to_raw_pointer( alloc, p );
  auto type_ptr = static_cast<T*>(void_ptr);
  auto current  = type_ptr;

  try {
    const auto end = current + n;

    for( ; current != end; ++current ) {
      traits_type::template construct<T>( alloc, current, std::forward<Args>(args)... );
    }
  } catch ( ... ) {
    // If at least one T constructed successfully, destruct all constructed
    // instances in reverse order
    while( current != type_ptr ) {
      traits_type::destroy( alloc, --current );
    }
    throw;
  }

  return traits_type::pointer_to( alloc, *type_ptr );
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
template<typename Pointer>
void bit::memory::detail::allocator_traits_impl<Allocator>
  ::do_dispose( std::true_type,
                Allocator& alloc,
                const Pointer& p )
{
  alloc.dispose( p );
}

template<typename Allocator>
template<typename Pointer>
void bit::memory::detail::allocator_traits_impl<Allocator>
  ::do_dispose( std::false_type,
                Allocator& alloc,
                const Pointer& p )
{
  using type = typename std::pointer_traits<Pointer>::element_type;

  // Default disposal cannot dispose from an abstract type, since the size of
  // the allocation must be calculated by sizeof(T).
  // Custom allocator implementations are free to implement this without
  // requiring the size of the allocation
  static_assert( !std::is_abstract<type>::value,
                 "Cannot dispose from base type" );

  // call the destructor for p
  traits_type::destroy( alloc, traits_type::to_raw_pointer( alloc, p ) );

  // deallocate the memory
  traits_type::deallocate( alloc, pointer(p), sizeof(type) );
}

//-----------------------------------------------------------------------------

template<typename Allocator>
template<typename Pointer>
void bit::memory::detail::allocator_traits_impl<Allocator>
  ::do_dispose_array( std::true_type,
                      Allocator& alloc,
                      const Pointer& p,
                      size_type n )
{
  alloc.dispose_array( p, n );
}

template<typename Allocator>
template<typename Pointer>
void bit::memory::detail::allocator_traits_impl<Allocator>
  ::do_dispose_array( std::false_type,
                      Allocator& alloc,
                      const Pointer& p,
                      size_type n )
{
  using type = typename std::pointer_traits<Pointer>::element_type;
  static_assert( !std::is_abstract<type>::value,
                 "Cannot dispose from base type" );

  // Destroy each entry in the array, then deallocate
  const auto void_ptr = traits_type::to_raw_pointer( alloc, p );
  const auto end = static_cast<type*>(void_ptr);
  auto current   = static_cast<type*>(void_ptr) + n;

  // No attempt has been made to prevent a memory leak if a destructor throws
  // at this point. It is intentionally undefined behavior
  while( current != end ) {
    traits_type::destroy( alloc, --current );
  }

  traits_type::deallocate( alloc, pointer(p), sizeof(type)*n );
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
  return std::numeric_limits<size_type>::max() == traits_type::max_size( alloc );
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

#endif /* BIT_MEMORY_TRAITS_DETAIL_ALLOCATOR_TRAITS_INL */
