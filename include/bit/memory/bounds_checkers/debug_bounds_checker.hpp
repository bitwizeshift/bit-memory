/**
 * \file debug_bounds_checker.hpp
 *
 * \brief This header contains the implementation of a debug bounds checker
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */
#ifndef BIT_MEMORY_BOUNDS_CHECKERS_DEBUG_BOUNDS_CHECKER_HPP
#define BIT_MEMORY_BOUNDS_CHECKERS_DEBUG_BOUNDS_CHECKER_HPP

#include "../concepts/BoundsChecker.hpp" // is_bounds_checker
#include "../allocator_info.hpp" // allocator_info
#include "../debugging.hpp"      // debug_tag_start_bytes, etc
#include "../errors.hpp"         // get_buffer_overflow_handler

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
    class debug_bounds_checker
    {
      //-----------------------------------------------------------------------
      // Public Static Members
      //-----------------------------------------------------------------------
    public:

      static constexpr std::size_t front_size = 8;
      static constexpr std::size_t back_size  = 8;

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

    static_assert( is_bounds_checker_v<debug_bounds_checker>,
                   "debug_bounds_checker must satisfy BoundsChecker" );

  } // namespace memory
} // namespace bit

#include "detail/debug_bounds_checker.inl"

#endif /* BIT_MEMORY_BOUNDS_CHECKERS_DEBUG_BOUNDS_CHECKER_HPP */
