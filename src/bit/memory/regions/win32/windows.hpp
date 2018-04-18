/*****************************************************************************
 * \file
 * \brief This is an internal header that is used to include windows.h with
 *        the appropriate macro definitions
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
#ifndef BIT_MEMORY_REGIONS_WIN32_WINDOWS_HPP
#define BIT_MEMORY_REGIONS_WIN32_WINDOWS_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#ifdef _WIN32_WINDOWS
# undef _WIN32_WINDOWS
#endif
#define _WIN32_WINDOWS 0x0501

#ifdef _WIN32_WINNT
# undef _WIN32_WINNT
#endif
#define _WIN32_WINNT   0x0501

#ifdef NOMINMAX
# undef NOMINMAX
#endif
#define WINVER         0x0501

#ifdef WINVER
# undef WINVER
#endif
#define NOMINMAX 1

#include <windows.h>

#endif /* BIT_MEMORY_REGIONS_WIN32_WINDOWS_HPP */
