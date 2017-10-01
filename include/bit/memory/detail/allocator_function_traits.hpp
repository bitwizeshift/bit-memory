/**
 * \file allocator_function_traits.hpp
 *
 * \brief This header contains internal trait functionality used for detecting
 *        functions and members of allocators
 *
 * \note This is an internal header file, included by other library headers.
 *       Do not attempt to use it directly.
 */
#ifndef BIT_MEMORY_DETAIL_ALLOCATOR_FUNCTION_TRAITS_HPP
#define BIT_MEMORY_DETAIL_ALLOCATOR_FUNCTION_TRAITS_HPP

#include "../allocator_info.hpp" // allocator_info
#include "void_t.hpp"  // void_t

#include <type_traits> // std::integral_constant, std::true_type, etc
#include <cstddef>     // std::max_align_t

namespace bit {
  namespace memory {
    namespace detail {

      template<typename T, typename = void>
      struct is_allocator : std::false_type{};

      template<typename T>
      struct is_allocator<T,void_t<
        decltype( std::declval<void*&>() = std::declval<T&>().try_allocate( std::declval<std::size_t>(), std::declval<std::size_t>() ) ),
        decltype( std::declval<T&>().deallocate( std::declval<void*>(), std::declval<std::size_t>() ) )>
      > : std::true_type{};

      //----------------------------------------------------------------------

      template<typename T, typename = void>
      struct is_extended_allocator : std::false_type{};

      template<typename T>
      struct is_extended_allocator<T,void_t<
        decltype( std::declval<void*&>() = std::declval<T&>().try_allocate( std::declval<std::size_t>(), std::declval<std::size_t>(), std::declval<std::size_t>() ) ),
        decltype( std::declval<bool&>() = std::declval<T&>().owns( std::declval<void*>() ) )>
      > : is_allocator<T>{};

      //----------------------------------------------------------------------
      // Allocator traits
      //----------------------------------------------------------------------

      template<typename T, typename = void>
      struct allocator_has_allocate : std::false_type{};

      template<typename T>
      struct allocator_has_allocate<T,
        void_t<decltype(std::declval<void*&>() = std::declval<T&>().allocate( std::declval<std::size_t>(), std::declval<std::size_t>() ) )>
      > : std::true_type{};

      //----------------------------------------------------------------------

      template<typename T, typename = void>
      struct allocator_has_deallocate_all : std::false_type{};

      template<typename T>
      struct allocator_has_deallocate_all<T,
        void_t<decltype(std::declval<T&>().deallocate_all())>
      > : std::true_type{};

      //----------------------------------------------------------------------

      template<typename T, typename = void>
      struct allocator_has_owns : std::false_type{};

      template<typename T>
      struct allocator_has_owns<T,
        void_t<decltype(std::declval<bool&>() = std::declval<T&>().owns( std::declval<const void*>() ))>
      > : std::true_type{};


      //----------------------------------------------------------------------

      template<typename T, typename = void>
      struct allocator_has_info : std::false_type{};

      template<typename T>
      struct allocator_has_info<T,
        void_t<decltype( std::declval<allocator_info>() = std::declval<const T&>().info() )>
      > : std::true_type{};

      //----------------------------------------------------------------------

      template<typename T, typename = void>
      struct allocator_has_max_size : std::false_type{};

      template<typename T>
      struct allocator_has_max_size<T,
        void_t<decltype( std::declval<std::size_t&>() = std::declval<const T&>().max_size() )>
      > : std::true_type{};

      //----------------------------------------------------------------------

      template<typename T, typename = void>
      struct allocator_has_min_size : std::false_type{};

      template<typename T>
      struct allocator_has_min_size<T,
        void_t<decltype( std::declval<std::size_t&>() = std::declval<const T&>().min_size() )>
      > : std::true_type{};

      //----------------------------------------------------------------------

      template<typename T, typename = void>
      struct allocator_is_always_equal : std::false_type{};

      template<typename T>
      struct allocator_is_always_equal<T,void_t<decltype(T::is_always_equal)>> : T::is_always_equal{};

      //----------------------------------------------------------------------

      template<typename T, typename = void>
      struct allocator_is_stateless : std::false_type{};

      template<typename T>
      struct allocator_is_stateless<T,void_t<decltype(T::is_stateless)>> : T::is_stateless{};

      //----------------------------------------------------------------------

      template<typename T, typename = void>
      struct allocator_default_alignment : std::integral_constant<std::size_t,1>{};

      template<typename T>
      struct allocator_default_alignment<T,void_t<decltype(T::default_alignment)>> : T::default_alignment{};

      //----------------------------------------------------------------------

      template<typename T, typename = void>
      struct allocator_max_alignment : std::integral_constant<std::size_t,alignof(std::max_align_t)>{};

      template<typename T>
      struct allocator_max_alignment<T,void_t<decltype(T::max_alignment)>> : std::integral_constant<std::size_t,alignof(std::max_align_t)>{};

      //----------------------------------------------------------------------

      template<typename T, typename = void>
      struct allocator_can_truncate_deallocations : std::integral_constant<bool,false>{};

      template<typename T>
      struct allocator_can_truncate_deallocations<T,void_t<decltype(T::can_truncate_deallocations)>> : T::can_truncate_deallocations{};

    } // namespace detail
  } // namespace memory
} // namespace bit

#endif /* BIT_MEMORY_DETAIL_ALLOCATOR_FUNCTION_TRAITS_HPP */
