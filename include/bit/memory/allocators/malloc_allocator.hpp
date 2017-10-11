/**
 * \file malloc_allocator.hpp
 *
 * \brief This header contains the definition for the RawAllocator,
 *        malloc_allocator
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */
#ifndef BIT_MEMORY_ALLOCATORS_MALLOC_ALLOCATOR_HPP
#define BIT_MEMORY_ALLOCATORS_MALLOC_ALLOCATOR_HPP

#include "../owner.hpp"  // owner
#include "../errors.hpp" // out_of_memory_handler

#include <cstdlib>     // std::malloc, std::free, std::size_t
#include <cstddef>     // std::max_align_t
#include <type_traits> // std::true_type

namespace bit {
  namespace memory {

    ///////////////////////////////////////////////////////////////////////////
    /// \brief This stateless allocator performs all of its allocation calls
    ///        using raw calls to 'malloc', and frees memory with 'free'
    ///
    /// Any alignment requests are ignored
    ///
    /// \satisfies{Allocator}
    /// \satisfies{Stateless}
    ///////////////////////////////////////////////////////////////////////////
    class malloc_allocator
    {
      //-----------------------------------------------------------------------
      // Public Static Members
      //-----------------------------------------------------------------------
    public:

      using is_stateless    = std::true_type;
      using default_alignment = std::integral_constant<std::size_t,alignof(std::max_align_t)>;

      //-----------------------------------------------------------------------
      // Constructor / Assignment
      //-----------------------------------------------------------------------
    public:

      /// \brief Default-constructs a malloc_allocator
      malloc_allocator() = default;

      /// \brief Move-constructs a malloc_allocator from another allocator
      ///
      /// \param other the other malloc_allocator to move
      malloc_allocator( malloc_allocator&& other ) noexcept = default;

      /// \brief Copy-constructs a malloc_allocator from another allocator
      ///
      /// \param other the other malloc_allocator to copy
      malloc_allocator( const malloc_allocator& other ) noexcept = default;

      //-----------------------------------------------------------------------

      /// \brief Move-assigns a malloc_allocator from another allocator
      ///
      /// \param other the other malloc_allocator to move
      /// \return reference to \c (*this)
      malloc_allocator& operator=( malloc_allocator&& other ) noexcept = default;

      /// \brief Copy-assigns a malloc_allocator from another allocator
      ///
      /// \param other the other malloc_allocator to copy
      /// \return reference to \c (*this)
      malloc_allocator& operator=( const malloc_allocator& other ) noexcept = default;

      //-----------------------------------------------------------------------
      // Allocations / Deallocation
      //-----------------------------------------------------------------------
    public:

      /// \brief Allocates memory of size \p size
      ///
      /// The alignment is ignored for calls to this allocator. The alignment
      /// is always guaranteed to be at least \c alignof(std::max_align_t)
      ///
      /// \param size the size of this allocation
      /// \param align the requested alignment (ignored)
      /// \return the allocated pointer
      owner<void*> allocate( std::size_t size, std::size_t align );

      /// \brief Attempts to allocate memory of size \p size, returning nullptr
      ///        on failure
      ///
      /// The alignment is ignored for calls to this allocator. The alignment
      /// is always guaranteed to be at least \c alignof(std::max_align_t)
      ///
      /// \param size the size of this allocation
      /// \param align the requested alignment (ignored)
      /// \return the allocated pointer
      owner<void*> try_allocate( std::size_t size, std::size_t align ) noexcept;

      /// \brief Deallocates a pointer \p p with the allocation size of \p size
      ///
      /// \param p the pointer to deallocate
      /// \param size the size to deallocate
      void deallocate( owner<void*> p, std::size_t size );
    };

    /// \{
    /// \brief Compares equality between two malloc_allocators
    ///
    /// Two malloc_allocators are always considered the same
    bool operator==( const malloc_allocator& lhs,
                     const malloc_allocator& rhs ) noexcept;
    bool operator!=( const malloc_allocator& lhs,
                     const malloc_allocator& rhs ) noexcept;
    /// \}

  } // namespace memory
} // namespace bit

#include "detail/malloc_allocator.inl"

#endif /* BIT_MEMORY_ALLOCATORS_MALLOC_ALLOCATOR_HPP */
