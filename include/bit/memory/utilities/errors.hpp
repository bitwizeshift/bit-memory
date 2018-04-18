/*****************************************************************************
 * \file
 * \brief This header contains a collection of various error handlers to be
 *        used by allocators
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
#ifndef BIT_MEMORY_UTILITIES_ERRORS_HPP
#define BIT_MEMORY_UTILITIES_ERRORS_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "allocator_info.hpp"

#include <cstddef> // std::size_t, std::ptrdiff_t

namespace bit {
  namespace memory {

    //------------------------------------------------------------------------
    // Types
    //------------------------------------------------------------------------

    /// \brief The type used by the leak handler
    using leak_handler_t = void(*)(const allocator_info&, const void*, std::ptrdiff_t);

    /// \brief The type used by the stomp handler
    using buffer_overflow_handler_t = void(*)(const allocator_info&, const void*, std::ptrdiff_t);

    /// \brief The type used by the double delete handler
    using double_delete_handler_t = void(*)(const allocator_info&, const void*, std::ptrdiff_t);

    /// \brief The type used by the out-of-memory handler
    using out_of_memory_handler_t = void(*)(const allocator_info&, std::size_t );

    //------------------------------------------------------------------------
    // Leak Handler
    //------------------------------------------------------------------------

    /// \brief Makes f the new global memory leak handler function and returns
    ///        the previously installed memory::leak_handler.
    ///
    /// \param f the new handler
    /// \return the old handler
    leak_handler_t set_leak_handler(leak_handler_t f) noexcept;

    /// \brief Gets the current leak handler
    ///
    /// \return the old handler
    leak_handler_t get_leak_handler() noexcept;

    //------------------------------------------------------------------------
    // Stomp Handler
    //------------------------------------------------------------------------

    /// \brief Makes f the new global memory buffer overflow handler function
    ///        and returns the previously installed handler.
    ///
    /// \param f the new handler
    /// \return the old handler
    buffer_overflow_handler_t set_buffer_overflow_handler(buffer_overflow_handler_t f) noexcept;

    /// \brief Gets the current stomp handler
    ///
    /// \return the old handler
    buffer_overflow_handler_t get_buffer_overflow_handler() noexcept;

    //------------------------------------------------------------------------
    // Double Delete Handler
    //------------------------------------------------------------------------

    /// \brief Makes f the new global double-delete handler function and returns
    ///        the previously installed memory::double_delete_handler.
    ///
    /// \param f the new handler
    /// \return the old handler
    double_delete_handler_t set_double_delete_handler(double_delete_handler_t f) noexcept;

    /// \brief Gets the current double-delete handler
    ///
    /// \return the old handler
    double_delete_handler_t get_double_delete_handler() noexcept;

    //------------------------------------------------------------------------
    // Out-of-memory Handler
    //------------------------------------------------------------------------

    /// \brief Makes f the new global out-of-memory handler function and returns
    ///        the previously installed memory::out_of_memory_handler.
    ///
    /// \param f the new handler
    /// \return the old handler
    out_of_memory_handler_t set_out_of_memory_handler(out_of_memory_handler_t f) noexcept;

    /// \brief Gets the current out-of-memory handler
    ///
    /// \return the old handler
    out_of_memory_handler_t get_out_of_memory_handler() noexcept;

  } // namespace memory
} // namespace bit

#endif /* BIT_MEMORY_UTILITIES_ERRORS_HPP */
