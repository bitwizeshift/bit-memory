/**
 * \file null_allocator.hpp
 *
 * \brief todo: fill in documentation
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */

#ifndef BIT_MEMORY_ALLOCATORS_NULL_ALLOCATOR_HPP
#define BIT_MEMORY_ALLOCATORS_NULL_ALLOCATOR_HPP

#include "../memory.hpp" // owner
#include "../errors.hpp" // out_of_memory_handler

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
    /// \satisfies Allocator
    //////////////////////////////////////////////////////////////////////////
    class null_allocator
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

      /// \brief Default-constructs a null_allocator
      null_allocator() = default;

      /// \brief Move-constructs a null_allocator from another allocator
      ///
      /// \param other the other null_allocator to move
      null_allocator( null_allocator&& other ) noexcept = default;

      // Deleted copy constructor
      null_allocator( const null_allocator& other ) = delete;

      //-----------------------------------------------------------------------

      /// \brief Move-assigns a null_allocator from another allocator
      ///
      /// \param other the other allocator to move_assign
      /// \return reference to \c (*this)
      null_allocator& operator=( null_allocator&& other ) noexcept = default;

      // Deleted copy assignment
      null_allocator& operator=( const null_allocator& other ) = delete;

      //----------------------------------------------------------------------
      // Allocation
      //----------------------------------------------------------------------
    public:

      /// \brief Allocates a null pointer
      ///
      /// This call immediately calls the 'out_of_memory_handler', since
      /// the null_allocator always returns nullptr
      ///
      /// \param size the size of the allocation
      /// \param align the alignment of the allocation
      /// \return nullptr
      owner<void*> allocate( std::size_t size, std::size_t align );

      /// \brief Allocates a null pointer
      ///
      /// \param size the size of the allocation
      /// \param align the alignment of the allocation
      /// \return nullptr
      owner<void*> try_allocate( std::size_t size, std::size_t align ) noexcept;

      /// \brief Deallocates a pointer previously allocated with a call to
      ///        allocate.
      ///
      /// \param p the pointer to deallocate
      /// \param n the number of entries previously allocated
      void deallocate( owner<void*> p, std::size_t n ) noexcept;
    };


    /// \{
    /// \brief Compares equality between two null_allocators
    ///
    /// Two null_allocators are always considered the same
    bool operator==( const null_allocator& lhs,
                     const null_allocator& rhs ) noexcept;
    bool operator!=( const null_allocator& lhs,
                     const null_allocator& rhs ) noexcept;
    /// \}

    //------------------------------------------------------------------------
    // Utilities
    //------------------------------------------------------------------------

    /// \brief Gets a static null_allocator
    ///
    /// \return a reference to a null_allocator
    null_allocator& null_allocator_instance() noexcept;

  } // namespace memory
} // namespace bit

#include "detail/null_allocator.inl"

#endif /* BIT_MEMORY_ALLOCATORS_NULL_ALLOCATOR_HPP */
