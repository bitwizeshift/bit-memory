/**
 * \file linear_allocator.hpp
 *
 * \brief todo: fill in documentation
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */
#ifndef BIT_MEMORY_ALLOCATORS_LINEAR_ALLOCATOR_HPP
#define BIT_MEMORY_ALLOCATORS_LINEAR_ALLOCATOR_HPP

#include "../memory.hpp"
#include "../memory_block.hpp"
#include "../alignment.hpp" // align_forward

#include <cassert>

namespace bit {
  namespace memory {

    //////////////////////////////////////////////////////////////////////////
    /// \brief A linear allocator is an allocator that allocates data
    ///        contiguously
    ///
    /// \satisfies Allocator
    //////////////////////////////////////////////////////////////////////////
    class linear_allocator
    {
      //----------------------------------------------------------------------
      // Constructors
      //----------------------------------------------------------------------
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

      //----------------------------------------------------------------------
      // Allocations
      //----------------------------------------------------------------------
    public:

      void* allocate( std::size_t size, std::size_t align );

      void* try_allocate( std::size_t size, std::size_t align ) noexcept;

      void deallocate( void* p, std::size_t size );

      //----------------------------------------------------------------------
      // Modifiers
      //----------------------------------------------------------------------
    public:

      /// \brief Resets memory in the linear allocator
      void reset() noexcept;

      //----------------------------------------------------------------------
      // Private Members
      //----------------------------------------------------------------------
    private:

      memory_block m_block;
      void*        m_current;
    };

  } // namespace memory
} // namespace bit

#include "detail/linear_allocator.inl"

#endif /* BIT_MEMORY_ALLOCATORS_LINEAR_ALLOCATOR_HPP */
