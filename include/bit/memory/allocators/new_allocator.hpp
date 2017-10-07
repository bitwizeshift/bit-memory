/**
 * \file new_allocator.hpp
 *
 * \brief This header contains the definition for the RawAllocator,
 *        new_allocator
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */
#ifndef BIT_MEMORY_ALLOCATORS_NEW_ALLOCATOR_HPP
#define BIT_MEMORY_ALLOCATORS_NEW_ALLOCATOR_HPP

#include "../owner.hpp"  // owner
#include "../errors.hpp" // out_of_memory_handler

#include <cstddef>     // std::size_t, std::max_align_t
#include <new>         // ::operator new, ::operator delete, std::nothrow
#include <type_traits> // std::true_type

namespace bit {
  namespace memory {

    ///////////////////////////////////////////////////////////////////////////
    /// \brief This stateless allocator performs all of its allocation calls
    ///        using raw calls to 'new', and frees memory with 'delete'
    ///
    /// Any alignment requests are ignored
    ///
    /// \satisfies Allocator
    ///////////////////////////////////////////////////////////////////////////
    class new_allocator
    {
      //-----------------------------------------------------------------------
      // Public Static Members
      //-----------------------------------------------------------------------
    public:

      using is_always_equal = std::true_type;
      using is_stateless    = std::true_type;
      using default_alignment = std::integral_constant<std::size_t,alignof(std::max_align_t)>;

      //-----------------------------------------------------------------------
      // Constructor / Assignment
      //-----------------------------------------------------------------------
    public:

      /// \brief Default-constructs a new_allocator
      new_allocator() = default;

      /// \brief Move-constructs a new_allocator from another allocator
      ///
      /// \param other the other new_allocator to move
      new_allocator( new_allocator&& other ) noexcept = default;

      // Deleted copy constructor
      new_allocator( const new_allocator& other ) = delete;

      //-----------------------------------------------------------------------

      // Deleted move assignment
      new_allocator& operator=( new_allocator&& other ) = delete;

      // Deleted copy assignment
      new_allocator& operator=( const new_allocator& other ) = delete;

      //-----------------------------------------------------------------------
      // Allocations
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
    /// \brief Compares equality between two new_allocators
    ///
    /// Two new_allocators are always considered the same
    bool operator==( const new_allocator&, const new_allocator& ) noexcept;
    bool operator!=( const new_allocator&, const new_allocator& ) noexcept;
    /// \}

  } // namespace memory
} // namespace bit

#include "detail/new_allocator.inl"

#endif /* BIT_MEMORY_ALLOCATORS_NEW_ALLOCATOR_HPP */
