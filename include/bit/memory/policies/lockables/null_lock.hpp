/*****************************************************************************
 * \file
 * \brief This header contains the definition for a null (no-op) BasicLockable
 *****************************************************************************/

/*
  The MIT License (MIT)

  Copyright (c) 2018 Matthew Rodusek

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
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
