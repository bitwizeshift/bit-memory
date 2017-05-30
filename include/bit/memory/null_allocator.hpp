/**
 * \file null_allocator.hpp
 *
 * \brief todo: fill in documentation
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */

#ifndef BIT_MEMORY_NULL_ALLOCATOR_HPP
#define BIT_MEMORY_NULL_ALLOCATOR_HPP

#include <cstdlib> // std::size_t

namespace bit {
  namespace memory {

    //////////////////////////////////////////////////////////////////////////
    /// \brief An allocator that only ever serves nullptr
    ///
    /// \satisfies Allocator
    //////////////////////////////////////////////////////////////////////////
    class null_allocator
    {
      //----------------------------------------------------------------------
      // Allocation
      //----------------------------------------------------------------------
    public:

      /// \brief Allocates a null pointer
      ///
      /// \param size the size of the allocation
      /// \param align the alignment of the allocation
      /// \param n the number of entries to allocate
      /// \return nullptr
      void* allocate( std::size_t size, std::size_t align, std::size_t n ) noexcept;

      /// \brief Deallocates a pointer previously allocated with a call to
      ///        allocate.
      ///
      /// \param p the pointer to deallocate
      /// \param n the number of entries previously allocated
      void deallocate( void* p, std::size_t n ) noexcept;
    };

  } // namespace memory
} // namespace bit

#include "detail/null_allocator.inl"

#endif /* BIT_MEMORY_NULL_ALLOCATOR_HPP */
