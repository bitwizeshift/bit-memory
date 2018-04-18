/*****************************************************************************
 * \file
 * \brief This header contains a block allocator that uses policy-based design
 *        to componentize entries
 *****************************************************************************/

/*
  The MIT License (MIT)

  Copyright (c) 2018 Matthew Rodusek

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/
#ifndef BIT_MEMORY_BLOCK_ALLOCATORS_POLICY_BLOCK_ALLOCATOR_HPP
#define BIT_MEMORY_BLOCK_ALLOCATORS_POLICY_BLOCK_ALLOCATOR_HPP

#include "../utilities/ebo_storage.hpp"      // detail::ebo_storage
#include "../utilities/allocator_info.hpp"         // allocator_info
#include "../utilities/memory_block.hpp"           // memory_block
#include "../utilities/owner.hpp"                  // owner

#include "../concepts/BlockAllocator.hpp" // block_allocator_has

#include "../traits/block_allocator_traits.hpp" // block_allocator_traits

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
               bool HasAlignment = block_allocator_has_default_block_alignment<BlockAllocator>::value>
      struct policy_block_allocator_base
      {

      };

      template<typename BlockAllocator>
      struct policy_block_allocator_base<BlockAllocator,true>
      {
        using default_block_alignment = typename BlockAllocator::default_block_alignment;
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
    /// \note Even if all policies satisfy the Stateless concept, this allocator
    ///       will always be considered stateful since it delegates actions in
    ///       its destructor -- which may call a function that touches external
    ///       state (which is thus no longer stateless).
    ///
    /// \satisfies{BlockAllocator}
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
      : private ebo_storage<BlockAllocator,MemoryTagger,MemoryTracker,BasicLockable>,
        public detail::policy_block_allocator_base<BlockAllocator>
    {
      using traits_type = block_allocator_traits<BlockAllocator>;
      using base_type = ebo_storage<BlockAllocator,
                                            MemoryTagger,
                                            MemoryTracker,
                                            BasicLockable>;

      //-----------------------------------------------------------------------
      // Public Member Types
      //-----------------------------------------------------------------------
    public:

      using lock_type    = BasicLockable;
      using tracker_type = MemoryTracker;

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
      template<typename Arg0, typename...Args,
               typename = std::enable_if_t<
                 std::is_constructible<BlockAllocator,Arg0,Args...>::value &&
                 !std::is_same<Arg0,std::decay_t<policy_block_allocator>>::value>>
      policy_block_allocator( Arg0&& arg0, Args&&...args );

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
      allocator_info info() const noexcept;

      /// \brief Queries the next block size expected from this allocator
      ///
      /// \return the size of the next allocated block
      std::size_t next_block_size() const noexcept;
    };

  } // namespace memory
} // namespace memory

#include "detail/policy_block_allocator.inl"

#endif /* BIT_MEMORY_BLOCK_ALLOCATORS_POLICY_BLOCK_ALLOCATOR_HPP */
