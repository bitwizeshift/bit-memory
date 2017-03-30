/**
 * \file memory.hpp
 *
 * \brief todo: fill in documentation
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */

#ifndef BIT_MEMORY_MEMORY_HPP
#define BIT_MEMORY_MEMORY_HPP

#include <cstddef> // std::size_t

namespace bit {
  namespace memory {

    /// \brief Identity alias used for denoting ownership on API calls
    ///
    /// A function returning a type marked \c owner passes any cleanup
    /// responsibility to the caller.
    template<typename T>
    using owner = T;

    inline namespace literals {
      inline namespace byte_literals {

        constexpr std::size_t operator ""_b( unsigned long long b ) noexcept;
        constexpr std::size_t operator ""_kb( unsigned long long b ) noexcept;
        constexpr std::size_t operator ""_mb( unsigned long long b ) noexcept;
        constexpr std::size_t operator ""_gb( unsigned long long b ) noexcept;

      } // inline namespace byte_literals
    } // inline namespace literals

  } // namespace memory
} // namespace bit

#include "detail/memory.inl"

#endif /* BIT_MEMORY_MEMORY_HPP */
