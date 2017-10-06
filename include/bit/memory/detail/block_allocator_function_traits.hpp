/**
 * \file block_allocator_function_traits.hpp
 *
 * \brief This header contains internal trait functionality used for detecting
 *        functions and members of block allocators
 *
 * \note This is an internal header file, included by other library headers.
 *       Do not attempt to use it directly.
 */
#ifndef BIT_MEMORY_DETAIL_BLOCK_ALLOCATOR_FUNCTION_TRAITS_HPP
#define BIT_MEMORY_DETAIL_BLOCK_ALLOCATOR_FUNCTION_TRAITS_HPP

#include "void_t.hpp"          // void_t
#include "../memory_block.hpp" // memory_block

#include <cstddef>     // std::size_t
#include <type_traits> // std::true_type, std::integral_constant, etc

namespace bit {
  namespace memory {
    namespace detail {

      template<typename T, typename = void>
      struct is_block_allocator : std::false_type{};

      template<typename T>
      struct is_block_allocator<T,void_t<
        decltype( std::declval<T&>().allocate_block() ),
        decltype( std::declval<T&>().deallocate_block(std::declval<memory_block>() ) )>
      > : std::true_type{};


      template<typename T, typename = void>
      struct block_allocator_has_block_alignment : std::false_type{};

      template<typename T>
      struct block_allocator_has_block_alignment<T,void_t<
        decltype(T::block_alignment)>
      > : std::true_type{};

      template<typename T, typename = void>
      struct block_allocator_has_block_size : std::false_type{};

      template<typename T>
      struct block_allocator_has_block_size<T,void_t<
        decltype(T::block_size)>
      > : std::true_type{};

      template<typename T, typename = void>
      struct block_allocator_is_stateless : std::false_type{};

      template<typename T>
      struct block_allocator_is_stateless<T,void_t<decltype(T::is_stateless)>> : T::is_stateless{};

    } // namespace detail
  } // namespace memory
} // namespace bit

#endif /* BIT_MEMORY_DETAIL_BLOCK_ALLOCATOR_FUNCTION_TRAITS_HPP */
