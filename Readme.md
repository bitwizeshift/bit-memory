# `bit::memory` Memory library

[![Build Status](https://travis-ci.org/bitwizeshift/bit-memory.svg?branch=master)](https://travis-ci.org/bitwizeshift/bit-memory)
[![Build status](https://ci.appveyor.com/api/projects/status/ou5sraydky6tjxv9?svg=true)](https://ci.appveyor.com/project/bitwizeshift/bit-memory)
[![Github Issues](https://img.shields.io/github/issues/bitwizeshift/bit-memory.svg)](http://github.com/bitwizeshift/bit-memory/issues)
[![Tested Compilers](https://img.shields.io/badge/compilers-gcc%20%7C%20clang%20%7C%20msvc-blue.svg)](#tested-compilers)
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

### 1. Allocators are heterogeneous

The C++ Standard Library's `Allocator` concept defines every `Allocator` to be homogeneous in its allocations; always allocating
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

The Allocator concept in this library optionally supports a member function named `info()` that returns an `allocator_info` object.
This object is used for _naming_ the allocator so that it can be uniquely identified in case an error handler is called.

## Using `bit::memory`

### Building & Installing

Building `bit::memory` is simple, and only requires a CMake version greater than 3.1, and a compatible compiler.

Fist, make a directory to build the library in, then build with CMake

```bash
mkdir build
cd build

cmake . -DBIT_MEMORY_BUILD_UNIT_TESTS=Off -DBIT_MEMORY_BUILD_INDEPENDENCE_TESTS=Off  # ... any additional toolchain parameters ...
cmake --build .
```

Omitting the `-DBIT_MEMORY_BUILD_UNIT_TESTS=Off` will build unit tests; likewise `DBIT_MEMORY_BUILD_INDEPENDENCE_TESTS=Off` will
build header-independence checks (building each header independently to ensure there are no header-order based transitive dependencies).

To install, run:

```bash
cmake --build . --target install
```

To change the output directory of the installation, you can export `BIT_HOME` to install to a specific directory

```bash
export BIT_HOME=/etc/bit
cmake --build . --target install
```

will install into `/etc/bit/`.

### Through a `cmake` subdirectory

Clone/copy/subtree the contents of this repository to a subdirectory, and `add_subdirectory` the directory containing `bit::memory`.

To add a dependency to the library, just add `target_link_libraries(<your target> [PUBLIC|INTERFACE|PRIVATE] bit::memory)`

## <a name="tested-compilers"></a>Tested Compilers

The following compilers are currently being tested through continuous integration with [Travis](https://travis-ci.org/bitwizeshift/bit-memory) and [AppVeyor](https://ci.appveyor.com/project/bitwizeshift/bit-memory/)

Note that `bit-memory` only works on compiler that provide proper conformance for c++14

| Compiler              | Operating System                   |
|-----------------------|------------------------------------|
| g++ 6.3.0             | Ubuntu 14.04.3 TLS                 |
| g++ 7.2.0             | Ubuntu 14.04.3 TLS                 |
| clang++ 3.9.0         | Ubuntu 14.04.3 TLS                 |
| clang++ 4.0.1         | Ubuntu 14.04.3 TLS                 |
| clang++ 5.0           | Ubuntu 14.04.3 TLS                 |
| g++ 7.2.0             | Ubuntu 14.04.3 TLS                 |
| clang xcode 7.3       | Darwin Kernel 15.6.0 (OSX 10.11.6) |
| clang xcode 8.0       | Darwin Kernel 15.6.0 (OSX 10.11.6) |
| clang xcode 8.1       | Darwin Kernel 16.1.0 (OSX 10.12.1) |
| clang xcode 8.2       | Darwin Kernel 16.1.0 (OSX 10.12.1) |
| clang xcode 8.3       | Darwin Kernel 16.6.0 (OSX 10.12.5) |
| clang xcode 9.0       | Darwin Kernel 16.7.0 (OSX 10.12.6) |
| Visual Studio 2017    | Windows Server 2016 (x64)          |

**Note:** Unfortunately, support for Visual Studio 2015 had to be dropped due to issues properly performing SFINAE
with some non-owning type-erased classes (`any_allocator` and `any_block_allocator`).
It has just not been worth the hassle to attempt to support a non-standards-conforming compiler. Fortunately,
Visual Studios 2017 is still supported, and will continue to be.

## <a name="license"></a>License

<img align="right" src="http://opensource.org/trademarks/opensource/OSI-Approved-License-100x137.png">

The class is licensed under the [MIT License](http://opensource.org/licenses/MIT):

Copyright &copy; 2018 Matthew Rodusek

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
