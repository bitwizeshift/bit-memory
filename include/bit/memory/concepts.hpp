/**
 * \file concepts.hpp
 *
 * \brief This header contains a mixture of documentation and type traits
 *
 * This header documents the various concepts used in this library. It also
 * provides type-traits that determine when a given type satisfies the
 * requirements.
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */
#ifndef BIT_MEMORY_CONCEPTS_HPP
#define BIT_MEMORY_CONCEPTS_HPP

///////////////////////////////////////////////////////////////////////////////
/// \concept BlockAllocator
///
/// \brief This concept defines the required interface and semantics
///        expected of a block allocator
///
/// The \c bit::memory BlockAllocator concept is used to distribute
/// memory blocks to allocators that can use them.
///
/// BlockAllocators are also used as upstream resources for
/// StreamingAllocators.
///
/// The size of a memory_block returned by a BlockAllocator is fixed at
/// construction time, and is not permitted to vary.
///
/// Requirements
///
/// - MoveConstructible
///
/// For type \c A to be \c BlockAllocator, it must satisfy the above
/// conditions as well as the following:
///
/// Provided
///
/// \c A - a BlockAllocator type
/// \c a - an instance of type \c A
/// \c b - a memory_block
///
/// the following expressions must be well-formed with the expected
/// side-effects:
///
/// \code
/// a.allocate_block()
/// \endcode
///
/// Allocates a \c memory_block of implementation-specific size
///
/// \code
/// a.deallocate_block( b );
/// \endcode
///////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------

///////////////////////////////////////////////////////////////////////////////
/// \concept Allocator
///
/// \brief This concept defines the required interface and semantics
///        expected of an allocator
///
/// The \c bit::memory Allocator concept varies from the C++ standard's
/// definition of the concept. One major difference is that an Allocator
/// is not templated on the type. Instead, an allocator simply must
/// satisfy the requirements of minimally supplying a non-throwing
///  \c try_allocate and \c deallocate function with a specific signature.
///
/// The rationale for this follows HP and SGI's rationale for removing
/// the template type; it's easier to be dealing with raw bytes from a
/// system, and giving it the single responsibility of allocating the
/// data.
///
/// Requirements
///
/// - MoveConstructible
///
/// For type \c A to be \c Allocator, it must satisfy the above
/// conditions as well as the following:
///
/// Provided
///
/// \c A - an Allocator type
/// \c a - an instance of type \c A
/// \c s - the size of an allocation
/// \c n - the alignment of the allocation
/// \c v - a void pointer
///
/// the following expressions must be well-formed with the expected
/// side-effects:
///
/// \code
/// v = a.try_allocate( s, n )
/// \endcode
/// \c a tries to allocate at least \c s bytes aligned to the boundary \c n.
/// This function returns \c nullptr on failure to allocate, otherwise
/// returns a pointer to data at least \c s bytes large, aligned to a \c n
/// byte boundary.
///
/// The expression \code a.try_allocate( s, n ) \endcode must be
/// non-throwing, otherwise it is undefined behaviour.
///
/// \code
/// a.deallocate( v, s )
/// \endcode
/// Deallocates the memory pointer to by \c v with the allocation size
/// \c s. The size must match the original size requested by
/// \c a.try_allocate, otherwise it is undefined behaviour.
/// Any use of \c v after a call to \c a.deallocate is undefined
/// behaviour.
///
/// \code
/// a.
/// \endcode
///
/// \code
/// a1 == a2
/// \endcode
///
/// returns true only if the storage allocated by the allocator a1 can be
/// deallocated through a2. Establishes reflexive, symmetric, and
/// transitive relationship. Does not throw exceptions.
///
/// \code
/// a1 != a2
/// \endcode
///
/// same as !(a1==a2)
///
///
/// Optionally:
///
/// \code
/// v = a.allocate( s, n )
/// \endcode
/// \c a allocates at least \c s bytes aligned to the boundary \c n.
///
/// The behaviour of this function is implementation-defined on failure
/// (may throw, may invoke out-of-memory handler, etc).
///
/// The default for this is to invoke out-of-memory handler on \c nullptr
/// if an implementation is not provided
///
/// \code
/// a.deallocate_all()
/// \endcode
/// \c a deallocates all entries inside of the allocator. Any existing pointers
/// are now invalidated
///
/// Default asserts.
///
/// \code
/// a.info()
/// \endcode
/// \c a returns an allocator_info object describing the allocator
///
/// Default returns "unknown allocator"
///
/// \code
/// a.max_size()
/// \endcode
///
/// Returns the maximum size the allocator can support
///
/// Default is \c std::numeric_limits<std::size_t>::max()
///
/// \code
/// a.min_size()
/// \endcode
///
/// Returns the minimum size the allocator can support
///
/// Default is \c 1
///
/// \code
/// A::is_stateless::value
/// \endcode
/// Returns a \c bool indicating  that allocator \c A is stateless, such
/// that any two instances of \c A will be identical. Note that
/// not every class where \c std::is_empty<A>::value is true is stateless;
/// this depends on whether there is external state that tracks information
/// about the allocator (e.g. static state stored outside of the class).
///
/// It is undefined-behaviour for this to be \c std::true_type for a
/// class \c A that is not actually stateless.
///
/// Default is \c std::false_type
///
/// \code
/// A::is_always_equal::value
/// \endcode
/// Returns a \c bool indicating whether two instances of the same allocator
/// will always compare equal.
///
/// Default is \c std::false_type
///
/// \code
/// A::default_alignment::value
/// \endcode
/// Determines the default-alignment of allocations from the given
/// allocator. This allows certain optimizations to be made by making
/// assumptions about the allocated results (such as avoiding the need
/// of storing padding-offset for alignment, if it's always guaranteed
/// to align to the correct boundary).
///
/// Default is \c 1.
///
/// \code
/// A::max_alignment::value
/// \endcode
/// Determines the maximum-alignment for allocations from the given
/// allocator. Any specified alignment argument that exceeds this value
/// results in undefined behaviour.
///
/// Default is \c alignof(std::max_align_t)
///
/// \code
/// A::can_truncate_deallocations
/// \endcode
/// Determines whether the allocator is capable of truncating deallocations
/// by clearing away larger chunks of data, rather than waiting for the every
/// deallocation call.
///////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------

///////////////////////////////////////////////////////////////////////////////
/// \concept ExtendedAllocator
///
/// \brief This concept defines the required interface and semantics
///        expected of an extended allocator
///
/// An \c ExtendedAllocator is also an \c Allocator that provides extended
/// functionality for tracking ownership, and providing the ability to
/// offset the allocations alignment.
///
/// Requirements
///
/// - Allocator
///
/// For type \c A to be \c ExtendedAllocator, it must satisfy the above
/// conditions as well as the following:
///
/// Provided
///
/// \c A - an Allocator type
/// \c a - an instance of type \c A
/// \c s - the size of an allocation
/// \c n - the alignment of the allocation
/// \c o - the offset for the alignment
/// \c v - a void pointer
///
/// the following expressions must be well-formed with the expected
/// side-effects:
///
/// \code
/// v = a.try_allocate( s, n, o )
/// \endcode
/// \c a tries to allocate at least \c s bytes aligned to the boundary \c n,
/// offset by \c o bytes.
///
/// The expression \code a.try_allocate( s, n ) \endcode must be
/// non-throwing, otherwise it is undefined behaviour.
///
/// \code
/// bool b = a.owns( p );
/// \endcode
///
/// \c a checks whether it owns the pointer \c p, returning the result.
///////////////////////////////////////////////////////////////////////////////

#endif /* BIT_MEMORY_CONCEPTS_HPP */
