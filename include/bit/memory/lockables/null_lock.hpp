/**
 * \file null_lock.hpp
 *
 * \brief This header contains the definition for a null (no-op) BasicLockable
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */
#ifndef BIT_MEMORY_LOCKABLES_NULL_LOCK_HPP
#define BIT_MEMORY_LOCKABLES_NULL_LOCK_HPP

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

  } // namespace memory
} // namespace bit

#endif /* BIT_MEMORY_LOCKABLES_NULL_LOCK_HPP */
