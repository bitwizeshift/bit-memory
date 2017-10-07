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

#include "../owner.hpp"          // owner
#include "../errors.hpp"         // out_of_memory_handler
#include "../aligned_memory.hpp" // aligned_malloc, aligned_free

#include <cstddef>     // std::max_align_t
#include <type_traits> // std::true_type

namespace bit {
  namespace memory {

    ///////////////////////////////////////////////////////////////////////////
    /// \brief This stateless allocator performs all of its allocation calls
    ///        using aligned memory invocations
    ///
    /// \satisfies Allocator
    ///////////////////////////////////////////////////////////////////////////
    class aligned_allocator
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

      /// \brief Default-constructs a aligned_allocator
      aligned_allocator() = default;

      /// \brief Move-constructs a aligned_allocator from another allocator
      ///
      /// \param other the other aligned_allocator to move
      aligned_allocator( aligned_allocator&& other ) noexcept = default;

      // Deleted copy constructor
      aligned_allocator( const aligned_allocator& other ) = delete;

      //-----------------------------------------------------------------------

      // Deleted move assignment
      aligned_allocator& operator=( aligned_allocator&& other ) = delete;

      // Deleted copy assignment
      aligned_allocator& operator=( const aligned_allocator& other ) = delete;

      //-----------------------------------------------------------------------
      // Allocations / Deallocation
      //-----------------------------------------------------------------------
    public:

      /// \brief Allocates aligned memory of size \p size, with alignment to a
      ///        boundary of at least \p align
      ///
      /// \param size the size of this allocation
      /// \param align the requested alignment
      /// \return the allocated pointer
      owner<void*> allocate( std::size_t size, std::size_t align );

      /// \brief Allocates aligned memory of size \p size, with alignment to a
      ///        boundary of at least \p align
      ///
      /// \param size the size of this allocation
      /// \param align the requested alignment
      /// \return the allocated pointer, or nullptr on failure
      void* try_allocate( std::size_t size, std::size_t align ) noexcept;

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
    bool operator==( const aligned_allocator& lhs,
                     const aligned_allocator& rhs ) noexcept;
    bool operator!=( const aligned_allocator& lhs,
                     const aligned_allocator& rhs ) noexcept;
    /// \}

  } // namespace memory
} // namespace bit

#include "detail/aligned_allocator.inl"

#endif /* BIT_MEMORY_ALLOCATORS_ALIGNED_ALLOCATOR_HPP */
