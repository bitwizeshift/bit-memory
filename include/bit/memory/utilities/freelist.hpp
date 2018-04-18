/*****************************************************************************
 * \file
 * \brief This header contains the definition of a simple freelist utility
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
#ifndef BIT_MEMORY_UTILITIES_FREELIST_HPP
#define BIT_MEMORY_UTILITIES_FREELIST_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "pointer_utilities.hpp"     // align_of
#include "uninitialized_storage.hpp" // uninitialized_construct_at

#include <cstddef> // std::size_t
#include <cassert> // assert

namespace bit {
  namespace memory {

    ///////////////////////////////////////////////////////////////////////////
    /// \brief A simple freelist that uses intrusive pointers inside raw memory
    ///
    /// This is dissimilar from the memory_block_cache in that it does not store
    /// each block sie in the freelist; it only knows of the pointers. The
    /// semantics, ownership, and valdity of the pointers must all be managed
    /// from outside
    ///////////////////////////////////////////////////////////////////////////
    class freelist
    {
      //-----------------------------------------------------------------------
      // Constructors
      //-----------------------------------------------------------------------
    public:

      /// \brief Default constructs an empty freelist
      constexpr freelist() noexcept;

      /// \brief Move-assigns a freelist from an existing freelist
      freelist( freelist&& other ) noexcept = default;

      // Deleted copy construction
      freelist( const freelist& other ) = delete;

      //-----------------------------------------------------------------------

      // Deleted move assignment
      freelist& operator=( freelist&& other ) = delete;

      // Deleted copy construction
      freelist& operator=( const freelist& other ) = delete;

      //----------------------------------------------------------------------
      // Modifiers
      //----------------------------------------------------------------------
    public:

      /// \brief Swaps this with another block cache
      ///
      /// \param other the other memory_block_cache to swap with
      void swap( freelist& other ) noexcept;

      //----------------------------------------------------------------------
      // Observers
      //----------------------------------------------------------------------
    public:

      /// \brief Returns whether or not this memory_block_cache is empty
      ///
      /// \return \c true if this memory_block_cache is empty, \c false otherwise
      bool empty() const noexcept;

      /// \brief Returns the number of entries in this freelist
      ///
      /// \return the number of entries in this freelist
      std::size_t size() const noexcept;

      //----------------------------------------------------------------------
      // Modifiers
      //----------------------------------------------------------------------
    public:

      /// \brief Empties the freelist cache
      void clear();

      //----------------------------------------------------------------------
      // Caching
      //----------------------------------------------------------------------
    public:

      /// \brief Requests raw memory from the freelist, if any exists
      ///
      /// The size of the returned instance is not known to the freelist
      ///
      /// \return pointer to memory, if it exists
      void* request() noexcept;

      /// \brief Steals raw meory from an existing freelist
      void steal( freelist& other ) noexcept;

      /// \brief Stores raw memory into this freelist
      ///
      /// \pre The pointer \p p must point to memory of at least
      ///      \c sizeof(void*) bytes, and must be suitably aligned to support
      ///      pointer types.
      ///
      /// \param p pointer to the raw memory to store
      void store( void* p ) noexcept;

      //-----------------------------------------------------------------------
      // Private Members
      //-----------------------------------------------------------------------
    private:

      void* m_head;

      friend bool operator==( const freelist&, const freelist& ) noexcept;
    };

    //-------------------------------------------------------------------------
    // Comparisons
    //-------------------------------------------------------------------------

    bool operator==( const freelist& lhs, const freelist& rhs ) noexcept;
    bool operator!=( const freelist& lhs, const freelist& rhs ) noexcept;

  } // namespace memory
} // namespace bit

#include "detail/freelist.inl"

#endif /* BIT_MEMORY_UTILITIES_FREELIST_HPP */
