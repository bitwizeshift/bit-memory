/**
 * \file null_bounds_checker.hpp
 *
 * \brief This header contains the definition of a null (no-operation)
 *        BoundsChecker
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */
#ifndef BIT_MEMORY_BOUNDS_CHECKERS_NULL_BOUNDS_CHECKER_HPP
#define BIT_MEMORY_BOUNDS_CHECKERS_NULL_BOUNDS_CHECKER_HPP

#include "../concepts/BoundsChecker.hpp" // is_bounds_checker
#include "../allocator_info.hpp" // allocator_info

#include <cstddef>               // std::size_t

namespace bit {
  namespace memory {

    ///////////////////////////////////////////////////////////////////////////
    /// \brief This bounds checker performs no operation, and is intended to
    ///        be used for non-debug variations of the arena_allocator
    ///
    /// \satisfies{BoundsChecker}
    ///////////////////////////////////////////////////////////////////////////
    class null_bounds_checker
    {
      //-----------------------------------------------------------------------
      // Public Static Members
      //-----------------------------------------------------------------------
    public:

      static constexpr std::size_t front_size = 0;
      static constexpr std::size_t back_size  = 0;

      //-----------------------------------------------------------------------
      // Preparation
      //-----------------------------------------------------------------------
    public:

      void prepare_front_fence( void*, std::size_t ){}
      void prepare_back_fence( void*, std::size_t ){}

      //-----------------------------------------------------------------------
      // Checking
      //-----------------------------------------------------------------------
    public:

      void check_front_fence( const allocator_info&, void*, std::size_t ){}
      void check_back_fence( const allocator_info&, void*, std::size_t ){}
    };

    static_assert( is_bounds_checker_v<null_bounds_checker>,
                   "null_bounds_checker must satisfy BoundsChecker" );

  } // namespace memory
} // namespace bit

#endif /* BIT_MEMORY_BOUNDS_CHECKERS_NULL_BOUNDS_CHECKER_HPP */
