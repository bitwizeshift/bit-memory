/**
 * \file void_t.hpp
 *
 * \brief This header contains the definition for \c void_t to be used for
 *        various template type-traits
 *
 * \note This is an internal header file, included by other library headers.
 *       Do not attempt to use it directly.
 */
#ifndef BIT_MEMORY_DETAIL_VOID_T_HPP
#define BIT_MEMORY_DETAIL_VOID_T_HPP

namespace bit {
  namespace memory {
    namespace detail {

      template<typename...>
      struct voidify{ using type = void; };

      template<typename...Ts>
      using void_t = typename voidify<Ts...>::type;

    } // namespace detail
  } // namespace memory
} // namespace bit

#endif /* BIT_MEMORY_DETAIL_VOID_T_HPP */
