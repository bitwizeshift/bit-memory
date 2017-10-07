/**
 * \file uninitialized_storage.hpp
 *
 * \brief This header defines various uninitialized storage functionality
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */
#ifndef BIT_MEMORY_UNINITIALIZED_STORAGE_HPP
#define BIT_MEMORY_UNINITIALIZED_STORAGE_HPP

#include <tuple>   // std::forward_as_tuple, std::tuple
#include <utility> // std::forward
#include <cstddef> // std::size_t
#include <new>     // placement new

namespace bit {
  namespace memory {

    //------------------------------------------------------------------------
    // Construction
    //------------------------------------------------------------------------

    /// \brief Constructs an instance of type \p T with the given \p args
    ///        at the memory location specified in \p ptr
    ///
    /// \param ptr     The memory location to construct into
    /// \param args... The arguments to supply to T's constructor
    /// \return Pointer to the initialized memory (cast of \p ptr)
    template<typename T, typename...Args>
    T* uninitialized_construct_at( void* p, Args&&...args );

    /// \brief Constructs an array of the specified size at pointer \p p
    ///
    /// \tparam T the type to construct
    /// \param p the pointer to construct the array at
    /// \param n the size of the array
    /// \return the pointer to the constructed array
    template<typename T>
    T* uninitialized_construct_array_at( void* p, std::size_t n );

    /// \brief Constructs an instance of type \p T from a given \p tuple
    ///
    /// \tparam T the type to construct
    /// \param ptr the pointer to construct the type at
    /// \param tuple the tuple to unpack as arguments to \p T
    /// \return pointer to the constructed element
    template<typename T, typename Tuple>
    T* uninitialized_construct_from_tuple( void* p, Tuple&& tuple );

    /// \brief Constructs a given type from a tuple
    ///
    /// \tparam T the type to construct
    /// \param tuple the tuple to unpack for arguments
    /// \return the construct type
    template<typename T, typename Tuple>
    T make_from_tuple( Tuple&& tuple );

    //------------------------------------------------------------------------
    // Destruction
    //------------------------------------------------------------------------

    /// \brief Destroys the type at the given pointer \p p
    ///
    /// \param p the pointer to the instance to destroy
    template<typename T>
    void destroy_at( T* p );

    /// \brief Destroys the array of instances at the specified pointer \p p
    ///
    /// \param p the pointer to the array to destroy
    /// \param n the size of the array
    template<typename T>
    void destroy_array_at( T* p, std::size_t n );

  } // namespace memory
} // namespace bit

#include "detail/uninitialized_storage.inl"

#endif /* BIT_MEMORY_UNINITIALIZED_STORAGE_HPP */
