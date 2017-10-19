/**
 * \file std_allocator_adapter.hpp
 *
 * \brief This header contains an implementation of a standard allocator
 *        adapter, used to adapt bit::memory allocators into standard
 *        allocators.
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */
#ifndef BIT_MEMORY_STD_ALLOCATOR_ADAPTER_HPP
#define BIT_MEMORY_STD_ALLOCATOR_ADAPTER_HPP

#include "detail/ebo_storage.hpp" // detail::ebo_storage

#include "concepts/Allocator.hpp" // detail::allocator_has_max_size

#include "allocator_reference.hpp"              // allocator_reference
#include "allocator_traits.hpp"                 // allocator_traits

#include <cstddef>     // std::size_t, std::ptrdiff_t
#include <type_traits> // std::is_reference, std::is_const, etc
#include <memory>      // std::pointer_traits
#include <tuple>       // std::forward_as_tuple

namespace bit {
  namespace memory {

    ///////////////////////////////////////////////////////////////////////////
    /// \brief The adapter to make an Allocator behave like a standard
    ///        allocator
    ///
    /// \tparam T the underlying allocator type
    /// \tparam Allocator the underlying allocator that satisfies the
    ///         Allocator requirements
    ///////////////////////////////////////////////////////////////////////////
    template<typename T, typename Allocator>
    class std_allocator_adapter
      : private detail::ebo_storage<allocator_reference<Allocator>>
    {
      using base_type = detail::ebo_storage<allocator_reference<Allocator>>;
      using traits_type = allocator_traits<allocator_reference<Allocator>>;

      static_assert( !std::is_reference<T>::value, "Unable to allocate reference type" );
      static_assert( !std::is_const<T>::value, "Unable to allocate const type" );
      static_assert( is_allocator<Allocator>::value, "Allocator must satisfy Allocator requirements" );

      //-----------------------------------------------------------------------
      // Public Member Types
      //-----------------------------------------------------------------------
    public:

      using void_pointer       = typename traits_type::pointer;
      using const_void_pointer = typename traits_type::const_pointer;
      using size_type          = typename traits_type::size_type;
      using difference_type    = typename traits_type::difference_type;

      using value_type      = T;
      using pointer         = typename std::pointer_traits<void_pointer>::template rebind<T>;
      using const_pointer   = typename std::pointer_traits<const_void_pointer>::template rebind<const T>;
      using reference       = T&;
      using const_reference = const T&;

      template<typename U> struct rebind { using other = std_allocator_adapter<U,Allocator>; };

      using is_always_equal = typename traits_type::is_stateless;

      //-----------------------------------------------------------------------
      // Constructors
      //-----------------------------------------------------------------------
    public:

      /// \brief Constructs a std_allocator_adapter that references the
      ///        underlying allocator
      ///
      /// \param allocator the allocator to reference
      explicit std_allocator_adapter( Allocator& allocator ) noexcept;

      /// \brief Copy-constructs a std_allocator_adapter from an existing one
      ///
      /// \param other the other adapter to copy
      std_allocator_adapter( const std_allocator_adapter& other ) = default;

      /// \brief Move-constructs a std_allocator_adapter from an existing one
      ///
      /// \param other the other adapter to move
      std_allocator_adapter( std_allocator_adapter&& other ) = default;

      /// \brief Copy-converts a std_allocator_adapter from an existing one
      ///
      /// \param other the other adapter to copy-convert
      template<typename U>
      std_allocator_adapter( const std_allocator_adapter<U,Allocator>& other ) noexcept;

      /// \brief Move-converts a std_allocator_adapter from an existing one
      ///
      /// \param other the other adapter to move-convert
      template<typename U>
      std_allocator_adapter( std_allocator_adapter<U,Allocator>&& other ) noexcept;

      //-----------------------------------------------------------------------
      // Allocation
      //-----------------------------------------------------------------------
    public:

      /// \brief Allocates memory using the underlying Allocator
      ///
      /// \param n the number of T entries to allocate
      /// \return the pointer to T
      pointer allocate( size_type n );

      /// \brief Deallocates memory using the underlying Allocator
      ///
      /// \param p the pointer to deallocate
      /// \param n the number of entries to deallocate
      void deallocate( pointer p, size_type n );

      //-----------------------------------------------------------------------
      // Observers
      //-----------------------------------------------------------------------

      /// \brief Gets the maximum size this allocator supports
      ///
      /// \return the maximum size able to be allocated
      template<typename U = Allocator, typename = std::enable_if<allocator_has_max_size<U>::value>>
      size_type max_size() const noexcept;

      /// \brief Gets the underlying allocator
      ///
      /// \return a reference to the underlying allocator
      Allocator& get() const noexcept;
    };

    template<typename T, typename Allocator>
    class std_allocator_adapter<T[],Allocator>
      : public std_allocator_adapter<T,Allocator>{};

    //-------------------------------------------------------------------------
    // Utilities
    //-------------------------------------------------------------------------

    /// \brief Makes an allocator adapter from a given allocator
    ///
    /// \param allocator an allocator that satisifes the Allocator requirement
    /// \return the adapted allocator
    template<typename T, typename Allocator>
    std_allocator_adapter<T,Allocator>
      make_allocator_adapter( Allocator& allocator ) noexcept;

    //-------------------------------------------------------------------------
    // Equality Comparisons
    //-------------------------------------------------------------------------

    template<typename T1, typename T2, typename Allocator>
    bool operator==( const std_allocator_adapter<T1,Allocator>& lhs,
                     const std_allocator_adapter<T2,Allocator>& rhs ) noexcept;

    template<typename T1, typename T2, typename Allocator>
    bool operator!=( const std_allocator_adapter<T1,Allocator>& lhs,
                     const std_allocator_adapter<T2,Allocator>& rhs ) noexcept;

  } // namespace memory
} // namespace bit

#include "detail/std_allocator_adapter.inl"

#endif /* BIT_MEMORY_STD_ALLOCATOR_ADAPTER_HPP */
