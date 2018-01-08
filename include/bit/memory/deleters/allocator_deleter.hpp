/**
 * \file allocator_deleter.hpp
 *
 * \brief This header contains the definition of an allocator deleter for use
 *        with smart pointer types
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */
#ifndef BIT_MEMORY_DELETERS_ALLOCATOR_DELETER_HPP
#define BIT_MEMORY_DELETERS_ALLOCATOR_DELETER_HPP

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
    /// \brief A deleter for custom allocators in the library.
    ///
    /// For any stateless allocator, this leverages EBO to avoid the need for
    /// storage.
    ///
    /// \note Unlike std::default_delete, this deleter does not have a
    ///       converting constructor that allows for
    ///       'allocator_deleter<Derived,...> -> allocator_deleter<Base,...>'
    ///       since the size of the deallocation is computed based on the size
    ///       of \c T. To support polymorphism with deleters, see
    ///       \ref polymorphic_allocator_deleter which always incurs the size
    ///       cost.
    ///
    /// \satisfies{Deleter}
    ///
    /// \tparam T the type being deallocated
    /// \tparam Allocator the allocator to deallocate with
    ///////////////////////////////////////////////////////////////////////////
    template<typename T, typename AllocatorStorage>
    class allocator_deleter
      : private detail::ebo_storage<AllocatorStorage>
    {
      using base_type = detail::ebo_storage<AllocatorStorage>;
      using allocator_type = typename AllocatorStorage::allocator_type;

      using alloc_traits   = allocator_traits<allocator_type>;
      using pointer_traits = std::pointer_traits<typename alloc_traits::pointer>;

      static_assert( !std::is_reference<T>::value, "Unable to delete reference type" );
      static_assert( !std::is_const<T>::value, "Unable to delete const type" );
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

      /// \brief Constructs an allocator_deleter that uses the referenced
      ///        allocator for deletion
      ///
      /// \param storage the allocator storage
      explicit allocator_deleter( AllocatorStorage storage );

      /// \brief Move-constructs an allocator_deleter from an existing one
      ///
      /// \param other the other allocator_delete to move
      allocator_deleter( allocator_deleter&& other ) noexcept = default;

      /// \brief Copy-constructs the allocator_deleter from an existing one
      ///
      /// \param other the other allocator deleter to copy
      allocator_deleter( const allocator_deleter& other ) noexcept = default;

      //-----------------------------------------------------------------------

      /// \brief Move-assigns an allocator_deleter from an existing one
      ///
      /// \param other the other allocator_deleter to move
      /// \return reference to \c (*this)
      allocator_deleter& operator=( allocator_deleter&& other ) noexcept = default;

      /// \brief Copy-assigns an allocator_deleter from an existing one
      ///
      /// \param other the other allocator_deleter to copy
      /// \return reference to \c (*this)
      allocator_deleter& operator=( const allocator_deleter& other ) noexcept = default;

      //-----------------------------------------------------------------------
      // Deallocation
      //-----------------------------------------------------------------------
    public:

      /// \brief Deletes the underlying pointer \p p
      ///
      /// \param p the pointer to delete
      void operator()( pointer p );
    };

    //-------------------------------------------------------------------------

    template<typename T, typename AllocatorStorage>
    class allocator_deleter<T[],AllocatorStorage>
      : public detail::ebo_storage<AllocatorStorage>
    {
      using base_type = detail::ebo_storage<AllocatorStorage>;
      using allocator_type = typename AllocatorStorage::allocator_type;

      using alloc_traits   = allocator_traits<allocator_type>;
      using pointer_traits = std::pointer_traits<typename alloc_traits::pointer>;

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

      /// \brief Constructs an allocator_deleter that references the specified
      ///        allocator
      ///
      /// \param storage the storage for the allocator
      /// \param size    the size of the array
      allocator_deleter( AllocatorStorage storage, size_type size );

      /// \brief Move-constructs an allocator_deleter from an existing one
      ///
      /// \param other the other allocator_delete to move
      allocator_deleter( allocator_deleter&& other ) noexcept = default;

      /// \brief Copy-constructs the allocator_deleter from an existing one
      ///
      /// \param other the other allocator deleter to copy
      allocator_deleter( const allocator_deleter& other ) noexcept = default;

      //-----------------------------------------------------------------------

      /// \brief Move-assigns an allocator_deleter from an existing one
      ///
      /// \param other the other allocator_deleter to move
      /// \return reference to \c (*this)
      allocator_deleter& operator=( allocator_deleter&& other ) noexcept = default;

      /// \brief Copy-assigns an allocator_deleter from an existing one
      ///
      /// \param other the other allocator_deleter to copy
      /// \return reference to \c (*this)
      allocator_deleter& operator=( const allocator_deleter& other ) noexcept = default;

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

      std::size_t m_size;
    };

  } // namespace memory
} // namespace bit

#include "detail/allocator_deleter.inl"

#endif /* BIT_MEMORY_DELETERS_ALLOCATOR_DELETER_HPP */
