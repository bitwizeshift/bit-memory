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
    /// satisfy the requirements of having an \c allocate and \c deallocate
    /// function with a specific signature.
    ///
    /// The rationale for this follows HP and SGI's rationale for removing
    /// the template type; it's easier to be dealing with raw bytes from a
    /// system, and giving it the single responsibility of allocating the
    /// data.
    ///
    /// The Allocator requirement expects the following.
    ///
    /// Provided:
    ///
    /// \c a - an instance of \c Allocator,
    /// \c s - the size of an allocation
    /// \c n - the alignment of the allocation
    /// \c o - the offset of the alignment
    /// \c p - an instance of \c std::ptrdiff_t
    /// \c v - a void pointer
    ///
    /// the following expressions must be well-formed with the expected
    /// side-effects:
    ///
    /// \code
    /// v = a.allocate( s, n, o, &p )
    /// \endcode
    /// \c a allocate at least \c s bytes aligned to the boundary \c n
    /// with offset \c o, and any additional padding must be stored in
    /// the address pointed to in \c p.
    ///
    /// \code
    /// a.deallocate( v, s )
    /// \endcode
    /// Deallocates the memory pointer to by \c v with the allocation size
    /// \c s. The size must match the original size requested by
    /// \c a.allocate. The memory originally pointed to by \c v may be
    /// reused in future calls to \c a.allocate.
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
    }

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

  } // namespace memory
} // namespace bit

#endif /* BIT_MEMORY_CONCEPTS_HPP */
