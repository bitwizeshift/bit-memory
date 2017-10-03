/**
 * \file allocator_deleter.hpp
 *
 * \brief TODO: Add description
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */
#ifndef BIT_MEMORY_ALLOCATOR_DELETER_HPP
#define BIT_MEMORY_ALLOCATOR_DELETER_HPP

#include "detail/ebo_storage.hpp"  // detail::ebo_storage

#include "allocator_reference.hpp" // allocator_reference
#include "allocator_traits.hpp"    // allocator_traits

#include <cstddef> // std::size_t
#include <tuple>   // std::forward_as_tuple
#include <memory>  // std::pointer_traits

namespace bit {
  namespace memory {

    ///////////////////////////////////////////////////////////////////////////
    /// \brief A deallocator for custom allocators in the library.
    ///
    /// For any stateless allocator, this leverages EBO to avoid the need for
    /// storage.
    ///
    /// \tparam T the type being deallocated
    /// \tparam Allocator the allocator to deallocate with
    ///////////////////////////////////////////////////////////////////////////
    template<typename T, typename Allocator>
    class allocator_deleter
      : public detail::ebo_storage<allocator_reference<Allocator>>
    {
      using base_type = detail::ebo_storage<allocator_reference<Allocator>>;

      using alloc_traits   = allocator_traits<Allocator>;
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
      /// \param alloc the allocator to reference
      explicit allocator_deleter( Allocator& alloc );

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

    template<typename T, typename Allocator>
    class allocator_deleter<T[],Allocator>
      : public detail::ebo_storage<allocator_reference<Allocator>>
    {
      using base_type = detail::ebo_storage<allocator_reference<Allocator>>;

      using alloc_traits   = allocator_traits<Allocator>;
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
      /// \param alloc the allocator to reference
      /// \param size  the size of the array
      explicit allocator_deleter( Allocator& alloc, size_type size );

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

#endif /* BIT_MEMORY_ALLOCATOR_DELETER_HPP */
