/**
 * \file linear_allocator.hpp
 *
 * \brief This header contains the definition of the ExtendedAllocator class,
 *        linear_allocator.
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */
#ifndef BIT_MEMORY_ALLOCATORS_LINEAR_ALLOCATOR_HPP
#define BIT_MEMORY_ALLOCATORS_LINEAR_ALLOCATOR_HPP

#include "detail/named_allocator.hpp"

#include "../memory.hpp"       // align_forward
#include "../memory_block.hpp" // memory_block
#include "../macros.hpp"       // BIT_MEMORY_UNLIKELY

#include <cassert>     // assert
#include <cstddef>     // std::size_t, std::ptrdiff_t
#include <type_traits> // std:integral_constant, std::true_type, etc

namespace bit {
  namespace memory {

    //////////////////////////////////////////////////////////////////////////
    /// \brief A linear allocator is an allocator that allocates data
    ///        contiguously
    ///
    /// \satisfies ExtendedAllocator
    //////////////////////////////////////////////////////////////////////////
    class linear_allocator
    {
      //-----------------------------------------------------------------------
      // Public Member Types
      //-----------------------------------------------------------------------
    public:

      using max_alignment              = std::integral_constant<std::size_t,(1 << (sizeof(std::size_t)-1))>;
      using can_truncate_deallocations = std::true_type;

      //-----------------------------------------------------------------------
      // Constructors
      //-----------------------------------------------------------------------
    public:

      /// \brief Constructs a linear_allocator
      explicit linear_allocator( memory_block block ) noexcept;

      /// \brief Move-constructs a linear_allocator from another allocator
      ///
      /// \param other the other linear_allocator to move
      linear_allocator( linear_allocator&& other ) noexcept = default;

      // Deleted copy constructor
      linear_allocator( const linear_allocator& other ) = delete;

      // Deleted nullblock constructor
      linear_allocator( nullblock_t ) = delete;

      //----------------------------------------------------------------------

      /// \brief Move-assigns a linear_allocator from another allocator
      ///
      /// \param other the other allocator to move assign
      /// \return reference to \c (*this)
      linear_allocator& operator=( linear_allocator&& other ) noexcept = default;

      // Deleted copy assignment
      linear_allocator& operator=( const linear_allocator& ) = delete;

      //-----------------------------------------------------------------------
      // Allocations / Deallocations
      //-----------------------------------------------------------------------
    public:

      /// \brief Tries to allocate memory of size \p size, aligned to the
      ///        boundary \p align, offset by \p offset
      ///
      /// \param size the size of the allocation
      /// \param align the requested alignment of the allocation
      /// \param offset the amount to offset the alignment by
      /// \return the allocated pointer on success, \c nullptr on failure
      void* try_allocate( std::size_t size,
                          std::size_t align,
                          std::size_t offset = 0 ) noexcept;

      /// \brief Does nothing for linear_allocator. Use deallocate_all
      ///
      /// \param p the pointer
      /// \param size the size of the allocation
      void deallocate( void* p, std::size_t size );

      /// \brief Deallocates everything from this allocator
      void deallocate_all();

      //-----------------------------------------------------------------------
      // Observers
      //-----------------------------------------------------------------------
    public:

      /// \brief Checks whether \p linear_allocator contains the pointer \p p
      ///
      /// \param p the pointer to check
      /// \return \c true if \p p is contained in this allocator
      bool owns( void* p ) const noexcept;

      //-----------------------------------------------------------------------
      // Private Members
      //-----------------------------------------------------------------------
    private:

      memory_block m_block;
      void*        m_current;
    };

    using named_linear_allocator = detail::named_allocator<linear_allocator>;

  } // namespace memory
} // namespace bit

#include "detail/linear_allocator.inl"

#endif /* BIT_MEMORY_ALLOCATORS_LINEAR_ALLOCATOR_HPP */
