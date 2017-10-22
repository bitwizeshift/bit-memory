/**
 * \file aligned_allocator.hpp
 *
 * \brief This header contains the implementation of a RawAllocator used for
 *        aligned allocations
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */
#ifndef BIT_MEMORY_ALLOCATORS_ALIGNED_ALLOCATOR_HPP
#define BIT_MEMORY_ALLOCATORS_ALIGNED_ALLOCATOR_HPP

#include "detail/named_allocator.hpp" // detail::named_allocator

#include "../allocator_info.hpp" // allocator_info
#include "../aligned_memory.hpp" // aligned_malloc, aligned_free
#include "../macros.hpp"         // BIT_MEMORY_UNUSED
#include "../owner.hpp"          // owner

#include <cstddef>     // std::max_align_t
#include <type_traits> // std::true_type

namespace bit {
  namespace memory {

    ///////////////////////////////////////////////////////////////////////////
    /// \brief This stateless allocator performs all of its allocation calls
    ///        using aligned memory invocations
    ///
    /// \satisfies{Allocator}
    /// \satisfies{Stateless}
    ///////////////////////////////////////////////////////////////////////////
    class aligned_allocator
    {
      //-----------------------------------------------------------------------
      // Public Static Members
      //-----------------------------------------------------------------------
    public:

      using is_stateless      = std::true_type;
      using default_alignment = std::integral_constant<std::size_t,1>;

      //-----------------------------------------------------------------------
      // Constructor / Assignment
      //-----------------------------------------------------------------------
    public:

      /// \brief Default-constructs a aligned_allocator
      aligned_allocator() = default;

      /// \brief Move-constructs an aligned_allocator from another allocator
      ///
      /// \param other the other aligned_allocator to move
      aligned_allocator( aligned_allocator&& other ) noexcept = default;

      /// \brief Copy-constructs an aligned_allocator from another allocator
      ///
      /// \param other the other aligned_allocator to copy
      aligned_allocator( const aligned_allocator& other ) noexcept = default;

      //-----------------------------------------------------------------------

      /// \brief Move-assigns an aligned_allocator from another allocator
      ///
      /// \param other the other aligned_allocator to move
      /// \return reference to \c (*this)
      aligned_allocator& operator=( aligned_allocator&& other ) noexcept = default;

      /// \brief Copy-assigns an aligned_allocator from another allocator
      ///
      /// \param other the other aligned_allocator to copy
      /// \return reference to \c (*this)
      aligned_allocator& operator=( const aligned_allocator& other ) noexcept = default;

      //-----------------------------------------------------------------------
      // Allocations / Deallocation
      //-----------------------------------------------------------------------
    public:

      /// \brief Allocates aligned memory of size \p size, with alignment to a
      ///        boundary of at least \p align
      ///
      /// \param size the size of this allocation
      /// \param align the requested alignment
      /// \return the allocated pointer, or nullptr on failure
      owner<void*> try_allocate( std::size_t size, std::size_t align ) noexcept;

      /// \brief Deallocates a pointer \p p with the allocation size of \p size
      ///
      /// \param p the pointer to deallocate
      /// \param size the size to deallocate
      void deallocate( owner<void*> p, std::size_t size );

      //---------------------------------------------------------------------
      // Observers
      //---------------------------------------------------------------------
    public:

      /// \brief Gets the info about this allocator
      ///
      /// This defaults to 'aligned_allocator'. Use a named_aligned_allocator
      /// to override this
      ///
      /// \return the info for this allocator
      allocator_info info() const noexcept;
    };

    //-------------------------------------------------------------------------
    // Equality
    //-------------------------------------------------------------------------

    /// \{
    /// \brief Compares equality between two aligned_allocator
    ///
    /// Two aligned_allocator are always considered the same
    bool operator==( const aligned_allocator& lhs,
                     const aligned_allocator& rhs ) noexcept;
    bool operator!=( const aligned_allocator& lhs,
                     const aligned_allocator& rhs ) noexcept;
    /// \}

    //-------------------------------------------------------------------------
    // Utilities
    //-------------------------------------------------------------------------

    using named_aligned_allocator = detail::named_allocator<aligned_allocator>;

  } // namespace memory
} // namespace bit

#include "detail/aligned_allocator.inl"

#endif /* BIT_MEMORY_ALLOCATORS_ALIGNED_ALLOCATOR_HPP */
