/*****************************************************************************
 * \file
 * \brief This header contains useful macros for the rest of the library
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
#ifndef BIT_MEMORY_UTILITIES_MACROS_HPP
#define BIT_MEMORY_UTILITIES_MACROS_HPP

#ifdef BIT_MEMORY_LIKELY
#error BIT_MEMORY_LIKELY cannot be defined outside of macros.hpp
#endif

#ifdef BIT_MEMORY_UNLIKELY
#error BIT_MEMORY_UNLIKELY cannot be defined outside of macros.hpp
#endif

#ifdef BIT_MEMORY_ASSUME
#error BIT_MEMORY_ASSUME cannot be defined outside of macros.hpp
#endif

#ifdef BIT_MEMORY_UNUSED
#error BIT_MEMORY_UNUSED cannot be defined outside of macros.hpp
#endif


#ifdef __GNUC__
#define BIT_MEMORY_LIKELY(x) __builtin_expect(!!(x),1)
#else
#define BIT_MEMORY_LIKELY(x) x
#endif

#ifdef __GNUC__
#define BIT_MEMORY_UNLIKELY(x) __builtin_expect(!!(x),0)
#else
#define BIT_MEMORY_UNLIKELY(x) x
#endif

#if defined(__clang__)
#define BIT_MEMORY_ASSUME(x) __builtin_assume(x)
#elif defined(_MSC_VER)
#define BIT_MEMORY_ASSUME(x) __assume(x)
#else
#define BIT_MEMORY_ASSUME(x) x
#endif

#define BIT_MEMORY_UNUSED(x) (void) x

#endif /* BIT_MEMORY_UTILITIES_MACROS_HPP */
