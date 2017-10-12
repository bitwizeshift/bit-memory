/**
 * \file fallback_allocator.hpp
 *
 * \brief This header defines an allocator that falls-back on failure to other
 *        allocators
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */
#ifndef BIT_MEMORY_ALLOCATORS_FALLBACK_ALLOCATOR_HPP
#define BIT_MEMORY_ALLOCATORS_FALLBACK_ALLOCATOR_HPP

#include "../detail/ebo_storage.hpp" // detail::ebo_storage

#include "../concepts/Allocator.hpp" // allocator_pointer_t, etc

#include "../macros.hpp"           // BIT_MEMORY_UNUSED
#include "../allocator_traits.hpp" // allocator_traits

#include <tuple>   // std::forward_as_tuple
#include <utility> // std::forward
#include <cstddef> // std::size_t

namespace bit {
  namespace memory {

    ///////////////////////////////////////////////////////////////////////////
    /// \brief
    ///
    ///
    ///////////////////////////////////////////////////////////////////////////
    template<typename...Allocators>
    class fallback_allocator
      : private detail::ebo_storage<Allocators...>
    {

      using base_type = detail::ebo_storage<Allocators...>;

      // TODO(bitwizeshift): Support pretty-pointers by determining the common
      //                     'pointer', 'const_pointer', and 'size_type' of
      //                     each allocator

      //-----------------------------------------------------------------------
      // Constructors
      //-----------------------------------------------------------------------
    public:

      fallback_allocator() = default;

      template<typename...Tuples>
      explicit fallback_allocator( std::piecewise_construct_t, Tuples&&...tuples );

      template<typename Arg0, typename...Args, typename = std::enable_if_t<!std::is_same<std::decay_t<Arg0>,fallback_allocator>::value>>
      explicit fallback_allocator( Arg0&& arg0, Args&&...args );

      fallback_allocator( fallback_allocator&& other ) = default;

      fallback_allocator( const fallback_allocator& other ) = default;

      //-----------------------------------------------------------------------

      fallback_allocator& operator=( fallback_allocator&& other ) = default;

      fallback_allocator& operator=( const fallback_allocator& other ) = default;

      //-----------------------------------------------------------------------
      // Allocations
      //-----------------------------------------------------------------------
    public:

      void* try_allocate( std::size_t size,
                          std::size_t align ) noexcept;

      void deallocate( void* p, std::size_t size );

      //-----------------------------------------------------------------------
      // Observers
      //-----------------------------------------------------------------------
    public:

      bool owns( const void* p ) const noexcept;

      //-----------------------------------------------------------------------
      // Capacity
      //-----------------------------------------------------------------------
    public:

      std::size_t max_size() const noexcept;

      std::size_t min_size() const noexcept;

    private:

      template<std::size_t Idx>
      void* do_try_allocate( std::integral_constant<std::size_t,Idx>,
                             std::size_t size,
                             std::size_t align ) noexcept;

      void* do_try_allocate( std::integral_constant<std::size_t,sizeof...(Allocators)>,
                             std::size_t size,
                             std::size_t align ) noexcept;

      template<std::size_t Idx>
      void* do_deallocate( std::integral_constant<std::size_t,Idx>,
                           void* p,
                           std::size_t size );

      void* do_deallocate( std::integral_constant<std::size_t,sizeof...(Allocators)>,
                           void* p,
                           std::size_t size );


      template<std::size_t...Idxs>
      bool do_owns( std::index_sequence<Idxs...>,
                    const void* p ) const noexcept;

      template<std::size_t...Idxs>
      std::size_t do_max_size( std::index_sequence<Idxs...> ) const noexcept;

      template<std::size_t...Idxs>
      std::size_t do_min_size( std::index_sequence<Idxs...> ) const noexcept;

    };

  } // namespace memory
} // namespace bit

#include "detail/fallback_allocator.inl"

#endif /* BIT_MEMORY_ALLOCATORS_FALLBACK_ALLOCATOR_HPP */
