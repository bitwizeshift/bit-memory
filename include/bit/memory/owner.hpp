/**
 * \file owner.hpp
 *
 * \brief This header defines the alias 'owner' for indicating memory ownership
 *
 * \note This is quite possibly the smallest header in this library; but also
 *       a very useful utility for self-documenting code
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */
#ifndef BIT_MEMORY_OWNER_HPP
#define BIT_MEMORY_OWNER_HPP

namespace bit {
  namespace memory {

    /// \brief Identity alias used for denoting ownership on API calls
    ///
    /// A function returning a type marked \c owner passes any cleanup
    /// responsibility to the caller.
    template<typename T>
    using owner = T;

  } // namespace memory
} // namespace bit

#endif /* BIT_MEMORY_OWNER_HPP */
