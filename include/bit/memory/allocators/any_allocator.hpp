/**
 * \file any_allocator.hpp
 *
 * \brief This header contains the implementation of a type-erased allocator
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */
#ifndef BIT_MEMORY_ALLOCATORS_ANY_ALLOCATOR_HPP
#define BIT_MEMORY_ALLOCATORS_ANY_ALLOCATOR_HPP

#include "../owner.hpp"            // owner
#include "../allocator_info.hpp"   // allocator_info
#include "../allocator_traits.hpp" // allocator_traits

#include <cstddef>     // std::size_t, std::ptrdiff_t
#include <type_traits> // std::integral_constant, std::is_same, etc

namespace bit {
  namespace memory {
    namespace detail {
      struct allocator_vtable;
    } // namespace detail

    //////////////////////////////////////////////////////////////////////////
    /// \brief A non-owning allocator type that type-erases an underlying
    ///        allocator
    ///
    /// This view is non-owning, and thus does not participate in the lifetime
    /// of a given allocator.
    ///
    /// \note Since this is type-erased, any static information -- such as
    ///       \c is_stateless, etc -- is lost. It is
    ///       NOT intended as a drop-in replacement for allocators; only a
    ///       means of type-erasing the allocator so that underlying APIs
    ///       need-not require the type of allocator embedded in the class.
    ///
    /// \satisfies{Allocator}
    //////////////////////////////////////////////////////////////////////////
    class any_allocator
    {
      //----------------------------------------------------------------------
      // Public Member Types
      //----------------------------------------------------------------------
    public:

      using default_alignment = std::integral_constant<std::size_t,1>;

      //----------------------------------------------------------------------
      // Constructor / Assignment
      //----------------------------------------------------------------------
    public:

      /// \brief Constructs an allocator by type erasing the underlying
      ///        allocator
      ///
      /// \param allocator the allocator to type-erase
      template<typename Allocator,
               typename = std::enable_if_t<is_allocator<std::decay_t<Allocator>>::value &&
                          !std::is_same<any_allocator,std::decay_t<Allocator>>::value>>
      any_allocator( Allocator& allocator ) noexcept;

      /// \brief Copy-constructs an allocator from an existing one
      ///
      /// \param other the other allocator to copy
      any_allocator( const any_allocator& other ) noexcept = default;

      /// \brief Move-constructs an allocator from an existing one
      ///
      /// \param other the other allocator to move
      any_allocator( any_allocator&& other ) noexcept = default;

      //----------------------------------------------------------------------

      /// \brief Copy-assigns an allocator from an existing one
      ///
      /// \param other the other allocator to copy
      /// \return reference to \c (*this)
      any_allocator& operator = ( const any_allocator& other ) noexcept = default;

      /// \brief Move-assigns an allocator from an existing one
      ///
      /// \param other the other allocator to move
      /// \return reference to \c (*this)
      any_allocator& operator = ( any_allocator&& other ) noexcept = default;

      //----------------------------------------------------------------------
      // Allocation / Deallocation
      //----------------------------------------------------------------------
    public:

      /// \brief Allocates a block from the underlying block allocator
      ///
      /// \param size the size of the allocation
      /// \param align the alignment of the allocation
      /// \return the allocated pointer
      owner<void*> allocate( std::size_t size, std::size_t align );

      /// \brief Allocates a block from the underlying block allocator
      ///
      /// \param size the size of the allocation
      /// \param align the alignment of the allocation
      /// \return the allocated pointer
      owner<void*> try_allocate( std::size_t size, std::size_t align ) noexcept;

      /// \brief Deallocates a block from the underlying block allocatore
      ///
      /// \param p the entry to deallocate, originally allocated with a
      ///          call to allocate
      /// \param n the number of entries to destroy
      void deallocate( owner<void*> p, std::size_t n );

      //----------------------------------------------------------------------
      // Observers
      //----------------------------------------------------------------------
    public:

      /// \brief Gets the amount of bytes used for this allocator
      ///
      /// \return the amount of bytes used
      allocator_info info() const noexcept;

      //----------------------------------------------------------------------
      // Private Member Types
      //----------------------------------------------------------------------
    private:

      using vtable_type = detail::allocator_vtable;

      //----------------------------------------------------------------------
      // Private Members
      //----------------------------------------------------------------------
    private:

      void*        m_ptr;
      vtable_type* m_vtable;

      friend bool operator==( const any_allocator&, const any_allocator& ) noexcept;
    };

    //-------------------------------------------------------------------------
    // Equality Comparison
    //-------------------------------------------------------------------------

    bool operator==( const any_allocator& lhs, const any_allocator& rhs ) noexcept;
    bool operator!=( const any_allocator& lhs, const any_allocator& rhs ) noexcept;

  } // namespace memory
} // namespace bit

#include "detail/any_allocator.inl"

#endif /* BIT_MEMORY_ALLOCATORS_ANY_ALLOCATOR_HPP */
