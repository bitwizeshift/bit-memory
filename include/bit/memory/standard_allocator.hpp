/**
 * \file standard_allocator.hpp
 *
 * \brief This header defines an adapter around bit::memory allocators that
 *        make them behave like the c++ standard's "Allocator" concept
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */
#ifndef BIT_MEMORY_STANDARD_ALLOCATOR_HPP
#define BIT_MEMORY_STANDARD_ALLOCATOR_HPP

#include "detail/ebo_storage.hpp" // detail::ebo_storage

#include "concepts/AllocatorStorage.hpp" // is_allocator_storage
#include "concepts/Stateless.hpp"        // is_stateless

#include "allocator_traits.hpp" // allocator_traits

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
    /// This requires an instance of an AllocatorStorage type to hold onto the
    /// underlying allocator, and possibly contribute to the lifetime
    /// (depending on the storage policy)
    ///
    /// \tparam T the underlying allocator type
    /// \tparam AllocatorStorage the storage that holds the allocator
    ///////////////////////////////////////////////////////////////////////////
    template<typename T, typename AllocatorStorage>
    class standard_allocator
      : private detail::ebo_storage<AllocatorStorage>
    {
      using base_type = detail::ebo_storage<AllocatorStorage>;

      using allocator_type = typename AllocatorStorage::allocator_type;
      using alloc_traits   = allocator_traits<allocator_type>;
      using pointer_traits = std::pointer_traits<typename alloc_traits::pointer>;

      static_assert( !std::is_reference<T>::value, "Unable to allocate reference type" );
      static_assert( !std::is_const<T>::value, "Unable to allocate const type" );
      static_assert( !std::is_void<T>::value, "Unable to delete void type" );
      static_assert( is_allocator_storage<AllocatorStorage>::value,
                     "AllocatorStorage must satisfy AllocatorStorage requirements" );

      //-----------------------------------------------------------------------
      // Public Member Types
      //-----------------------------------------------------------------------
    public:

      using void_pointer       = typename alloc_traits::pointer;
      using const_void_pointer = typename alloc_traits::const_pointer;
      using size_type          = typename alloc_traits::size_type;
      using difference_type    = typename alloc_traits::difference_type;

      using value_type      = T;
      using pointer         = typename std::pointer_traits<void_pointer>::template rebind<T>;
      using const_pointer   = typename std::pointer_traits<const_void_pointer>::template rebind<const T>;
      using reference       = T&;
      using const_reference = const T&;

      using propagate_on_container_copy_assignment
        = typename alloc_traits::propagate_on_container_copy_assignment;
      using propagate_on_container_move_assignment
        = typename alloc_traits::propagate_on_container_move_assignment;
      using is_always_equal = is_stateless<allocator_type>;

      template<typename U>
      struct rebind
      {
        using other = standard_allocator<U,AllocatorStorage>;
      };

      //-----------------------------------------------------------------------
      // Constructors / Assignment
      //-----------------------------------------------------------------------
    public:

      /// \brief Constructs a standard_allocator that references the
      ///        underlying allocator
      ///
      /// \param storage the allocator storage
      explicit standard_allocator( AllocatorStorage storage ) noexcept;

      /// \brief Copy-constructs a std_allocator_adapter from an existing one
      ///
      /// \param other the other adapter to copy
      standard_allocator( const standard_allocator& other ) noexcept = default;

      /// \brief Move-constructs a std_allocator_adapter from an existing one
      ///
      /// \param other the other adapter to move
      standard_allocator( standard_allocator&& other ) noexcept = default;

      /// \brief Copy-converts a std_allocator_adapter from an existing one
      ///
      /// \param other the other adapter to copy-convert
      template<typename U, typename UAllocatorStorage,
               typename = std::enable_if_t<std::is_convertible<UAllocatorStorage,AllocatorStorage>::value>>
      standard_allocator( const standard_allocator<U,UAllocatorStorage>& other ) noexcept;

      /// \brief Move-converts a std_allocator_adapter from an existing one
      ///
      /// \param other the other adapter to move-convert
      template<typename U, typename UAllocatorStorage,
               typename = std::enable_if_t<std::is_convertible<UAllocatorStorage,AllocatorStorage>::value>>
      standard_allocator( standard_allocator<U,UAllocatorStorage>&& other ) noexcept;

      //-----------------------------------------------------------------------

      /// \brief Move-assigns another standard allocator to this one
      ///
      /// \param other the other allocator
      /// \return reference to \c (*this)
      standard_allocator& operator=( standard_allocator&& other ) noexcept = default;

      /// \brief Copy-assigns another standard allocator to this one
      ///
      /// \param other the other allocator
      /// \return reference to \c (*this)
      standard_allocator& operator=( const standard_allocator& other ) noexcept = default;

      //-----------------------------------------------------------------------
      // Allocation / Deallocation
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
    public:

      /// \brief Gets the maximum size this allocator supports
      ///
      /// \return the maximum size able to be allocated
      template<typename U = AllocatorStorage, typename = std::enable_if<allocator_has_max_size<U>::value>>
      size_type max_size() const noexcept;

      /// \{
      /// \brief Gets the underlying allocator
      ///
      /// \return a reference to the underlying allocator
      allocator_type& get_underlying() noexcept;
      const allocator_type& get_underlying() const noexcept;
      /// \}

      //-----------------------------------------------------------------------
      // Private Observers
      //-----------------------------------------------------------------------
    private:

      /// \brief Gets the storage used for this allocator
      ///
      /// This is an implementation detail required for the converting
      /// constructors
      ///
      /// \return the allocator storage
      AllocatorStorage storage() const;

      template<typename, typename> friend class standard_allocator;
    };

    //-------------------------------------------------------------------------
    // Utilities
    //-------------------------------------------------------------------------

    /// \brief Makes an allocator adapter from a given allocator
    ///
    /// \param allocator an allocator that satisifes the Allocator requirement
    /// \return the adapted allocator
    template<typename T, typename AllocatorStorage>
    standard_allocator<T,AllocatorStorage>
      make_standard_allocator( AllocatorStorage allocator ) noexcept;

    //-------------------------------------------------------------------------
    // Equality Comparisons
    //-------------------------------------------------------------------------

    template<typename T1, typename T2, typename AllocatorStorage>
    bool operator==( const standard_allocator<T1,AllocatorStorage>& lhs,
                     const standard_allocator<T2,AllocatorStorage>& rhs ) noexcept;

    template<typename T1, typename T2, typename AllocatorStorage>
    bool operator!=( const standard_allocator<T1,AllocatorStorage>& lhs,
                     const standard_allocator<T2,AllocatorStorage>& rhs ) noexcept;

  } // namespace memory
} // namespace bit

#include "detail/standard_allocator.inl"

#endif /* BIT_MEMORY_STANDARD_ALLOCATOR_HPP */
