/**
 * \file aligned_offset_allocator.hpp
 *
 * \brief This header contains the implementation of an allocator used for
 *        offset aligned allocations
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */
#ifndef BIT_MEMORY_ALLOCATORS_ALIGNED_OFFSET_ALLOCATOR_HPP
#define BIT_MEMORY_ALLOCATORS_ALIGNED_OFFSET_ALLOCATOR_HPP

#include "../owner.hpp"          // owner
#include "../aligned_memory.hpp" // aligned_malloc, aligned_free

#include <cstddef>     // std::max_align_t
#include <type_traits> // std::true_type

namespace bit {
  namespace memory {

    ///////////////////////////////////////////////////////////////////////////
    /// \brief This stateless allocator performs all of its allocation calls
    ///        using aligned memory invocations aligned to an offset
    ///
    /// \satisfies ExtendedAllocator
    ///////////////////////////////////////////////////////////////////////////
    class aligned_offset_allocator
    {
      //-----------------------------------------------------------------------
      // Public Static Members
      //-----------------------------------------------------------------------
    public:

      using is_always_equal   = std::true_type;
      using is_stateless      = std::true_type;
      using default_alignment = std::integral_constant<std::size_t,1>;

      //-----------------------------------------------------------------------
      // Constructor / Assignment
      //-----------------------------------------------------------------------
    public:

      /// \brief Default-constructs a aligned_offset_allocator
      aligned_offset_allocator() = default;

      /// \brief Move-constructs a aligned_offset_allocator from another allocator
      ///
      /// \param other the other aligned_offset_allocator to move
      aligned_offset_allocator( aligned_offset_allocator&& other ) noexcept = default;

      /// \brief Copy-constructs a aligned_offset_allocator from another allocator
      ///
      /// \param other the other aligned_offset_allocator to copy
      aligned_offset_allocator( const aligned_offset_allocator& other ) noexcept = default;

      //-----------------------------------------------------------------------

      /// \brief Move-assigns an aligned_offset_allocator from another allocator
      ///
      /// \param other the other aligned_offset_allocator to copy
      /// \return reference to \c (*this)
      aligned_offset_allocator& operator=( aligned_offset_allocator&& other ) noexcept = default;

      /// \brief Copy-assigns an aligned_offset_allocator from another allocator
      ///
      /// \param other the other aligned_offset_allocator to copy
      /// \return reference to \c (*this)
      aligned_offset_allocator& operator=( const aligned_offset_allocator& other ) noexcept = default;

      //-----------------------------------------------------------------------
      // Allocations / Deallocation
      //-----------------------------------------------------------------------
    public:

      /// \brief Allocates aligned memory of size \p size, with alignment to a
      ///        boundary of at least \p align, aligned to an offset
      ///
      /// \param size the size of this allocation
      /// \param align the requested alignment
      /// \param offset the offset to align to
      /// \return the allocated pointer, or nullptr on failure
      void* try_allocate( std::size_t size,
                          std::size_t align,
                          std::size_t offset = 0 ) noexcept;

      /// \brief Deallocates a pointer \p p with the allocation size of \p size
      ///
      /// \param p the pointer to deallocate
      /// \param size the size to deallocate
      void deallocate( owner<void*> p, std::size_t size );
    };

    /// \{
    /// \brief Compares equality between two aligned_allocator
    ///
    /// Two aligned_allocator are always considered the same
    bool operator==( const aligned_offset_allocator& lhs,
                     const aligned_offset_allocator& rhs ) noexcept;
    bool operator!=( const aligned_offset_allocator& lhs,
                     const aligned_offset_allocator& rhs ) noexcept;
    /// \}

  } // namespace memory
} // namespace bit

#include "detail/aligned_offset_allocator.inl"

#endif /* BIT_MEMORY_ALLOCATORS_ALIGNED_OFFSET_ALLOCATOR_HPP */
