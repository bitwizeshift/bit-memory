/**
 * \file debug_bounds_checker.hpp
 *
 * \brief This header contains the implementation of a debug bounds checker
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */
#ifndef BIT_MEMORY_BOUNDS_CHECKERS_DEBUG_BOUNDS_CHECKER_HPP
#define BIT_MEMORY_BOUNDS_CHECKERS_DEBUG_BOUNDS_CHECKER_HPP

#include "../utilities/allocator_info.hpp" // allocator_info
#include "../utilities/debugging.hpp"      // debug_tag_start_bytes, etc
#include "../utilities/errors.hpp"         // get_buffer_overflow_handler

#include "../concepts/BoundsChecker.hpp" // is_bounds_checker

#include <cstddef> // std::size_t

namespace bit {
  namespace memory {

    ///////////////////////////////////////////////////////////////////////////
    /// \brief This type is used for checking boundaries around memory
    ///        allocations to determine if buffer-overflows have occurred
    ///
    /// On any failure, the buffer overflow handler will be called
    ///
    /// \satisfies{BoundsChecker}
    ///////////////////////////////////////////////////////////////////////////
    template<std::size_t FrontSize, std::size_t BackSize = FrontSize>
    class debug_bounds_checker
    {
      static_assert( FrontSize > 0, "FrontSize must be greater than 0" );
      static_assert( BackSize > 0, "BackSize must be greater than 0" );

      //-----------------------------------------------------------------------
      // Public Static Members
      //-----------------------------------------------------------------------
    public:

      static constexpr std::size_t front_size = FrontSize;
      static constexpr std::size_t back_size  = BackSize;

      //-----------------------------------------------------------------------
      // Preparation
      //-----------------------------------------------------------------------
    public:

      void prepare_front_fence( void* p, std::size_t size ) noexcept;

      void prepare_back_fence( void* p, std::size_t size ) noexcept;

      //-----------------------------------------------------------------------
      // Checking
      //-----------------------------------------------------------------------
    public:

      void check_front_fence( const allocator_info& info,
                              void* p,
                              std::size_t size ) noexcept;

      void check_back_fence( const allocator_info& info,
                             void* p,
                             std::size_t size ) noexcept;
    };

    static_assert( is_bounds_checker_v<debug_bounds_checker<1,1>>,
                   "debug_bounds_checker must satisfy BoundsChecker" );

  } // namespace memory
} // namespace bit

#include "detail/debug_bounds_checker.inl"

#endif /* BIT_MEMORY_BOUNDS_CHECKERS_DEBUG_BOUNDS_CHECKER_HPP */
