/**
 * \file polymorphic_allocator_deleter.hpp
 *
 * \brief This header defines an allocator deleter that operates polymorphically
 *        with types, which comes at the cost of storing the size with each
 *        allocation
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */
#ifndef BIT_MEMORY_DELETERS_POLYMORPHIC_ALLOCATOR_DELETER_HPP
#define BIT_MEMORY_DELETERS_POLYMORPHIC_ALLOCATOR_DELETER_HPP

#include "../detail/ebo_storage.hpp"

#include "../concepts/AllocatorStorage.hpp"

#include "../allocator_traits.hpp"
#include "../pointer_utilities.hpp" // to_raw_pointer

#include <cstddef>     // std::size_t, std::ptrdiff_t
#include <tuple>       // std::forward_as_tuple
#include <type_traits> // std::is_reference, std::is_const, etc
#include <utility>     // std::move
#include <memory>      // std::pointer_traits

namespace bit {
  namespace memory {

    ///////////////////////////////////////////////////////////////////////////
    /// \brief A deleter for bit::memory allocators that allows for polymorphic
    ///        conversions, at the cost of storing the size of the allocation
    ///        in the deleter.
    ///
    /// \tparam T the underlying type
    /// \tparam AllocatorStorage the storage for the allocator
    ///////////////////////////////////////////////////////////////////////////
    template<typename T, typename AllocatorStorage>
    class polymorphic_allocator_deleter
      : private detail::ebo_storage<AllocatorStorage>
    {
      using base_type = detail::ebo_storage<AllocatorStorage>;
      using allocator_type = typename AllocatorStorage::allocator_type;

      using alloc_traits   = allocator_traits<allocator_type>;
      using pointer_traits = std::pointer_traits<typename alloc_traits::pointer>;

      static_assert( !std::is_reference<T>::value, "Unable to delete reference type" );
      static_assert( !std::is_const<T>::value, "Unable to delete const type" );
      static_assert( !std::is_void<T>::value, "Unable to delete void type" );
      static_assert( is_allocator_storage<AllocatorStorage>::value,
                     "AllocatorStorage must satisfy AllocatorStorage requirements" );

      //-----------------------------------------------------------------------
      // Public Member Types
      //-----------------------------------------------------------------------
    public:

      using void_pointer    = typename alloc_traits::pointer;
      using pointer         = typename pointer_traits::template rebind<T>;
      using size_type       = typename alloc_traits::size_type;
      using difference_type = typename alloc_traits::difference_type;

      //-----------------------------------------------------------------------
      // Constructor
      //-----------------------------------------------------------------------
    public:

      /// \brief Constructs an polymorphic_allocator_deleter that uses the referenced
      ///        allocator for deletion
      ///
      /// \param storage the allocator storage
      explicit polymorphic_allocator_deleter( AllocatorStorage storage );

      /// \brief Move-constructs an polymorphic_allocator_deleter from an
      ///        existing one
      ///
      /// \param other the other polymorphic_allocator_deleter to move
      polymorphic_allocator_deleter( polymorphic_allocator_deleter&& other ) noexcept = default;

      /// \brief Copy-constructs the polymorphic_allocator_deleter from an
      ///        existing one
      ///
      /// \param other the other polymorphic_allocator_deleter to copy
      polymorphic_allocator_deleter( const polymorphic_allocator_deleter& other ) noexcept = default;

      //-----------------------------------------------------------------------

      template<typename U,
               typename = std::enable_if_t<std::is_convertible<U,T>::value>>
      polymorphic_allocator_deleter( polymorphic_allocator_deleter<U,AllocatorStorage>&& other ) noexcept;

      template<typename U,
               typename = std::enable_if_t<std::is_convertible<U,T>::value>>
      polymorphic_allocator_deleter( const polymorphic_allocator_deleter<U,AllocatorStorage>& other ) noexcept;

      //-----------------------------------------------------------------------

      /// \brief Move-assigns an polymorphic_allocator_deleter from an existing one
      ///
      /// \param other the other polymorphic_allocator_deleter to move
      /// \return reference to \c (*this)
      polymorphic_allocator_deleter&
        operator=( polymorphic_allocator_deleter&& other ) noexcept = default;

      /// \brief Copy-assigns an polymorphic_allocator_deleter from an existing one
      ///
      /// \param other the other polymorphic_allocator_deleter to copy
      /// \return reference to \c (*this)
      polymorphic_allocator_deleter& operator=( const polymorphic_allocator_deleter& other ) noexcept = default;

      //-----------------------------------------------------------------------

      template<typename U,
               typename = std::enable_if_t<std::is_convertible<U,T>::value>>
      polymorphic_allocator_deleter&
        operator=( polymorphic_allocator_deleter<U,AllocatorStorage>&& other ) noexcept;

      template<typename U,
               typename = std::enable_if_t<std::is_convertible<U,T>::value>>
      polymorphic_allocator_deleter&
        operator=( const polymorphic_allocator_deleter<U,AllocatorStorage>& other ) noexcept;

      //-----------------------------------------------------------------------
      // Deallocation
      //-----------------------------------------------------------------------
    public:

      /// \brief Deletes the underlying pointer \p p
      ///
      /// \param p the pointer to delete
      void operator()( pointer p );

      //-----------------------------------------------------------------------
      // Private Members
      //-----------------------------------------------------------------------
    private:

      size_type m_size;

      template<typename,typename> friend class polymorphic_allocator_deleter;

      //-----------------------------------------------------------------------
      // Private Observers
      //-----------------------------------------------------------------------
    private:

      AllocatorStorage storage() const noexcept;

      std::size_t size() const noexcept;
    };


