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

#include "memory_block.hpp" // memory_block

#include <type_traits> // std::declval

namespace bit {
  namespace memory {

    //////////////////////////////////////////////////////////////////////////
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
    /// - MoveAssignable
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
    //////////////////////////////////////////////////////////////////////////

    //------------------------------------------------------------------------

    //////////////////////////////////////////////////////////////////////////
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
    /// - MoveAssignable
    ///
    /// For type \c A to be \c BlockAllocator, it must satisfy the above
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
    /// a.name()
    /// \endcode
    /// \c a returns the name of the current allocator. The name me be a
    /// hardcoded ansi-string, or a user-supplied one stored as part of the
    /// allocator data.
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
    /// a.used()
    /// \endcode
    ///
    /// Returns the current number of bytes allocated by this allocator
    ///
    /// Default is \c 0
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
    //////////////////////////////////////////////////////////////////////////

    namespace detail {

      template<typename...>
      struct voidify{ using type = void; };

      template<typename...Ts>
      using void_t = typename voidify<Ts...>::type;

      //----------------------------------------------------------------------

      template<typename T, typename = void>
      struct is_block_allocator : std::false_type{};

      template<typename T>
      struct is_block_allocator<T,void_t<
        decltype( std::declval<T&>().allocate_block() ),
        decltype( std::declval<T&>().deallocate_block(std::declval<memory_block>() ) )>
      > : std::true_type{};

      //----------------------------------------------------------------------

      template<typename T, typename = void>
      struct is_allocator : std::false_type{};

      template<typename T>
      struct is_allocator<T,void_t<
        decltype( std::declval<void*&>() = std::declval<T&>().allocate( std::declval<std::size_t>(), std::declval<std::size_t>(), std::declval<std::size_t>(), std::declval<std::ptrdiff_t*>() ) ),
        decltype( std::declval<T&>().deallocate( std::declval<void*>(), std::declval<std::size_t>() ) )>
      > : std::true_type{};

      //----------------------------------------------------------------------

      template<typename T, typename = void>
      struct has_construct : std::false_type{};

      template<typename T>
      struct has_construct<T,
        void_t<decltype(std::declval<T>().template construct<int>( std::declval<std::size_t>(), std::declval<std::size_t>(), std::declval<std::size_t>() ))>
      > : std::true_type{};

      //----------------------------------------------------------------------

      template<typename T, typename = void>
      struct has_construct_array : std::false_type{};

      template<typename T>
      struct has_construct_array<T,
        void_t<decltype(std::declval<T>().template construct_array<int>( std::declval<std::size_t>() ))>
      > : std::true_type{};

      //----------------------------------------------------------------------

      template<typename T, typename = void>
      struct has_destruct : std::false_type{};

      template<typename T>
      struct has_destruct<T,
        void_t<decltype( std::declval<T>().destruct( std::declval<int*>() ) )>
      > : std::true_type{};

      //----------------------------------------------------------------------

      template<typename T, typename = void>
      struct has_destruct_array : std::false_type{};

      template<typename T>
      struct has_destruct_array<T,
        void_t<decltype( std::declval<T>().destruct_array( std::declval<int*>(), std::declval<std::size_t>() ))>
      > : std::true_type{};

      //----------------------------------------------------------------------

      template<typename T, typename = void>
      struct has_name : std::false_type{};

      template<typename T>
      struct has_name<T,
        void_t<decltype( std::declval<const char*>() = std::declval<T>().name() )>
      > : std::true_type{};
    } // namespace detail

    //----------------------------------------------------------------------

    /// \brief Does the given type satisfy the BlockAllocator requirements?
    /// \tparam T the type to check
    ///
    /// The result is aliased as \c ::value
    template<typename T>
    using is_block_allocator = detail::is_block_allocator<T>;

    /// \brief Convenience helper to extract the boolean value \c ::value
    ///        from \ref is_block_allocator
    template<typename T>
    constexpr bool is_block_allocator_v = is_block_allocator<T>::value;

    //----------------------------------------------------------------------

    /// \brief Does the given type satisfy the Allocator requirements?
    /// \tparam T the type to check
    ///
    /// The result is aliased as \c ::value
    template<typename T>
    using is_allocator = detail::is_allocator<T>;

    /// \brief Convenience helper to extract the boolean value \c ::value
    ///        from \ref is_allocator
    template<typename T>
    constexpr bool is_allocator_v = is_allocator<T>::value;

    //----------------------------------------------------------------------

    template<typename T>
    using has_construct = detail::has_construct<T>;

    template<typename T>
    constexpr bool has_construct_v = detail::has_construct<T>::value;

    //----------------------------------------------------------------------

    template<typename T>
    using has_construct_array = detail::has_construct_array<T>;

    template<typename T>
    constexpr bool has_construct_array_v = detail::has_construct_array<T>::value;

    //----------------------------------------------------------------------

    template<typename T>
    using has_destruct = detail::has_destruct<T>;

    template<typename T>
    constexpr bool has_destruct_v = detail::has_destruct<T>::value;

    //----------------------------------------------------------------------

    template<typename T>
    using has_destruct_array = detail::has_destruct_array<T>;

    template<typename T>
    constexpr bool has_destruct_array_v = detail::has_destruct_array<T>::value;

    //----------------------------------------------------------------------


    template<typename T>
    using has_name = detail::has_name<T>;

    template<typename T>
    constexpr bool has_name_v = detail::has_name<T>::value;

  } // namespace memory
} // namespace bit

#endif /* BIT_MEMORY_CONCEPTS_HPP */
