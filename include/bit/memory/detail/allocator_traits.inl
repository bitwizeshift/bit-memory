#ifndef BIT_MEMORY_BLOCK_ALLOCATORS_DETAIL_ALLOCATOR_TRAITS_INL
#define BIT_MEMORY_BLOCK_ALLOCATORS_DETAIL_ALLOCATOR_TRAITS_INL

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

  return do_try_allocate_hint( tag, hint, size, align );
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

  // try_allocate *must* be defined for ExtendedAllocator
  return do_extended_try_allocate_hint( tag, size, align, offset );
}

//-----------------------------------------------------------------------------

template<typename Allocator>
inline typename bit::memory::allocator_traits<Allocator>::pointer
  bit::memory::allocator_traits<Allocator>::allocate( Allocator& alloc,
                                                      size_type size,
                                                      size_type align )
{
  static constexpr auto tag = allocator_has_allocate<Allocator>{};

  return do_allocate( tag , alloc, size, align );
}

template<typename Allocator>
inline typename bit::memory::allocator_traits<Allocator>::pointer
  bit::memory::allocator_traits<Allocator>::allocate( Allocator& alloc,
                                                      const_pointer hint,
                                                      size_type size,
                                                      size_type align )
{
  static constexpr auto tag = allocator_has_allocate_hint<Allocator>{};

  return do_allocate_hint( tag , alloc, size, align );
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

  return do_extended_allocate( tag, alloc, size, align, offset );
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

  return do_extended_allocate_hint( tag, alloc, size, align, offset );
}

//-----------------------------------------------------------------------------

template<typename Allocator>
inline bool bit::memory::allocator_traits<Allocator>
  ::expand( Allocator& alloc,
            pointer p,
            size_type new_size )
{
  static constexpr auto tag = allocator_has_expand<Allocator>{};

  return do_expand( tag, alloc, p, new_size );
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
  do_recommended_allocation_size( tag, alloc, requested );
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
  return do_info( allocator_has_info<Allocator>{}, alloc );
}

//-----------------------------------------------------------------------------
// Construction
//-----------------------------------------------------------------------------

template<typename Allocator>
template<typename T, typename...Args>
inline T* bit::memory::allocator_traits<Allocator>
  ::try_make( Allocator& alloc, Args&&...args )
  noexcept( std::is_nothrow_constructible<T,Args...>::value )
{
  static const auto tag = std::is_nothrow_constructible<T,Args...>{};
  return do_try_make<T>( tag, alloc, std::forward<Args>(args)... );
}

template<typename Allocator>
template<typename T>
inline T* bit::memory::allocator_traits<Allocator>
  ::try_make_array( Allocator& alloc, size_type n )
  noexcept( std::is_nothrow_default_constructible<T>::value )
{
  static const auto tag = std::is_nothrow_constructible<T>{};
  return do_try_make_array<T>( tag, alloc, n );
}

template<typename Allocator>
template<typename T>
inline T* bit::memory::allocator_traits<Allocator>
  ::try_make_array( Allocator& alloc, size_type n, const T& copy )
  noexcept( std::is_nothrow_copy_constructible<T>::value )
{
  static const auto tag = std::is_nothrow_copy_constructible<T>{};
  return do_try_make_array<T>( tag, alloc, n, copy );
}

//-----------------------------------------------------------------------------

template<typename Allocator>
template<typename T, typename...Args>
inline T* bit::memory::allocator_traits<Allocator>
  ::make( Allocator& alloc, Args&&...args )
{
  static const auto tag = std::is_nothrow_constructible<T,Args...>{};
  return do_make<T>( tag, alloc, std::forward<Args>(args)... );
}


template<typename Allocator>
template<typename T>
inline T* bit::memory::allocator_traits<Allocator>
  ::make_array( Allocator& alloc, size_type n )
{
  static const auto tag = std::is_nothrow_constructible<T>{};
  return do_make_array<T>( tag, alloc, n );
}

template<typename Allocator>
template<typename T>
inline T* bit::memory::allocator_traits<Allocator>
  ::make_array( Allocator& alloc, size_type n, const T& copy )
{
  static const auto tag = std::is_nothrow_copy_constructible<T>{};
  return do_make_array<T>( tag, alloc, n, copy );
}

//-----------------------------------------------------------------------------
// Destruction
//-----------------------------------------------------------------------------

