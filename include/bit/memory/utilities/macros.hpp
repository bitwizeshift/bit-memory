/**
 * \file macros.hpp
 *
 * \brief This header contains useful macros for the rest of the library
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
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
