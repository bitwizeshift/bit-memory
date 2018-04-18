/*****************************************************************************
 * \file
 * \brief This header defines various uninitialized storage functionality
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
#ifndef BIT_MEMORY_UTILITIES_UNINITIALIZED_STORAGE_HPP
#define BIT_MEMORY_UTILITIES_UNINITIALIZED_STORAGE_HPP

#include <tuple>   // std::forward_as_tuple, std::tuple
#include <utility> // std::forward
#include <cstddef> // std::size_t
#include <new>     // placement new

namespace bit {
  namespace memory {

    //------------------------------------------------------------------------
    // Construction
    //------------------------------------------------------------------------

    /// \brief Constructs an instance of type \p T with the given \p args
    ///        at the memory location specified in \p ptr
    ///
    /// \param ptr     The memory location to construct into
    /// \param args... The arguments to supply to T's constructor
    /// \return Pointer to the initialized memory (cast of \p ptr)
    template<typename T, typename...Args>
    T* uninitialized_construct_at( void* p, Args&&...args );

    /// \brief Constructs an array of the specified size at pointer \p p
    ///
    /// \tparam T the type to construct
    /// \param p the pointer to construct the array at
    /// \param n the size of the array
    /// \return the pointer to the constructed array
    template<typename T>
    T* uninitialized_construct_array_at( void* p, std::size_t n );

    /// \brief Constructs an array of the specified size at pointer \p p by
    ///        copying \p copy to each array member
    ///
    /// \tparam T the type to construct
    /// \tparam p the pointer to construct the array at
    /// \param n the size of the array
    /// \param copy the instance to copy to each array member
    /// \return the pointer to the constructed array
    template<typename T>
    T* uninitialized_construct_array_at( void* p, std::size_t n, const T& copy );

    /// \brief Constructs an instance of type \p T from a given \p tuple
    ///
    /// \tparam T the type to construct
    /// \param ptr the pointer to construct the type at
    /// \param tuple the tuple to unpack as arguments to \p T
    /// \return pointer to the constructed element
    template<typename T, typename Tuple>
    T* uninitialized_construct_from_tuple( void* p, Tuple&& tuple );

    /// \brief Constructs a given type from a tuple
    ///
    /// \tparam T the type to construct
    /// \param tuple the tuple to unpack for arguments
    /// \return the construct type
    template<typename T, typename Tuple>
    T make_from_tuple( Tuple&& tuple );

    //------------------------------------------------------------------------
    // Destruction
    //------------------------------------------------------------------------

    /// \brief Destroys the type at the given pointer \p p
    ///
    /// \param p the pointer to the instance to destroy
    template<typename T>
    void destroy_at( T* p );

    /// \brief Destroys the array of instances at the specified pointer \p p
    ///
    /// \param p the pointer to the array to destroy
    /// \param n the size of the array
    template<typename T>
    void destroy_array_at( T* p, std::size_t n );

  } // namespace memory
} // namespace bit

#include "detail/uninitialized_storage.inl"

#endif /* BIT_MEMORY_UTILITIES_UNINITIALIZED_STORAGE_HPP */