template<typename Allocator>
template<typename T>
void bit::memory::allocator_traits<Allocator>
  ::dispose( Allocator& alloc,  T* p )
{
  static_assert( !std::is_abstract<T>::value,
                 "Cannot deallocate/destroy from base type" );

  destroy_at( p );
  deallocate( alloc, p, sizeof(T) );
}

template<typename Allocator>
template<typename T>
void bit::memory::allocator_traits<Allocator>
  ::dispose_array( Allocator& alloc,  T* p, size_type n )
{
  destroy_array_at( p, n );
  deallocate( alloc, p, sizeof(T)*n );
}

//-----------------------------------------------------------------------------
// Capacity
//-----------------------------------------------------------------------------

template<typename Allocator>
inline bool
  bit::memory::allocator_traits<Allocator>::is_unbounded( const Allocator& alloc )
  noexcept
{
  return do_is_unbounded( allocator_has_is_unbounded<Allocator>{}, alloc );
}

template<typename Allocator>
inline typename bit::memory::allocator_traits<Allocator>::size_type
  bit::memory::allocator_traits<Allocator>::max_size( const Allocator& alloc )
  noexcept
{
  return do_max_size( allocator_has_max_size<Allocator>{}, alloc );
}

template<typename Allocator>
inline typename bit::memory::allocator_traits<Allocator>::size_type
  bit::memory::allocator_traits<Allocator>::min_size( const Allocator& alloc )
  noexcept
{
  return do_min_size( allocator_has_min_size<Allocator>{}, alloc );
}

//=============================================================================
// Private Implementation
//=============================================================================

//-----------------------------------------------------------------------------
// Try Allocation With Hints
//-----------------------------------------------------------------------------

template<typename Allocator>
inline typename bit::memory::allocator_traits<Allocator>::pointer
  bit::memory::allocator_traits<Allocator>
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
  bit::memory::allocator_traits<Allocator>
  ::do_try_allocate_hint( std::false_type,
                          Allocator& alloc,
                          const_pointer hint,
                          size_type size,
                          size_type align )
{
  BIT_MEMORY_UNUSED(hint);

  return allocator_traits::try_allocate( alloc, size, align );
}

//-----------------------------------------------------------------------------

template<typename Allocator>
inline typename bit::memory::allocator_traits<Allocator>::pointer
  bit::memory::allocator_traits<Allocator>
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
  bit::memory::allocator_traits<Allocator>
  ::do_extended_try_allocate_hint( std::false_type,
                                   Allocator& alloc,
                                   const_pointer hint,
                                   size_type size,
                                   size_type align,
                                   size_type offset )
{
  BIT_MEMORY_UNUSED(hint);

  return allocator_traits::try_allocate( alloc, size, align, offset );
}


//-----------------------------------------------------------------------------
// Allocation
//-----------------------------------------------------------------------------

template<typename Allocator>
inline typename bit::memory::allocator_traits<Allocator>::pointer
  bit::memory::allocator_traits<Allocator>::do_allocate( std::true_type,
                                                         Allocator& alloc,
                                                         size_type size,
                                                         size_type align )
{
  return alloc.allocate( size, align );
}

template<typename Allocator>
inline typename bit::memory::allocator_traits<Allocator>::pointer
  bit::memory::allocator_traits<Allocator>::do_allocate( std::false_type,
                                                         Allocator& alloc,
                                                         size_type size,
                                                         size_type align )
{
  auto p = try_allocate(alloc,size,align);

  if( BIT_MEMORY_UNLIKELY(p == nullptr) ) {
    const auto info = allocator_traits<Allocator>::info( alloc );

    get_out_of_memory_handler()(info, size);
  }

  return p;
}

//-----------------------------------------------------------------------------

