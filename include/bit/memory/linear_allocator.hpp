/**
 * \file linear_allocator.hpp
 *
 * \brief todo: fill in documentation
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */

#ifndef BIT_MEMORY_LINEAR_ALLOCATOR_HPP
#define BIT_MEMORY_LINEAR_ALLOCATOR_HPP

#include "memory.hpp"
#include "memory_block.hpp"
#include "alignment.hpp" // align_forward

#include <cassert>

namespace bit {
  namespace memory {

    //////////////////////////////////////////////////////////////////////////
    /// \brief A linear allocator is an allocator that allocates data
    ///        contiguously
    //////////////////////////////////////////////////////////////////////////
    class linear_allocator
    {
      //----------------------------------------------------------------------
      // Constructors
      //----------------------------------------------------------------------
    public:

      explicit linear_allocator( memory_block block ) noexcept;

      linear_allocator( linear_allocator&& other ) noexcept = default;

      //----------------------------------------------------------------------
      // Allocations
      //----------------------------------------------------------------------
    public:

      void* allocate( std::size_t size, std::size_t align, std::size_t n );

      //----------------------------------------------------------------------

      void deallocate( void* p, std::size_t n );

      //----------------------------------------------------------------------
      // Construction / Destruction
      //----------------------------------------------------------------------
    public:

      template<typename T, typename...Args>
      T* construct( Args&&...args );

      template<typename T>
      T* construct_array( std::size_t n );

      //----------------------------------------------------------------------

      template<typename T>
      void destruct( T* p );

      template<typename T>
      void destruct_array( T* p, std::size_t n );

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

#endif /* BIT_MEMORY_LINEAR_ALLOCATOR_HPP */
