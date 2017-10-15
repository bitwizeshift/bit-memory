/**
 * \file cached_block_allocator.hpp
 *
 * \brief This header contains an allocator adapter that caches previously
 *        allocated blocks
 *
 * \note This is an internal header file, included by other library headers.
 *       Do not attempt to use it directly.
 */
#ifndef BIT_MEMORY_BLOCK_ALLOCATORS_CACHED_BLOCK_ALLOCATOR_HPP
#define BIT_MEMORY_BLOCK_ALLOCATORS_CACHED_BLOCK_ALLOCATOR_HPP

#include "../../owner.hpp"              // owner
#include "../../memory_block.hpp"       // memory_block
#include "../../memory_block_cache.hpp" // memory_block_cache

#include <utility>     // std::forward
#include <type_traits> // std::true_type, std::false_type

namespace bit {
  namespace memory {
    namespace detail {

      /////////////////////////////////////////////////////////////////////////
      /// \brief This class is an adapter around a given block allocator that
      ///        caches any allocated blocks for reuse.
      ///
      /// Any constructors for the underlying BlockAllocator are propagated
      /// to the cache_block_allocator constructor -- allowing it to be
      /// constructed as though it is the underlying type.
      ///
      /// \tparam BlockAllocator the block allocator to adapt around
      /// \satisfies{BlockAllocator}
      /////////////////////////////////////////////////////////////////////////
      template<typename BlockAllocator>
      class cached_block_allocator : public BlockAllocator
      {
        //---------------------------------------------------------------------
        // Constructor / Destructor / Assignment
        //---------------------------------------------------------------------
      public:

        /// \brief Constructs a cached_block_allocator from another allocator
        ///
        /// \param alloc the underlying allocator to construct out of
        template<typename Arg0, typename...Args, typename = std::enable_if_t<!std::is_same<std::decay_t<Arg0>,cached_block_allocator>::value>>
        explicit cached_block_allocator( Arg0&& arg0, Args&&...args );

        /// \brief Move-constructs a cached_block_allocator from another one
        ///
        /// \param other the other allocator to move
        cached_block_allocator( cached_block_allocator&& other ) = default;

        // Deleted copy constructor
        cached_block_allocator( const cached_block_allocator& other ) = delete;

        //---------------------------------------------------------------------

        /// \brief Destructs this cached_block_allocator and destroys all
        ///        cached entries
        ~cached_block_allocator();

        //---------------------------------------------------------------------

        // Deleted move assignment
        cached_block_allocator& operator=( cached_block_allocator&& other ) = delete;

        // Deleted copy assignment
        cached_block_allocator& operator=( const cached_block_allocator& other ) = delete;

        //---------------------------------------------------------------------
        // Block Allocations
        //---------------------------------------------------------------------
      public:

        /// \brief Allocates a block, drawing from a cache if a cached block
        ///        exists
        ///
        /// \return the memory block
        owner<memory_block> allocate_block();

        /// \brief Deallocates a specified memory block
        ///
        /// This function stores the memory block in the cache for later use
        ///
        /// \param block the block to deallocate
        void deallocate_block( owner<memory_block> block );

        //---------------------------------------------------------------------
        // Private Members
        //---------------------------------------------------------------------
      private:

        memory_block_cache m_cache;
      };

    } // namespace detail

    template<typename T>
    struct is_cached_block_allocator : std::false_type{};

    template<typename T>
    struct is_cached_block_allocator<detail::cached_block_allocator<T>>
      : std::true_type{};
  } // namespace memory
} // namespace bit

#include "cached_block_allocator.inl"

#endif /* BIT_MEMORY_BLOCK_ALLOCATORS_CACHED_BLOCK_ALLOCATOR_HPP */
