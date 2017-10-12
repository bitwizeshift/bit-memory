# `bit::memory` Memory library

[![Build Status](https://travis-ci.org/bitwizeshift/bit-memory.svg?branch=master)](https://travis-ci.org/bitwizeshift/bit-memory)
[![Build status](https://ci.appveyor.com/api/projects/status/ou5sraydky6tjxv9?svg=true)](https://ci.appveyor.com/project/bitwizeshift/bit-memory)
[![Github Issues](https://img.shields.io/github/issues/bitwizeshift/bit-memory.svg)](http://github.com/bitwizeshift/bit-memory/issues)
[![Tested Compilers](https://img.shields.io/badge/compilers-gcc%20%7C%20clang-blue.svg)](#tested-compilers)
[![Documentation](https://img.shields.io/badge/docs-doxygen-blue.svg)](http://bitwizeshift.github.io/bit-memory)
[![GitHub License](https://img.shields.io/badge/license-MIT-blue.svg)](https://github.com/bitwizeshift/bit-memory/blob/master/LICENSE.md)
[![Github Releases](https://img.shields.io/github/release/bitwizeshift/bit-memory.svg)](https://github.com/bitwizeshift/bit-memory/releases)

## What is `bit::memory`?

This is an all-purpose memory management library. The primary emphasis on this library revolves around the memory allocators in the library,
designed to be as lightweight and efficient as possible. Custom C++ _logical_ concepts are designed and adhered to strictly within this library. Sadly these are not (yet) `C++20` or `C++ Concepts TS` concepts since they are not standardized.

Additionally, this library offers utilities for the following concerns:

- Standard Adapters: Adapter functionality between this library's `Allocator` concepts and the C++ standard library Allocator
- Endianness: Detects host machine endianness, and offers casting with `endian_cast` and `endian_swap` utilities
- Unaligned Access: Utilities for loading unaligned data in a standard-compliant manner
- Virtual Memory: Functionality for reserving and committing virtual memory pages in a cross-platform way (MacOS,Linux, and Windows)
- Aligned Memory: Functionality for allocating over-aligned memory

This library defines a variety of `BlockAllocator`s and `Allocator`s -- and is extendable through user-defined custom types that
conform to the concepts defined here.

## Differences from Standard Allocators

The `Allocator` concept in this library differs from the C++ standard's allocators in a few ways

### 1. Allocators are heterogenious

The C++ Standard Library's `Allocator` concept defines every `Allocator` to be homogenous in its allocations; always allocating
a specific type `T`, and only converting to different allocators using `rebind`. This offers some level of optimizations, since
all allocations are of fixed size; but also can cause large amounts of code duplication due to the requirement of templates to
produce different instances of the same type, all based on type `T`.

As a result, this library uses the SGI and HP approach of only allocating by `void*`, rather than by a given type. A wrapper
type called `std_allocator_wrapper` exists to wrap the given allocator in a manner that works with C++ standard containers.

### 2. Allocators support alignment

Since the C++ Standard Library's `Allocator` only cares about the type `T` being allocated, there is no generalized support
for over-aligning types.

The `Allocator` concept in this library _requires_ the presence of an alignment argument in order to work properly (although
this argument may be ignored in certain implementations).

### 3. Allocators require `try_allocate` rather than `allocate`

This is a subtle difference. The C++ Standard Library's `allocate(...)` function is free to throw exceptions on failure; leaving
non-throwing allocators up to an implementation's discretion. The `Allocator` concept in this library requires `try_allocate`, which
may return a null pointer on failure and must be non-throwing.

This allows for greater composition with other allocators, since an allocator that fails to allocate can be queried without requiring
`try`/`catch` statements (e.g. for fallback allocators)

The optional `allocate` function _may_ throw. If this is not defined, then `allocator_traits<Allocator>::allocate(...)` defines a custom
one that -- on failure -- calls a global out-of-memory handler.

### 4. Allocator info

The Allocator concept in this library optionally supports a member function named `infO()` that returns an `allocator_info` object.
This object is used for _naming_ the allocator so that it can be uniquely identified in case an error handler is called.

## Memory Management

The Allocators in this library follow strictly followed concepts, documented [here](http://bitwizeshift.github.io/bit-memory/modules.html)