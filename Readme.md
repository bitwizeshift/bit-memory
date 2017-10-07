# `bit::memory` Memory library

[![Build Status](https://travis-ci.org/bitwizeshift/bit-memory.svg?branch=master)](https://travis-ci.org/bitwizeshift/bit-memory)
[![Build status](https://ci.appveyor.com/api/projects/status/q3k3qns2amtdhliy?svg=true)](https://ci.appveyor.com/project/bitwizeshift/bit-memory)
[![Github Issues](https://img.shields.io/github/issues/bitwizeshift/bit-memory.svg)](http://github.com/bitwizeshift/bit-memory/issues)
[![Tested Compilers](https://img.shields.io/badge/compilers-gcc%20%7C%20clang-blue.svg)](#tested-compilers)
[![Documentation](https://img.shields.io/badge/docs-doxygen-blue.svg)](http://bitwizeshift.github.io/bit-memory)
[![GitHub License](https://img.shields.io/badge/license-MIT-blue.svg)](https://raw.githubusercontent.com/bitwizeshift/bit-memory/master/LICENSE.md)
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

## Memory Management

The Allocators in this library are designed around a few crucial concepts:

- [`Stateless`](#stateless)
- [`BlockAllocator`](#block_allocator)
- [`Allocator`](#allocator)
- [`ExtendedAllocator`](#extended-allocator)

### <a name='stateless'></a>`Stateless` Concept

This simple concept is significant for managing lightweight types, since it
enables EBO (Empty Base-class Optimization) with ellided duplicates.

A `Stateless` type must satisfy the following C++ concepts:

- [`DefaultConstructible`](http://en.cppreference.com/w/cpp/concept/DefaultConstructible)
- [`CopyConstructible`](http://en.cppreference.com/w/cpp/concept/CopyConstructible)
- [`MoveConstructible`](http://en.cppreference.com/w/cpp/concept/MoveConstructible)
- [`CopyAssignable`](http://en.cppreference.com/w/cpp/concept/CopyAssignable)
- [`MoveAssignable`](http://en.cppreference.com/w/cpp/concept/MoveAssignable)
- [`EqualityComparable`](http://en.cppreference.com/w/cpp/concept/EqualityComparable)

Additionaly, the following criteria must be met:

- `std::is_empty<Stateless>::value` must be `true`,
- `std::is_trivially_constructible<Stateless>::value` must be `true`, and
- `std::is_trivially_destructible<Stateless>::value` must be `true`
- `s1 == s2` for any `Stateless s1, s2` must always be `true`
- `s1 != s2` for any `Stateless s1, s2` must always be `false`

Semantically, a `Stateless` type simply must not have or modify any internal or external state.
By not containing or modifying any form of state, it allows for only one instance of a base class to
exist in a multiple-inheritance structure in order to leverage EBO.

### <a name='block_allocator'></a>`BlockAllocator` Concept

A `BlockAllocator` is a very simple form of allocator that only ever allocates blocks of set size.
The `BlockAllocator` is the **upstream-resource** for arena-based allocators. A `BlockAllocator`
may also be `Stateless`.

For type `A` to be `BlockAllocator`, it must satisfy the above
conditions as well as the following:

Provided
`A` - a `BlockAllocator` type
`a` - an instance of type `A`
`b` - a `memory_block`

the following expressions must be well-formed with the expected side-effects:

#### Required

```c++
a.allocate_block()
```

Allocates a `memory_block` of implementation-specific size

```c++
a.deallocate_block( b );
```

Deallocates the `memory_block`, freeing the resource

#### Optional

```c++
A::is_stateless
```

Determines that `A` is a `Stateless` object. It is undefined behaviour if `A` does not
actually conform to `Stateless`.

```c++
A::block_alignment::value
```

`A::block_alignment::value` indicates the guaranteed alignment-size of every block allocated from `A`.
It is undefined behaviour if the alignment is less than this value.

This is used for optimizations in `Allocator`s that use `BlockAllocator` as an upstream resource. For example,
if the max-supported-alignment of an `Allocator` is less-than-or-equal to the guaranteed alignment of a
`BlockAllocator`'s allocated block, then there is no need to store offset-information for each allocation, since
offsets will not be required for the allocations.

```c++
A::block_size::value
```

`A::block_size::value` indicates the guaranteed size of every block allocated from `A`.
It is undefined behaviour if the size is less than this value

Similar to `A::block_alignment`, this is used for optimizations in `Allocator`s that use `BlockAllocator` as an upstream resource

### <a name='allocator'></a>`Allocator` Concept

The `bit::memory` `Allocator` concept varies from the C++ standard's
definition of the concept. One major difference is that an `Allocator`
is not templated on the type. Instead, an `Allocator` simply must
satisfy the requirements of minimally supplying a non-throwing
`try_allocate` and `deallocate` function with a specific signature.

The rationale for this follows HP and SGI's rationale for removing
the template type; it's easier to be dealing with raw bytes from a
system, and giving it the single responsibility of allocating the
data.

As with `BlockAllocator`, an `Allocator` may additionally be `Stateless`.

For type `A` to be an `Allocator`, it must satisfy the following:

Provided

`A` - an `Allocator` type
`a` - an instance of type `A`
`s` - the size of an allocation
`n` - the alignment of the allocation
`v` - a void pointer

the following expressions must be well-formed with the expected side-effects:

#### Required

```c++
v = a.try_allocate( s, n );
```

`a` tries to allocate at least `s` bytes aligned to the boundary `n`.

This function returns `nullptr` on failure to allocate, otherwise
returns a pointer to data at least `s` bytes large, aligned to a `n`
byte boundary.

The expression `a.try_allocate( s, n )` must be non-throwing,
otherwise it is undefined behaviour.

```c++
a.deallocate( v, s );
```

Deallocates the memory pointer to by `v` with the allocation size
`s`. The size and pointer must match the original values from a call to
`a.try_allocate` or `a.allocate`, otherwise it is undefined behaviour.

Any use of `v` after a call to `a.deallocate` is undefined behaviour.

#### Optional

```c++
v = a.allocate( s, n )
```
`a` allocates at least `s` bytes aligned to the boundary `n`.

The behaviour of this function is implementation-defined on failure
(may throw, may invoke out-of-memory handler, etc).

The default for this is to invoke out-of-memory handler on `nullptr`
if an implementation is not provided

```c++
a.deallocate_all()
```

`a` deallocates all entries inside of the allocator. Any existing pointers
are now invalidated

Default asserts.

```c++
a.info()
```
`a` returns an allocator_info object describing the allocator

Default returns `"unknown allocator"`

```c++
a.max_size()
```

Returns the maximum size the allocator can support

Default is `std::numeric_limits<std::size_t>::max()`

```c++
a.min_size()
```

Returns the minimum size the allocator can support

Default is `1`

```c++
A::is_stateless::value
```

Returns a `bool` indicating that allocator `A` is `Stateless`

It is undefined-behaviour for this to be `std::true_type` for a
class `A` that is not actually stateless.

Default is `std::false_type`

```c++
A::is_always_equal::value
```

Returns a `bool` indicating whether two instances of the same allocator
will always compare equal.

Default is `std::false_type`

```c++
A::default_alignment::value
```

Determines the default-alignment of allocations from the given
allocator. This allows certain optimizations to be made by making
assumptions about the allocated results (such as avoiding the need
of storing padding-offset for alignment, if it's always guaranteed
to align to the correct boundary).

Default is `1`.

```c++
A::max_alignment::value
```

Determines the maximum-alignment for allocations from the given
allocator. Any specified alignment argument that exceeds this value
results in undefined behaviour.

Default is `alignof(std::max_align_t)`

```c++
A::can_truncate_deallocations
```

Determines whether the allocator is capable of truncating deallocations
by clearing away larger chunks of data, rather than waiting for the every
deallocation call.

```c++
bool b = a.owns( p );
```

`a` checks whether it owns the pointer `p`, returning the result.

### <a name='extended-allocator'></a>`ExtendedAllocator` Concept

This concept defines the required interface and semantics expected of an
extended allocator

An `ExtendedAllocator` is also an `Allocator` that provides extended
ability to offset the allocations alignment.

**Requirements**

[`Allocator`](#allocator)

For type `A` to be `ExtendedAllocator`, it must satisfy the above
conditions as well as the following:

Provided

`A` - an `Allocator` type
`a` - an instance of type `A`
`s` - the size of an allocation
`n` - the alignment of the allocation
`o` - the offset for the alignment
`v` - a void pointer

the following expressions must be well-formed with the expected
side-effects:

#### Required

```c++
v = a.try_allocate( s, n, o );
```

`a` tries to allocate at least `s` bytes aligned to the boundary `n`,
offset by `o` bytes.

The expression ```c++ a.try_allocate( s, n, o ) ``` must be
non-throwing, otherwise it is undefined behaviour.

#### Optional

```c++
v = a.allocate( s, n, o );
```

`a` allocates at least `s` bytes aligned to the boundary `n`, offset by `o`

The behaviour of this function is implementation-defined on failure
(may throw, may invoke out-of-memory handler, etc).

The default for this is to invoke out-of-memory handler on `nullptr`
if an implementation is not provided
