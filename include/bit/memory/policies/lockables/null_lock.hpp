/**
 * \file null_lock.hpp
 *
 * \brief This header contains the definition for a null (no-op) BasicLockable
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */
#ifndef BIT_MEMORY_POLICIES_LOCKABLES_NULL_LOCK_HPP
#define BIT_MEMORY_POLICIES_LOCKABLES_NULL_LOCK_HPP

#include "../../concepts/BasicLockable.hpp"

namespace bit {
  namespace memory {

    ///////////////////////////////////////////////////////////////////////////
    /// \brief A BasicLockable type that performs no locking at all
    ///
    /// \satisfies{BasicLockable}
    ///////////////////////////////////////////////////////////////////////////
    class null_lock
    {
      //-----------------------------------------------------------------------
      // Locking
      //-----------------------------------------------------------------------
    public:

      void lock(){}

      bool try_lock(){ return true; }

      void unlock(){}
    };

    static_assert( is_basic_lockable_v<null_lock>,
                   "null_lock must satisfy BasicLockable" );

  } // namespace memory
} // namespace bit

#endif /* BIT_MEMORY_POLICIES_LOCKABLES_NULL_LOCK_HPP */
