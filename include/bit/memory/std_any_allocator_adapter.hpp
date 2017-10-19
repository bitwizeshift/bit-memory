/**
 * \file std_any_allocator_adapter.hpp
 *
 * \brief A type-erased standard allocator adapter
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */
#ifndef BIT_MEMORY_STD_ANY_ALLOCATOR_ADAPTER_HPP
#define BIT_MEMORY_STD_ANY_ALLOCATOR_ADAPTER_HPP

#include "allocators/any_allocator.hpp" // any_allocator

#include <cstddef>     // std::size_t, std::ptrdiff_t
#include <type_traits> // std::is_reference, std::is_const

namespace bit {
  namespace memory {

    ///////////////////////////////////////////////////////////////////////////
    /// \brief The adapter to make an Allocator behave like a standard
    ///        allocator, while type-erasing the actual allocator
    ///
    /// \tparam T the underlying allocator type
    ///////////////////////////////////////////////////////////////////////////
    template<typename T>
    class std_any_allocator_adapter
    {
      template<typename A>
      using is_enabled = std::integral_constant<bool,
        is_allocator<A>::value && !std::is_same<any_allocator,A>::value
      >;

      static_assert( !std::is_reference<T>::value, "Unable to allocate reference type" );
      static_assert( !std::is_const<T>::value, "Unable to allocate const type" );

      //-----------------------------------------------------------------------
      // Public Member Types
      //-----------------------------------------------------------------------
    public:

      using void_pointer       = void*;
      using const_void_pointer = const void*;
      using size_type          = std::size_t;
      using difference_type    = std::ptrdiff_t;

      using value_type      = T;
      using pointer         = T*;
      using const_pointer   = const T*;
      using reference       = T&;
      using const_reference = const T&;

      template<typename U> struct rebind { using other = std_any_allocator_adapter<U>; };

      //-----------------------------------------------------------------------
      // Constructors
      //-----------------------------------------------------------------------
    public:

      /// \brief Constructs a std_any_allocator_adapter that references the
      ///        underlying allocator
      ///
      /// \param allocator the allocator to reference
      template<typename Allocator, typename = std::enable_if_t<is_enabled<Allocator>::value>>
      std_any_allocator_adapter( Allocator& allocator ) noexcept;

      /// \brief Copy-constructs a std_any_allocator_adapter from an existing one
      ///
      /// \param other the other adapter to copy
      std_any_allocator_adapter( const std_any_allocator_adapter& other ) = default;

      /// \brief Move-constructs a std_any_allocator_adapter from an existing one
      ///
      /// \param other the other adapter to move
      std_any_allocator_adapter( std_any_allocator_adapter&& other ) = default;

      /// \brief Copy-converts a std_any_allocator_adapter from an existing one
      ///
      /// \param other the other adapter to copy-convert
      template<typename U>
      std_any_allocator_adapter( const std_any_allocator_adapter<U>& other ) noexcept;

      /// \brief Move-converts a std_any_allocator_adapter from an existing one
      ///
      /// \param other the other adapter to move-convert
      template<typename U>
      std_any_allocator_adapter( std_any_allocator_adapter<U>&& other ) noexcept;

      //-----------------------------------------------------------------------
      // Allocation
      //-----------------------------------------------------------------------
    public:

      /// \brief Allocates memory using the underlying allocator
      ///
      /// \param n the number of T entries to allocate
      /// \return the pointer to T
      pointer allocate( size_type n );

      /// \brief Deallocates memory using the underlying allocator
      ///
      /// \param p the pointer to deallocate
      /// \param n the number of entries to deallocate
      void deallocate( pointer p, size_type n );

      //-----------------------------------------------------------------------
      // Observer
      //-----------------------------------------------------------------------
    public:

      any_allocator get() const noexcept;

      //-----------------------------------------------------------------------
      // Private Members
      //-----------------------------------------------------------------------
    private:

      any_allocator m_allocator;
    };

  template<typename T>
  class std_any_allocator_adapter<T[]> : public std_any_allocator_adapter<T>{};

  //-------------------------------------------------------------------------
  // Equality Comparisons
  //-------------------------------------------------------------------------

  template<typename T, typename U>
  bool operator==( const std_any_allocator_adapter<T>& lhs,
                   const std_any_allocator_adapter<U>& rhs ) noexcept;

  template<typename T, typename U>
  bool operator!=( const std_any_allocator_adapter<T>& lhs,
                   const std_any_allocator_adapter<U>& rhs ) noexcept;

  } // namespace memory
} // namespace bit

#include "detail/std_any_allocator_adapter.inl"

#endif /* BIT_MEMORY_STD_ANY_ALLOCATOR_ADAPTER_HPP */
