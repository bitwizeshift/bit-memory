/**
 * \file policy_block_allocator.hpp
 *
 * \brief This header contains a block allocator that uses policy-based design
 *        to componentize entries
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */
#ifndef BIT_MEMORY_BLOCK_ALLOCATORS_POLICY_BLOCK_ALLOCATOR_HPP
#define BIT_MEMORY_BLOCK_ALLOCATORS_POLICY_BLOCK_ALLOCATOR_HPP

#include "../detail/ebo_storage.hpp"      // detail::ebo_storage
#include "../concepts/BlockAllocator.hpp" // block_allocator_has

#include "../owner.hpp"                  // owner
#include "../memory_block.hpp"           // memory_block
#include "../allocator_info.hpp"         // allocator_info
#include "../block_allocator_traits.hpp" // block_allocator_traits

#include <cstddef> // std::size_t
#include <tuple>   // std::tuple, std::forward_as_tuple
#include <mutex>   // std::lock_guard

namespace bit {
  namespace memory {
    namespace detail {

      /////////////////////////////////////////////////////////////////////////
      /// \brief Internal type to conditionally inherit 'block_alignment' and
      ///        'block_size' from BlockAllocator, if it exists in BlockAllocator
      ///
      /////////////////////////////////////////////////////////////////////////
      template<typename BlockAllocator,
               bool HasAlignment = block_allocator_has_block_alignment<BlockAllocator>::value,
               bool HasSize = block_allocator_has_block_size<BlockAllocator>::value>
      struct policy_block_allocator_base
      {

      };

      template<typename BlockAllocator, bool HasSize>
      struct policy_block_allocator_base<BlockAllocator,true,HasSize>
      {
        using block_alignment = typename BlockAllocator::block_alignment;
      };

      template<typename BlockAllocator, bool HasAlignment>
      struct policy_block_allocator_base<BlockAllocator,HasAlignment,true>
      {
        using block_size = typename BlockAllocator::block_size;
      };

      template<typename BlockAllocator>
      struct policy_block_allocator_base<BlockAllocator,true,true>
      {
        using block_alignment = typename BlockAllocator::block_alignment;
        using block_size = typename BlockAllocator::block_size;
      };

    } // namespae detail


    ///////////////////////////////////////////////////////////////////////////
    /// \brief This block allocator uses policies to modify the behaviour of
    ///        the underlying type
    ///
    /// This block allocator acts and behaves identically to the underlying
    /// BlockAllocator by proxying the same functions defined in the
    /// BlockAllocator concept.
    ///
    /// \satisfies BlockAllocator
    ///
    /// \tparam BlockAllocator The block allocator to use
    /// \tparam MemoryTagger A class used for tagging memory on allocations and
    ///                      on deallocations.
    /// \tparam MemoryTracker A class used for tracking memory allocations.
    /// \tparam BoundsChecker A class used for bounds checking; used to
    ///                       catch buffer-overflow issues
    /// \tparam BasicLockable A lockable type for thread-synchronization
    ///////////////////////////////////////////////////////////////////////////
    template<typename BlockAllocator,
             typename MemoryTagger,
             typename MemoryTracker,
             typename BasicLockable>
    class policy_block_allocator
      : private detail::ebo_storage<BlockAllocator,MemoryTagger,MemoryTracker,BasicLockable>,
        public detail::policy_block_allocator_base<BlockAllocator>
    {
      using traits_type = block_allocator_traits<BlockAllocator>;
      using base_type = detail::ebo_storage<BlockAllocator,MemoryTagger,MemoryTracker,BasicLockable>;

      //-----------------------------------------------------------------------
      // Public Member Types
      //-----------------------------------------------------------------------
    public:

      using lock_type    = BasicLockable;
      using tracker_type = MemoryTracker;
      using is_stateless = std::integral_constant<bool,is_stateless<BlockAllocator>::value &&
                                                       std::is_empty<MemoryTagger>::value &&
                                                       std::is_empty<MemoryTracker>::value &&
                                                       std::is_empty<BasicLockable>::value>;

      //-----------------------------------------------------------------------
      // Constructors
      //-----------------------------------------------------------------------
    public:

      /// \brief Default-constructs the policy_block_allocator
      policy_block_allocator() = default;

      /// \brief Constructs the underlying block allocator by forwarding the
      ///        arguments to it
      ///
      /// \param args the arguments to forward to the underlying block allocator
      template<typename...Args, typename = std::enable_if_t<std::is_constructible<BlockAllocator,Args...>::value>>
      policy_block_allocator( Args&&...args );

      /// \brief Move-constructs this policy_block_allocator from an existing
      ///        one
      ///
      /// \note this is only enabled if the underlying allocator supports it
      ///
      /// \param other the other allocator to move
      policy_block_allocator( policy_block_allocator&& other ) = default;

      /// \brief Copy-constructs this policy_block_allocator from an existing
      ///        one
      ///
      /// \note this is only enabled if the underlying allocator supports it
      ///
      /// \param other the other allocator to copy
      policy_block_allocator( const policy_block_allocator& other ) = default;

      //-----------------------------------------------------------------------

      /// \brief Destructs the policy_block_allocator, finalizing any leak
      ///        tracking
      ~policy_block_allocator();

      //-----------------------------------------------------------------------

      /// \brief Move-assigns this policy_block_allocator from an existing
      ///        one
      ///
      /// \note this is only enabled if the underlying allocator supports it
      ///
      /// \param other the other allocator to move
      /// \return reference to \c (*this)
      policy_block_allocator& operator=( policy_block_allocator&& other ) = default;

      /// \brief Copy-assigns this policy_block_allocator from an existing
      ///        one
      ///
      /// \note this is only enabled if the underlying allocator supports it
      ///
      /// \param other the other allocator to copy
      /// \return reference to \c (*this)
      policy_block_allocator& operator=( const policy_block_allocator& other ) = default;

      //-----------------------------------------------------------------------
      // Block Allocation
      //-----------------------------------------------------------------------
    public:

      /// \brief Allocates a memory block using the underliny allocator
      ///
      /// \return the allocated block
      owner<memory_block> allocate_block();

      /// \brief Deallocates the specified memory block
      ///
      /// \param block the block to deallocate
      void deallocate_block( owner<memory_block> block );

      //-----------------------------------------------------------------------
      // Observers
      //-----------------------------------------------------------------------
    public:

      /// \brief Accesses the tracker from the arena_allocator
      ///
      /// \return the tracker
      const tracker_type& tracker() const noexcept;

      /// \brief Gets the info about this current block allocator, or null
      ///        otherwise
      ///
      /// \note This is only enabled if the underlying BlockAllocator supports
      ///       \c info()
      ///
      /// \return the allocator info
      template<typename U = BlockAllocator, typename = std::enable_if_t<block_allocator_has_info<U>::value>>
      allocator_info info() const noexcept;

      //-----------------------------------------------------------------------
      // Private Members
      //-----------------------------------------------------------------------
    private:

      std::size_t block_align( std::true_type ) const noexcept;
      std::size_t block_align( std::false_type ) const noexcept;

    };

  } // namespace memory
} // namespace memory

#include "detail/policy_block_allocator.inl"

#endif /* BIT_MEMORY_BLOCK_ALLOCATORS_POLICY_BLOCK_ALLOCATOR_HPP */
