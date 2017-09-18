/**
 * \file new_allocator.hpp
 *
 * \brief This header contains the definition for the RawAllocator,
 *        new_allocator
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */
#ifndef BIT_MEMORY_NEW_ALLOCATOR_HPP
#define BIT_MEMORY_NEW_ALLOCATOR_HPP

#include "debugging.hpp" // out_of_memory_handler

#include <cstdlib>     // std::size_t
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
    /// \satisfies RawAllocator
    ///////////////////////////////////////////////////////////////////////////
    class new_allocator
    {
      //-----------------------------------------------------------------------
      // Public Static Members
      //-----------------------------------------------------------------------
    public:

      using is_always_equal = std::true_type;
      using is_stateless    = std::true_type;

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

      /// \brief Move-assigns a new_allocator from another allocator
      ///
      /// \param other the other allocator to move_assign
      /// \return reference to \c (*this)
      new_allocator& operator=( new_allocator&& other ) noexcept = default;

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
      void* allocate( std::size_t size, std::size_t align );

      /// \brief Attempts to allocate memory of size \p size, returning nullptr
      ///        on failure
      ///
      /// The alignment is ignored for calls to this allocator. The alignment
      /// is always guaranteed to be at least \c alignof(std::max_align_t)
      ///
      /// \param size the size of this allocation
      /// \param align the requested alignment (ignored)
      /// \return the allocated pointer
      void* try_allocate( std::size_t size, std::size_t align ) noexcept;

      /// \brief Deallocates a pointer \p p with the allocation size of \p size
      ///
      /// \param p the pointer to deallocate
      /// \param size the size to deallocate
      void deallocate( void* p, std::size_t size );
    };

  } // namespace memory
} // namespace bit

#include "detail/new_allocator.inl"

#endif /* BIT_MEMORY_NEW_ALLOCATOR_HPP */