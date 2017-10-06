/**
 * \file freelist.hpp
 *
 * \brief TODO: Add description
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */
#ifndef BIT_MEMORY_FREELIST_HPP
#define BIT_MEMORY_FREELIST_HPP

#include "memory.hpp" // align_of

#include <cstddef> // std::size_t
#include <cassert> // assert

namespace bit {
  namespace memory {

    class freelist
    {
      //-----------------------------------------------------------------------
      // Constructors
      //-----------------------------------------------------------------------
    public:

      constexpr freelist() noexcept;

      freelist( const freelist& other ) = delete;

      freelist( freelist&& other ) noexcept = default;

      //-----------------------------------------------------------------------

      freelist& operator=( freelist&& other ) = delete;

      freelist& operator=( const freelist& other ) = delete;


      //----------------------------------------------------------------------
      // Modifiers
      //----------------------------------------------------------------------
    public:

      /// \brief Swaps this with another block cache
      ///
      /// \param other the other memory_block_cache to swap with
      void swap( freelist& other ) noexcept;

      //----------------------------------------------------------------------
      // Observers
      //----------------------------------------------------------------------
    public:

      /// \brief Returns whether or not this memory_block_cache is empty
      ///
      /// \return \c true if this memory_block_cache is empty, \c false otherwise
      bool empty() const noexcept;

      /// \brief Returns the number of entries in this freelist
      ///
      /// \return the number of entries in this freelist
      std::size_t size() const noexcept;

      //----------------------------------------------------------------------
      // Caching
      //----------------------------------------------------------------------
    public:

      /// \brief Requests raw memory from the freelist, if any exists
      ///
      /// The size of the returned instance is not known to the freelist
      ///
      /// \return pointer to memory, if it exists
      void* request() noexcept;

      /// \brief Steals raw meory from an existing freelist
      void steal( freelist& other ) noexcept;

      /// \brief Stores raw memory into this freelist
      ///
      /// \pre The pointer \p p must point to memory of at least
      ///      \c sizeof(void*) bytes, and must be suitably aligned to support
      ///      pointer types.
      ///
      /// \param p pointer to the raw memory to store
      void store( void* p ) noexcept;

      //-----------------------------------------------------------------------
      // Private Members
      //-----------------------------------------------------------------------
    private:

      void* m_head;
    };

  } // namespace memory
} // namespace bit

#include "detail/freelist.inl"

#endif /* BIT_MEMORY_FREELIST_HPP */
