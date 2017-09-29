/**
 * \file macros.hpp
 *
 * \brief TODO: Add description
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */
#ifndef BIT_MEMORY_MACROS_HPP
#define BIT_MEMORY_MACROS_HPP

#ifdef BIT_MEMORY_LIKELY
#error BIT_MEMORY_LIKELY cannot be defined outside of macros.hpp
#endif

#ifdef BIT_MEMORY_UNLIKELY
#error BIT_MEMORY_UNLIKELY cannot be defined outside of macros.hpp
#endif

#ifdef BIT_MEMORY_ASSUME
#error BIT_MEMORY_ASSUME cannot be defined outside of macros.hpp
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
#elif defined(__GNUC__)
#define BIT_MEMORY_ASSUME(x) __builtin_assume(x)
#else
#define BIT_MEMORY_ASSUME(x) x
#endif

#endif /* BIT_MEMORY_MACROS_HPP */
