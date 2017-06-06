/**
 * \file smart_ptr.hpp
 *
 * \brief This header contains templates for various different smart pointers.
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */

#ifndef BIT_MEMORY_SMART_PTR_HPP
#define BIT_MEMORY_SMART_PTR_HPP

#include <memory>      // std::unique_ptr, std::shared_ptr
#include <type_traits> // std::enable_if, ...
#include <utility>     // std::swap

// IWYU pragma: begin_exports
#include "detail/smart_ptr/observer_ptr.hpp"
#include "detail/smart_ptr/scoped_ptr.hpp"
#include "detail/smart_ptr/fat_ptr.hpp"
// IWYU pragma: end_exports

namespace bit {
  namespace memory {

//    class allocator;
//
//    template<typename T>
//    using unique_ptr = std::unique_ptr<T>;
//
//    template<typename T>
//    using shared_ptr = std::shared_ptr<T>;
//
//    template<typename T>
//    struct allocate_unique_dispatch
//    {
//      template<typename...Args>
//      static unique_ptr<T> allocate( allocator alloc, Args&&...args );
//    };
//
//    template<typename T>
//    struct allocate_shared_dispatch
//    {
//      template<typename...Args>
//      static shared_ptr<T> allocate( allocator alloc, Args&&...args );
//    };
//
//    template<typename T, typename...Args>
//    unique_ptr<T> allocate_unique( allocator alloc, Args&&...args );
//
//    template<typename T, typename...Args>
//    unique_ptr<T> allocate_shared( allocator alloc, Args&&...args );

  } // namespace memory
} // namespace bit

#endif /* BIT_MEMORY_SMART_PTR_HPP */
