/**
 * \file standard_allocator.hpp
 *
 * \brief This header contains an implementation of a standard allocator
 *        adapter
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */

#ifndef BIT_MEMORY_STD_ALLOCATOR_ADAPTER_HPP
#define BIT_MEMORY_STD_ALLOCATOR_ADAPTER_HPP

#include "allocator_traits.hpp"

#include <type_traits> // std::is_reference, std::is_const, etc
#include <cstddef>     // std::size_t, std::ptrdiff_t

namespace bit {
  namespace memory {

    //////////////////////////////////////////////////////////////////////////
    /// \brief The adapter to make an Allocator behave like a standard
    ///        allocator
    ///
    /// \tparam T the underlying allocator type
    /// \tparam Allocator the underlying allocator that satisfies the
    ///         Allocator requirements
    //////////////////////////////////////////////////////////////////////////
    template<typename T, typename Allocator>
    class std_allocator_adapter
    {
      static_assert( !std::is_reference<T>::value, "Unable to allocate reference type" );
      static_assert( !std::is_const<T>::value, "Unable to allocate const type" );
      static_assert( is_allocator<Allocator>::value, "Allocator must satisfy Allocator requirements" );

      //----------------------------------------------------------------------
      // Public Member Types
      //----------------------------------------------------------------------
    public:

      using value_type      = T;
      using pointer         = T*;
      using const_pointer   = const T*;
      using reference       = T&;
      using const_reference = const T&;
      using size_type       = std::size_t;
      using difference_type = std::ptrdiff_t;

      template<typename U> struct rebind { using other = std_allocator_adapter<U,Allocator>; };

      //----------------------------------------------------------------------
      // Constructors
      //----------------------------------------------------------------------
    public:

      explicit std_allocator_adapter( Allocator& allocator ) noexcept;

      std_allocator_adapter( const std_allocator_adapter& allocator )
        noexcept = default;

      std_allocator_adapter( std_allocator_adapter&& allocator )
        noexcept = default;

      template<typename U>
      std_allocator_adapter( const std_allocator_adapter<U,Allocator>& ) noexcept;

      template<typename U>
      std_allocator_adapter( std_allocator_adapter<U,Allocator>&& ) noexcept;

      //----------------------------------------------------------------------
      // Allocation
      //----------------------------------------------------------------------
    public:

      /// \brief Allocates memory using the underlying Allocator
      ///
      /// \param n the number of T entries to allocate
      /// \return the pointer to T
      T* allocate( std::size_t n );

      /// \brief Deallocates memory using the underlying Allocator
      ///
      /// \param p the pointer to deallocate
      /// \param n the number of entries to deallocate
      void deallocate( T* p, std::size_t n );

      //----------------------------------------------------------------------
      // Observers
      //----------------------------------------------------------------------

      /// \brief Gets the underlying allocator
      ///
      /// \return a reference to the underlying allocator
      Allocator& get() const noexcept;

      //----------------------------------------------------------------------
      // Private Members
      //----------------------------------------------------------------------
    private:

      Allocator* m_instance; ///< The underlying allocator
    };

    //------------------------------------------------------------------------
    // Utilities
    //------------------------------------------------------------------------

    /// \brief Makes an allocator adapter from a given allocator
    ///
    /// \param allocator an allocator that satisifes the Allocator requirement
    /// \return the adapted allocator
    template<typename T, typename Allocator>
    std_allocator_adapter<T,Allocator>
      make_allocator_adapter( Allocator& allocator ) noexcept;

    //------------------------------------------------------------------------
    // Equality Comparisons
    //------------------------------------------------------------------------

    template<typename T1, typename T2, typename Allocator>
    bool operator==( const std_allocator_adapter<T1,Allocator>& lhs,
                     const std_allocator_adapter<T2,Allocator>& rhs ) noexcept;

    template<typename T1, typename T2, typename Allocator1, typename Allocator2>
    bool operator==( const std_allocator_adapter<T1,Allocator1>& lhs,
                     const std_allocator_adapter<T2,Allocator2>& rhs ) noexcept;

    template<typename T1, typename T2, typename Allocator>
    bool operator!=( const std_allocator_adapter<T1,Allocator>& lhs,
                     const std_allocator_adapter<T2,Allocator>& rhs ) noexcept;

    template<typename T1, typename T2, typename Allocator1, typename Allocator2>
    bool operator!=( const std_allocator_adapter<T1,Allocator1>& lhs,
                     const std_allocator_adapter<T2,Allocator2>& rhs ) noexcept;

  } // namespace memory
} // namespace bit

#include "detail/std_allocator_adapter.inl"

#endif /* BIT_MEMORY_STD_ALLOCATOR_ADAPTER_HPP */
