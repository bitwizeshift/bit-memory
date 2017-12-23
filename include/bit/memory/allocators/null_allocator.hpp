/**
 * \file null_allocator.hpp
 *
 * \brief This header defines the null allocator policy, null_allocator
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */
#ifndef BIT_MEMORY_ALLOCATORS_NULL_ALLOCATOR_HPP
#define BIT_MEMORY_ALLOCATORS_NULL_ALLOCATOR_HPP

#include "detail/named_allocator.hpp" // detail::named_allocator

#include "../allocator_info.hpp" // allocator_info
#include "../macros.hpp"         // BIT_MEMORY_UNUSED
#include "../owner.hpp"          // owner

#include <cstddef>     // std::size_t
#include <type_traits> // std::true_type

namespace bit {
  namespace memory {

    //////////////////////////////////////////////////////////////////////////
    /// \brief An allocator that only ever serves nullptr
    ///
    /// As is expected, the size and alignment of the allocation will always
    /// be ignored, since this only returns nullptr.
    ///
    /// Invoking \c null_allocator::deallocate on a pointer has no effect, but
    /// is considered bad-practice, since this allocator could never produce
    /// a valid allocation.
    ///
    /// \satisfies{ExtendedAllocator}
    /// \satisfies{Stateless}
    //////////////////////////////////////////////////////////////////////////
    class null_allocator
    {
      //-----------------------------------------------------------------------
      // Public Static Members
      //-----------------------------------------------------------------------
    public:

      using default_alignment = std::integral_constant<std::size_t,1>;

      //-----------------------------------------------------------------------
      // Constructor / Assignment
      //-----------------------------------------------------------------------
    public:

      /// \brief Default-constructs a null_allocator
      null_allocator() = default;

      /// \brief Move-constructs a null_allocator from another allocator
      ///
      /// \param other the other null_allocator to move
      null_allocator( null_allocator&& other ) noexcept = default;

      /// \brief Copy-constructs a null_allocator from another allocator
      ///
      /// \param other the other null_allocator to copy
      null_allocator( const null_allocator& other ) noexcept = default;

      //-----------------------------------------------------------------------

      /// \brief Move-assigns a null_allocator from another allocator
      ///
      /// \param other the other null_allocator to move
      /// \return reference to \c (*this)
      null_allocator& operator=( null_allocator&& other ) noexcept = default;

      /// \brief Copy-assigns a null_allocator from another allocator
      ///
      /// \param other the other null_allocator to copy
      /// \return reference to \c (*this)
      null_allocator& operator=( const null_allocator& other ) noexcept = default;

      //----------------------------------------------------------------------
      // Allocation
      //----------------------------------------------------------------------
    public:

      /// \brief Allocates a null pointer
      ///
      /// \param size the size of the allocation
      /// \param align the alignment of the allocation
      /// \return nullptr
      owner<void*> try_allocate( std::size_t size,
                                 std::size_t align,
                                 std::size_t offset = 0 ) noexcept;

      /// \brief Deallocates a pointer previously allocated with a call to
      ///        allocate.
      ///
      /// \param p the pointer to deallocate
      /// \param n the number of entries previously allocated
      void deallocate( owner<void*> p, std::size_t n ) noexcept;

      //----------------------------------------------------------------------
      // Observers
      //----------------------------------------------------------------------
    public:

      /// \brief Checks if the pointer \p p is contained in the null_allocator
      ///
      /// \param p the pointer to check
      /// \return \c true if \p p is \c nullptr
      bool owns( const void* p ) const noexcept;

      /// \brief Checks if nullptr is part of this allocator
      ///
      /// \return true
      bool owns( std::nullptr_t ) const noexcept;

      //----------------------------------------------------------------------

      /// \brief Gets the info about this allocator
      ///
      /// This defaults to 'null_allocator'. Use a
      /// named_null_allocator to override this
      ///
      /// \return the info for this allocator
      allocator_info info() const noexcept;
    };

    //-------------------------------------------------------------------------
    // Equality
    //-------------------------------------------------------------------------

    /// \{
    /// \brief Compares equality between two null_allocators
    ///
    /// Two null_allocators are always considered the same
    bool operator==( const null_allocator& lhs,
                     const null_allocator& rhs ) noexcept;
    bool operator!=( const null_allocator& lhs,
                     const null_allocator& rhs ) noexcept;
    /// \}

    //-------------------------------------------------------------------------
    // Utilities
    //-------------------------------------------------------------------------

    using named_null_allocator = detail::named_allocator<null_allocator>;

  } // namespace memory
} // namespace bit

#include "detail/null_allocator.inl"

#endif /* BIT_MEMORY_ALLOCATORS_NULL_ALLOCATOR_HPP */
