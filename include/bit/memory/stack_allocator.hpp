/**
 * \file stack_allocator.hpp
 *
 * \brief todo: fill in documentation
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */

#ifndef BIT_MEMORY_STACK_ALLOCATOR_HPP
#define BIT_MEMORY_STACK_ALLOCATOR_HPP

namespace bit {
  namespace memory {

    enum class stack_direction {
      upward, ///< Stack memory grows upward
      downard ///< Stack memory grows downward
    };

    class growing_stack_allocator
    {


    public:

      void* allocate();
    };

    class shrinking_stack_allocator
    {


    public:

      void* allocate( std::size_t, std::size_t, std::size_t );
    };

  } // namespace memory
} // namespace bit

#endif /* BIT_MEMORY_STACK_ALLOCATOR_HPP */
