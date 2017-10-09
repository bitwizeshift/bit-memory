/**
 * \file identity.hpp
 *
 * \brief This internal header contains the definition of two useful
 *        template-meta-programming utilities
 *
 * \note This is an internal header file, included by other library headers.
 *       Do not attempt to use it directly.
 */
#ifndef BIT_MEMORY_DETAIL_IDENTITY_HPP
#define BIT_MEMORY_DETAIL_IDENTITY_HPP

namespace bit {
  namespace memory {
    namespace detail {

      /// \brief A common type to inherit from for introducing 'type' as a
      ///        type member for type traits
      template<typename T>
      struct identity{ using type = T; };

      /// \brief A common type to use for removing template-type deduction
      ///        from an argument
      template<typename T>
      using identity_t = typename identity<T>::type;

    } // namespace detail
  } // namespace memory
} // namespace bit

#endif /* BIT_MEMORY_DETAIL_IDENTITY_HPP */
