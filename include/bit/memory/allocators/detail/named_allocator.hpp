/*****************************************************************************
 * \file
 * \brief This header contains a composite operator that allows naming
 *        of the underlying allocator
 *
 * \note This is an internal header file, included by other library headers.
 *       Do not attempt to use it directly.
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
#ifndef BIT_MEMORY_ALLOCATORS_DETAIL_NAMED_ALLOCATOR_HPP
#define BIT_MEMORY_ALLOCATORS_DETAIL_NAMED_ALLOCATOR_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "../../utilities/allocator_info.hpp" // allocator_info

#include <type_traits> // std::enable_if_t, std::is_constructible
#include <utility>     // std::forward

namespace bit {
  namespace memory {
    namespace detail {

      /////////////////////////////////////////////////////////////////////////
      /// \brief A wrapper around allocators to provide a name to their
      ///        allocator info.
      ///
      /// The underlying allocator must not be marked 'final', otherwise
      /// this will not compile.
      ///
      /// \tparam Allocator the underlying Allocator
      /////////////////////////////////////////////////////////////////////////
      template<typename Allocator>
      class named_allocator : public Allocator
      {
        //---------------------------------------------------------------------
        // Public Member Types
        //---------------------------------------------------------------------
      public:

        /// Named allocators cannot be stateless, since they have unique names
        using is_stateless = std::false_type;

        //---------------------------------------------------------------------
        // Constructors
        //---------------------------------------------------------------------
      public:

        /// \brief constructs a named allocator
        ///
        /// \param str the string name of the allocator
        /// \param args arguments to forward to the underlying allocator
        template<typename...Args, typename = std::enable_if_t<std::is_constructible<Allocator,Args...>::value>>
        named_allocator( const char* str, Args&&...args );

        //---------------------------------------------------------------------
        // Observers
        //---------------------------------------------------------------------
      public:

        /// \brief Gets the info about this allocator
        ///
        /// \return the info for this allocator
        allocator_info info() const noexcept;

        //---------------------------------------------------------------------
        // Private Members
        //---------------------------------------------------------------------
      private:

        const char* m_name;
      };

    } // namespace detail
  } // namespace memory
} // namespace bit

#include "named_allocator.inl"

#endif /* BIT_MEMORY_ALLOCATORS_DETAIL_NAMED_ALLOCATOR_HPP */
