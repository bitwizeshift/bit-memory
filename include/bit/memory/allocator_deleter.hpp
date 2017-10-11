/**
 * \file allocator_deleter.hpp
 *
 * \brief This header contains the definition of an allocator deleter for use
 *        with smart pointer types
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */
#ifndef BIT_MEMORY_ALLOCATOR_DELETER_HPP
#define BIT_MEMORY_ALLOCATOR_DELETER_HPP

#include "detail/ebo_storage.hpp"  // detail::ebo_storage

#include "allocator_reference.hpp"   // allocator_reference
#include "allocator_traits.hpp"      // allocator_traits
#include "uninitialized_storage.hpp" // uninitialized_construct_at

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

    namespace detail {
      template<typename T, typename Allocator>
      struct unique_ptr_if
      {
        using single_type = std::unique_ptr<T,allocator_deleter<T,Allocator>>;
      };

      template<typename T, typename Allocator>
      struct unique_ptr_if<T[],Allocator>
      {
        using array_type = std::unique_ptr<T[],allocator_deleter<T[],Allocator>>;
      };

      template<typename T, typename Allocator, std::size_t N>
      struct unique_ptr_if<T[N],Allocator>{};

      template<typename T, typename Allocator>
      using unique_ptr_single_t = typename unique_ptr_if<T,Allocator>::single_type;

      template<typename T, typename Allocator>
      using unique_ptr_array_t = typename unique_ptr_if<T,Allocator>::array_type;

      template<typename T>
      struct shared_ptr_if
      {
        using single_type = std::shared_ptr<T>;
      };

      template<typename T>
      struct shared_ptr_if<T[]>
      {
        using array_type = std::shared_ptr<T[]>;
      };

      template<typename T, std::size_t N>
      struct shared_ptr_if<T[N]>{};

      template<typename T>
      using shared_ptr_single_t = typename shared_ptr_if<T>::single_type;

      template<typename T>
      using shared_ptr_array_t = typename shared_ptr_if<T>::array_type;
    }

    //-------------------------------------------------------------------------
    // Free Functions
    //-------------------------------------------------------------------------

    /// \brief Allocates a single element with the given \p allocator, using an
    ///        allocator_deleter
    ///
    /// \tparam T the type to construct
    /// \param allocator the allocator to allocate the memory
    /// \param args the arguments to forward to \p T
    /// \return the unique_ptr
    template<typename T, typename Allocator, typename... Args>
    detail::unique_ptr_single_t<T,Allocator>
      allocate_unique( Allocator& allocator, Args&&...args );

    /// \brief Allocates an array with the given \p allocator, using an
    ///        allocator_deleter
    ///
    /// \tparam T the type to construct
    /// \param allocator the allocator to allocate the memory
    /// \param n the number of elements to default construct
    /// \return the unique_ptr
    template<typename T, typename Allocator, typename... Args>
    detail::unique_ptr_array_t<T,Allocator>
      allocate_unique( Allocator& allocator, std::size_t n );

    //-------------------------------------------------------------------------

    /// \brief Allocates a single element with the given \p allocator, using an
    ///        allocator_deleter
    ///
    /// \tparam T the type to construct
    /// \param allocator the allocator to allocate the memory
    /// \param args the arguments to forward to \p T
    /// \return the shared_ptr
    template<typename T, typename Allocator, typename... Args>
    detail::shared_ptr_single_t<T>
      allocate_shared( Allocator& allocator, Args&&...args );

    /// \brief Allocates an array with the given \p allocator, using an
    ///        allocator_deleter
    ///
    /// \tparam T the type to construct
    /// \param allocator the allocator to allocate the memory
    /// \param n the number of elements to default construct
    /// \return the shared_ptr
    template<typename T, typename Allocator, typename... Args>
    detail::shared_ptr_array_t<T>
      allocate_shared( Allocator& allocator, std::size_t n );

  } // namespace memory
} // namespace bit

#include "detail/allocator_deleter.inl"

#endif /* BIT_MEMORY_ALLOCATOR_DELETER_HPP */