    template<typename T, typename AllocatorStorage>
    class polymorphic_allocator_deleter<T[],AllocatorStorage>
      : private detail::ebo_storage<AllocatorStorage>
    {
      using base_type = detail::ebo_storage<AllocatorStorage>;
      using allocator_type = typename AllocatorStorage::allocator_type;

      using alloc_traits   = allocator_traits<allocator_type>;
      using pointer_traits = std::pointer_traits<typename alloc_traits::pointer>;

      static_assert( !std::is_reference<T>::value, "Unable to delete reference type" );
      static_assert( !std::is_const<T>::value, "Unable to delete const type" );
      static_assert( !std::is_void<T>::value, "Unable to delete void type" );
      static_assert( is_allocator_storage<AllocatorStorage>::value,
                     "AllocatorStorage must satisfy AllocatorStorage requirements" );

      //-----------------------------------------------------------------------
      // Public Member Types
      //-----------------------------------------------------------------------
    public:

      using void_pointer    = typename alloc_traits::pointer;
      using pointer         = typename pointer_traits::template rebind<T>;
      using size_type       = typename alloc_traits::size_type;
      using difference_type = typename alloc_traits::difference_type;

      //-----------------------------------------------------------------------
      // Constructor
      //-----------------------------------------------------------------------
    public:

      /// \brief Constructs an polymorphic_allocator_deleter that uses the referenced
      ///        allocator for deletion
      ///
      /// \param storage the allocator storage
      polymorphic_allocator_deleter( AllocatorStorage storage,
                                     size_type size );

      /// \brief Move-constructs an polymorphic_allocator_deleter from an
      ///        existing one
      ///
      /// \param other the other polymorphic_allocator_deleter to move
      polymorphic_allocator_deleter( polymorphic_allocator_deleter&& other ) noexcept = default;

      /// \brief Copy-constructs the polymorphic_allocator_deleter from an
      ///        existing one
      ///
      /// \param other the other polymorphic_allocator_deleter to copy
      polymorphic_allocator_deleter( const polymorphic_allocator_deleter& other ) noexcept = default;

      // polymorphic conversions of arrays would be UB

      //-----------------------------------------------------------------------

      /// \brief Move-assigns an polymorphic_allocator_deleter from an existing one
      ///
      /// \param other the other polymorphic_allocator_deleter to move
      /// \return reference to \c (*this)
      polymorphic_allocator_deleter&
        operator=( polymorphic_allocator_deleter&& other ) noexcept = default;

      /// \brief Copy-assigns an polymorphic_allocator_deleter from an existing one
      ///
      /// \param other the other polymorphic_allocator_deleter to copy
      /// \return reference to \c (*this)
      polymorphic_allocator_deleter& operator=( const polymorphic_allocator_deleter& other ) noexcept = default;

      //-----------------------------------------------------------------------
      // Deallocation
      //-----------------------------------------------------------------------
    public:

      /// \brief Deletes the underlying pointer \p p
      ///
      /// \param p the pointer to delete
      void operator()( pointer p );

      //-----------------------------------------------------------------------
      // Private Members
      //-----------------------------------------------------------------------
    private:

      size_type m_size;
    };
  } // namespace memory
} // namespace bit

#include "detail/polymorphic_allocator_deleter.inl"

#endif /* BIT_MEMORY_DELETERS_POLYMORPHIC_ALLOCATOR_DELETER_HPP */
