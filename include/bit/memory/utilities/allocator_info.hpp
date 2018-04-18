/*****************************************************************************
 * \file
 * \brief This header contains the generic identifier used for identifying
 *        unique named allocators
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
#ifndef BIT_MEMORY_UTILITIES_ALLOCATOR_INFO_HPP
#define BIT_MEMORY_UTILITIES_ALLOCATOR_INFO_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

namespace bit {
  namespace memory {

    ///////////////////////////////////////////////////////////////////////////
    /// \brief This type is used to hold the generic information for a given
    ///        allocator
    ///////////////////////////////////////////////////////////////////////////
    class allocator_info
    {
      //-----------------------------------------------------------------------
      // Constructors
      //-----------------------------------------------------------------------
    public:

      /// \brief Constructs an allocator_info with a given \p name and the
      ///        \p address of the allocator
      ///
      /// \param name the name of the allocator
      /// \param address the (possibly null) address of the allocator
      allocator_info( const char* name, const void* address );

      //-----------------------------------------------------------------------
      // Observers
      //-----------------------------------------------------------------------
    public:

      /// \brief Gets the name of the allocator
      ///
      /// \return the name of the allocator
      const char* name() const noexcept;

      /// \brief Gets the address of the allocator
      ///
      /// \return the address of the allocator
      const void* address() const noexcept;

      //-----------------------------------------------------------------------
      // Private Members
      //-----------------------------------------------------------------------
    private:

      const char* m_name;     ///< A non-owned null-terminated byte-string to
                              ///< a name for this debugger
      const void* m_address; ///< Raw pointer to an allocator
    };

    //-------------------------------------------------------------------------
    // Comparisons
    //-------------------------------------------------------------------------

    bool operator==( const allocator_info& lhs, const allocator_info& rhs ) noexcept;
    bool operator!=( const allocator_info& lhs, const allocator_info& rhs ) noexcept;


  } // namespace memory
} // namespace bit

#include "detail/allocator_info.inl"

#endif /* BIT_MEMORY_UTILITIES_ALLOCATOR_INFO_HPP */
