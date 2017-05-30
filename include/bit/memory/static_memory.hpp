/**
 * \file static_memory.hpp
 *
 * \brief This header contains the definition for an allocator that can only
 *        allocate a single static block
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */
#ifndef BIT_MEMORY_STATIC_MEMORY_HPP
#define BIT_MEMORY_STATIC_MEMORY_HPP

#include "memory.hpp"       // owner
#include "memory_block.hpp" // memory_block
#include <cstdlib>          // std::size_t

namespace bit {
  namespace memory {

    //////////////////////////////////////////////////////////////////////////
    /// \brief This is a block allocator that dolls out a single block of
    ///        static memory
    ///
    /// Repeated allocations will result in \c nullblock entries.
    ///
    /// This type is templated on both the size of the static block, and an
    /// additional Tag type that may be used to create individual instantiations
    ///
    /// \note Since this type creates static storage data, two instantiations
    ///       with the same size/tag will point to the same data -- hence the
    ///       ability to use type-tagging to produce unique instances.
    ///
    /// \tparam Size the size of the static allocation
    /// \tparam Tag the type used for a tag
    ///
    /// \satisfies BlockAllocator
    //////////////////////////////////////////////////////////////////////////
    template<std::size_t Size, typename Tag = void>
    class static_block_allocator
    {
      //----------------------------------------------------------------------
      // Public Members
      //----------------------------------------------------------------------
    public:

      static constexpr std::size_t block_size = Size;

      //----------------------------------------------------------------------
      // Constructor
      //----------------------------------------------------------------------
    public:

      /// \brief Constructs a static_block_allocator from another allocator
      explicit static_block_allocator() noexcept;

      /// Deleted copy constructor
      static_block_allocator( const static_block_allocator& other ) = delete;

      /// \brief Move-constructs a static_block_allocator from another one
      ///
      /// \param other the other allocator to move
      static_block_allocator( static_block_allocator&& other ) noexcept = default;

      //----------------------------------------------------------------------

      /// Deleted copy assignment
      static_block_allocator& operator=( const static_block_allocator& other ) = delete;

      /// \brief Move-assigns a static_block_allocator from another one
      ///
      /// \param other the other allocator to move
      /// \return a reference to \c (*this)
      static_block_allocator& operator=( static_block_allocator&& other ) noexcept = default;

      //----------------------------------------------------------------------
      // Block Allocations
      //----------------------------------------------------------------------
    public:

      /// \brief Allocates a single memory block of size \c Size
      ///
      /// This function can only be called once, since it allocates from the
      /// static storage
      ///
      /// \return a newly allocated memory_block
      owner<memory_block> allocate_block() noexcept;

      /// \brief Deallocates a given memory_block
      ///
      /// \param block the block to deallocate
      void deallocate_block( owner<memory_block> block ) noexcept;

      //----------------------------------------------------------------------
      // Private Members
      //----------------------------------------------------------------------
    private:

      static unsigned char storage[Size];

      bool m_is_allocated; ///< is this block allocated?
    };

  } // namespace memory
} // namespace bit

#include "detail/static_memory.inl"

#endif /* BIT_MEMORY_STATIC_MEMORY_HPP */