template<typename Allocator>
inline typename bit::memory::allocator_traits<Allocator>::pointer
  bit::memory::allocator_traits<Allocator>
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
  bit::memory::allocator_traits<Allocator>
  ::do_extended_allocate( std::false_type,
                          Allocator& alloc,
                          size_type size,
                          size_type align,
                          size_type offset )
{
  auto p = try_allocate( alloc, size, align, offset );

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
  bit::memory::allocator_traits<Allocator>
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
  bit::memory::allocator_traits<Allocator>
  ::do_allocate_hint( std::false_type,
                      Allocator& alloc,
                      const_pointer hint,
                      size_type size,
                      size_type align )
{
  BIT_MEMORY_UNUSED(hint);

  return allocator_traits::allocate( alloc, size, align );
}

//-----------------------------------------------------------------------------

template<typename Allocator>
inline typename bit::memory::allocator_traits<Allocator>::pointer
  bit::memory::allocator_traits<Allocator>
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
  bit::memory::allocator_traits<Allocator>
  ::do_extended_allocate_hint( std::false_type,
                               Allocator& alloc,
                               const_pointer hint,
                               size_type size,
                               size_type align,
                               size_type offset )
{
  BIT_MEMORY_UNUSED(hint);

  return allocator_traits::allocate( alloc, size, align, offset );
}

//-----------------------------------------------------------------------------

template<typename Allocator>
inline bool
  bit::memory::allocator_traits<Allocator>::do_expand( std::true_type,
                                                       Allocator& alloc,
                                                       pointer p,
                                                       size_type new_size)
{
  return alloc.expand( p, new_size );
}

template<typename Allocator>
inline bool
  bit::memory::allocator_traits<Allocator>::do_expand( std::false_type,
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
// Private Observers
//-----------------------------------------------------------------------------

template<typename Allocator>
inline typename bit::memory::allocator_traits<Allocator>::size_type
  bit::memory::allocator_traits<Allocator>
  ::do_recommended_allocation_size( std::true_type,
                                    const Allocator& alloc,
                                    size_type requested )
{
  return alloc.recommended_allocation_size( requested );
}

template<typename Allocator>
inline typename bit::memory::allocator_traits<Allocator>::size_type
  bit::memory::allocator_traits<Allocator>
  ::do_recommended_allocation_size( std::false_type,
                                    const Allocator& alloc,
                                    size_type requested )
{
  BIT_MEMORY_UNUSED(alloc);

  static constexpr auto multiple = default_alignment::value;

  return ((requested + multiple - 1) & ~multiple);
}

//-----------------------------------------------------------------------------

template<typename Allocator>
inline bit::memory::allocator_info
  bit::memory::allocator_traits<Allocator>::do_info( std::true_type,
                                                     const Allocator& alloc )
{
  return alloc.info();
}

template<typename Allocator>
inline bit::memory::allocator_info
  bit::memory::allocator_traits<Allocator>::do_info( std::false_type,
                                                     const Allocator& alloc )
{
  BIT_MEMORY_UNUSED(alloc);

  return {typeid(Allocator).name(),std::addressof(alloc)};
}

//-----------------------------------------------------------------------------
// Private Construction
//-----------------------------------------------------------------------------

template<typename Allocator>
template<typename T, typename...Args>
inline T* bit::memory::allocator_traits<Allocator>
  ::do_try_make( std::true_type,
                 Allocator& alloc,
                 Args&&...args )
  noexcept
{
  auto p = try_allocate( alloc, sizeof(T), alignof(T) );

  if( BIT_MEMORY_UNLIKELY(p == nullptr) ) return nullptr;

  uninitialized_construct_at<T>( p, std::forward<Args>(args)... );
  return static_cast<T*>(p);
}

template<typename Allocator>
template<typename T, typename...Args>
inline T* bit::memory::allocator_traits<Allocator>
  ::do_try_make( std::false_type,
                 Allocator& alloc,
                 Args&&...args )
{
  auto p = try_allocate( alloc, sizeof(T), alignof(T) );

  if( BIT_MEMORY_UNLIKELY(p == nullptr) ) return nullptr;

  try {
    uninitialized_construct_at<T>( p, std::forward<Args>(args)... );
  } catch ( ... ) {
    deallocate( p, sizeof(T) );
    throw;
  }
  return static_cast<T*>(p);
}

//-----------------------------------------------------------------------------

template<typename Allocator>
template<typename T, typename...Args>
inline T* bit::memory::allocator_traits<Allocator>
  ::do_try_make_array( std::true_type,
                       Allocator& alloc,
                       std::size_t n,
                       Args&&...args )
  noexcept
{
  auto p = try_allocate( alloc, sizeof(T)*n, alignof(T) );

  if( BIT_MEMORY_UNLIKELY(p == nullptr) ) return nullptr;

  uninitialized_construct_array_at<T>( p, n, std::forward<Args>(args)... );
  return static_cast<T*>(p);
}

template<typename Allocator>
template<typename T, typename...Args>
inline T* bit::memory::allocator_traits<Allocator>
  ::do_try_make_array( std::false_type,
                       Allocator& alloc,
                       std::size_t n,
                       Args&&...args )
{
  auto p = try_allocate( alloc, sizeof(T)*n, alignof(T) );

  if( BIT_MEMORY_UNLIKELY(p == nullptr) ) return nullptr;

  try {
    uninitialized_construct_array_at<T>( p, n, std::forward<Args>(args)... );
  } catch ( ... ) {
    deallocate( p, sizeof(T)*n );
    throw;
  }
  return static_cast<T*>(p);
}

//-----------------------------------------------------------------------------

template<typename Allocator>
template<typename T, typename...Args>
inline T* bit::memory::allocator_traits<Allocator>
  ::do_make( std::true_type,
             Allocator& alloc,
             Args&&...args )
{
  auto p = allocate( alloc, sizeof(T), alignof(T) );

  return uninitialized_construct_at<T>( p, std::forward<Args>(args)... );
}

template<typename Allocator>
template<typename T, typename...Args>
inline T* bit::memory::allocator_traits<Allocator>
  ::do_make( std::false_type,
             Allocator& alloc,
             Args&&...args )
{
  auto p = allocate( alloc, sizeof(T), alignof(T) );

  try {
    uninitialized_construct_at<T>( p, std::forward<Args>(args)... );
  } catch ( ... ) {
    deallocate( p, sizeof(T) );
    throw;
  }
  return static_cast<T*>(p);
}

//-----------------------------------------------------------------------------

template<typename Allocator>
template<typename T, typename...Args>
inline T* bit::memory::allocator_traits<Allocator>
  ::do_make_array( std::true_type,
                   Allocator& alloc,
                   std::size_t n,
                   Args&&...args )
{
  auto p = allocate( alloc, sizeof(T)*n, alignof(T) );

  uninitialized_construct_array_at<T>( p, n, std::forward<Args>(args)... );
  return static_cast<T*>(p);
}

template<typename Allocator>
template<typename T, typename...Args>
inline T* bit::memory::allocator_traits<Allocator>
  ::do_make_array( std::false_type,
                   Allocator& alloc,
                   std::size_t n,
                   Args&&...args )
{
  auto p = allocate( alloc, sizeof(T)*n, alignof(T) );

  try {
    uninitialized_construct_array_at<T>( p, n, std::forward<Args>(args)... );
  } catch ( ... ) {
    deallocate( p, sizeof(T)*n );
    throw;
  }
  return static_cast<T*>(p);
}

//-----------------------------------------------------------------------------
// Private Capacity
//-----------------------------------------------------------------------------

template<typename Allocator>
inline bool
  bit::memory::allocator_traits<Allocator>
  ::do_is_unbounded( std::true_type, const Allocator& alloc )
{
  return alloc.is_unbounded();
}

template<typename Allocator>
inline bool
  bit::memory::allocator_traits<Allocator>
  ::do_is_unbounded( std::false_type, const Allocator& alloc )
{
  return std::numeric_limits<size_type>::max() == max_size( alloc );
}

//-----------------------------------------------------------------------------

template<typename Allocator>
inline typename bit::memory::allocator_traits<Allocator>::size_type
  bit::memory::allocator_traits<Allocator>::do_max_size( std::true_type,
                                                         const Allocator& alloc )
{
  return alloc.max_size();
}

template<typename Allocator>
inline typename bit::memory::allocator_traits<Allocator>::size_type
  bit::memory::allocator_traits<Allocator>::do_max_size( std::false_type,
                                                         const Allocator& alloc )
{
  BIT_MEMORY_UNUSED(alloc);

  return std::numeric_limits<size_type>::max();
}

//-----------------------------------------------------------------------------

template<typename Allocator>
inline typename bit::memory::allocator_traits<Allocator>::size_type
  bit::memory::allocator_traits<Allocator>::do_min_size( std::true_type,
                                                         const Allocator& alloc )
{
  return alloc.min_size();
}

template<typename Allocator>
inline typename bit::memory::allocator_traits<Allocator>::size_type
  bit::memory::allocator_traits<Allocator>::do_min_size( std::false_type,
                                                         const Allocator& alloc )
{
  BIT_MEMORY_UNUSED(alloc);

  return 1;
}

#endif /* BIT_MEMORY_BLOCK_ALLOCATORS_DETAIL_ALLOCATOR_TRAITS_INL */
