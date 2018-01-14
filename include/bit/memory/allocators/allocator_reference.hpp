/**
 * \file allocator_reference.hpp
 *
 * \brief This header contains the implementation of a type-erased allocator
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */
#ifndef BIT_MEMORY_ALLOCATORS_ALLOCATOR_REFERENCE_HPP
#define BIT_MEMORY_ALLOCATORS_ALLOCATOR_REFERENCE_HPP

#include "../utilities/owner.hpp"            // owner
#include "../utilities/allocator_info.hpp"   // allocator_info
#include "../utilities/macros.hpp"           // BIT_MEMORY_UNUSED

#include "../concepts/Allocator.hpp" // is_allocator
#include "../concepts/Stateless.hpp" // is_stateless

#include "../traits/allocator_traits.hpp" // allocator_traits

#include <cstddef>     // std::size_t, std::ptrdiff_t
#include <type_traits> // std::integral_constant, std::is_same, etc
#include <memory>      // std::addressof

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
    class allocator_reference
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
                          !std::is_same<allocator_reference,std::decay_t<Allocator>>::value>>
      allocator_reference( Allocator& allocator ) noexcept;

      /// \brief Copy-constructs an allocator from an existing one
      ///
      /// \param other the other allocator to copy
      allocator_reference( const allocator_reference& other ) noexcept = default;

      /// \brief Move-constructs an allocator from an existing one
      ///
      /// \param other the other allocator to move
      allocator_reference( allocator_reference&& other ) noexcept = default;

      //----------------------------------------------------------------------

      /// \brief Copy-assigns an allocator from an existing one
      ///
      /// \param other the other allocator to copy
      /// \return reference to \c (*this)
      allocator_reference& operator = ( const allocator_reference& other ) noexcept = default;

      /// \brief Move-assigns an allocator from an existing one
      ///
      /// \param other the other allocator to move
      /// \return reference to \c (*this)
      allocator_reference& operator = ( allocator_reference&& other ) noexcept = default;

      //----------------------------------------------------------------------
      // Allocation / Deallocation
      //----------------------------------------------------------------------
    public:

      /// \brief Allocates a block from the underlying block allocator
      ///
      /// \param size the size of the allocation
      /// \param align the alignment of the allocation
      /// \return the allocated pointer
      owner<void*> try_allocate( std::size_t size, std::size_t align ) noexcept;

      /// \brief Allocates a block from the underlying block allocator
      ///
      /// \param size the size of the allocation
      /// \param align the alignment of the allocation
      /// \return the allocated pointer
      owner<void*> allocate( std::size_t size, std::size_t align );

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

      template<typename Allocator>
      struct stateless_type;

      //----------------------------------------------------------------------
      // Private Constructor
      //----------------------------------------------------------------------
    private:

      /// \brief Constructs an allocator reference to a stateless allocator
      ///
      /// This is an implementation function of
      /// \ref make_stateless_allocator_reference.
      template<typename Allocator>
      explicit allocator_reference( stateless_type<Allocator> ) noexcept;

      //----------------------------------------------------------------------
      // Private Members
      //----------------------------------------------------------------------
    private:

      void*              m_ptr;
      const vtable_type* m_vtable;

      friend bool operator==( const allocator_reference&, const allocator_reference& ) noexcept;

      template<typename>
      friend allocator_reference make_stateless_allocator_reference() noexcept;
    };

    //-------------------------------------------------------------------------
    // Equality Comparison
    //-------------------------------------------------------------------------

    bool operator==( const allocator_reference& lhs, const allocator_reference& rhs ) noexcept;
    bool operator!=( const allocator_reference& lhs, const allocator_reference& rhs ) noexcept;

    //-------------------------------------------------------------------------
    // Utility
    //-------------------------------------------------------------------------

    /// \brief Makes an allocator_reference that refers to a stateless allocator
    ///
    /// This creates a reference to any stateless allocator that doesn't require
    /// an actual instantiation.
    ///
    /// \tparam StatelessAllocator the type of a stateless allocator
    template<typename StatelessAllocator>
    allocator_reference make_stateless_allocator_reference() noexcept;

  } // namespace memory
} // namespace bit

#include "detail/allocator_reference.inl"

#endif /* BIT_MEMORY_ALLOCATORS_ALLOCATOR_REFERENCE_HPP */
