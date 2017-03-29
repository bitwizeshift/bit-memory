/**
 * \file windows.hpp
 *
 * \brief This is an internal header that is used to include windows.h with
 *        the appropriate macro definitions
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */

#ifndef BIT_MEMORY_WIN32_WINDOWS_HPP
#define BIT_MEMORY_WIN32_WINDOWS_HPP

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



#endif /* BIT_MEMORY_WIN32_WINDOWS_HPP */
