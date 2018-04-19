/*****************************************************************************
 * \file
 * \brief This header contains the definition of a policy-based allocator
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
#ifndef BIT_MEMORY_ALLOCATORS_POLICY_ALLOCATOR_HPP
#define BIT_MEMORY_ALLOCATORS_POLICY_ALLOCATOR_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "../utilities/ebo_storage.hpp" // detail::ebo_storage
#include "../utilities/allocator_info.hpp" // allocator_info
#include "../utilities/debugging.hpp"      // debug_tag_...
#include "../utilities/errors.hpp"         // get_leak_handler, etc
#include "../utilities/macros.hpp"         // BIT_MEMORY_UNLIKELY

#include "../concepts/Allocator.hpp"         // Allocator
#include "../concepts/ExtendedAllocator.hpp" // ExtendedAllocator

#include "../traits/allocator_traits.hpp" // allocator_traits

#include <cstddef> // std::size_t, std::ptrdiff_t
#include <mutex>   // std::lock_guard

namespace bit {
  namespace memory {

    ///////////////////////////////////////////////////////////////////////////
    /// \brief This allocator manages policy-based memory allocation strategies
    ///        using policy-based-design.
    ///
    /// It is comprised of debugger types that better help memory allocation
    /// strategies, while also supporting
    ///
    /// \note Since this is an policy allocator, it is not possible for all of
    ///       the underlying policies to be stateless (since something must
    ///       keep track of the used blocks); thus this class does not default
    ///       itself to being stateless
    ///
    /// \satisfies{Allocator}
    ///
    /// \tparam ExtendedAllocator An allocator that satisfies the
    ///                           ExtendedAllocator concept
    /// \tparam MemoryTagger A class used for tagging memory on allocations and
    ///                      on deallocations.
    /// \tparam MemoryTracker A class used for tracking memory allocations.
    /// \tparam BoundsChecker A class used for bounds checking; used to
    ///                       catch buffer-overflow issues
    /// \tparam BasicLockable A lockable type for thread-synchronization
    ///////////////////////////////////////////////////////////////////////////
    template<typename ExtendedAllocator,
             typename MemoryTagger,
             typename MemoryTracker,
             typename BoundsChecker,
             typename BasicLockable>
    class policy_allocator
      : private ebo_storage<ExtendedAllocator,
                                    MemoryTagger,
                                    MemoryTracker,
                                    BoundsChecker,
                                    BasicLockable>
    {

      using base_type = ebo_storage<ExtendedAllocator,
                                            MemoryTagger,
                                            MemoryTracker,
                                            BoundsChecker,
                                            BasicLockable>;

      //-----------------------------------------------------------------------
      // Public Member Types
      //-----------------------------------------------------------------------
    public:

      // Inherit the various traits specified by the allocation policy;
      // otherwise default back to the same defaults that the allocator_traits
      // will choose.

      using default_alignment = allocator_default_alignment<ExtendedAllocator>;
      using max_alignment     = allocator_max_alignment<ExtendedAllocator>;
      using lock_type         = BasicLockable;
      using tracker_type      = MemoryTracker;

      //-----------------------------------------------------------------------
      // Constructor / Destructor / Assignment
      //-----------------------------------------------------------------------
    public:

      /// \brief Constructs an policy_allocator by forwarding all arguments to
      ///        the underlying ExtendedAllocator
      ///
      /// \param args the arguments to forward to the ExtendedAllocator
      template<typename...Args, typename = std::enable_if_t<std::is_constructible<ExtendedAllocator,Args...>::value>>
      policy_allocator( Args&&...args );

      /// \brief Move-constructs this policy_allocator from another allocator
      ///
      /// \note This constructor is only enabled if ExtendedAllocator is
      ///       stateless
      ///
      /// \param other the other allocator to move
      policy_allocator( policy_allocator&& other ) = default;

      /// \brief Copy-constructs this policy_allocator from another allocator
      ///
      /// \note This constructor is only enabled if ExtendedAllocator is
      ///       stateless
      ///
      /// \param other the other allocator to copy
      policy_allocator( const policy_allocator& other ) = default;

      //-----------------------------------------------------------------------

      /// \brief Destructs the policy allocator, checking for any leaks
      ~policy_allocator();

      //-----------------------------------------------------------------------

      /// \brief Move-assigns this policy_allocator from another allocator
      ///
      /// \note This assignment is only enabled if ExtendedAllocator is
      ///       stateless
      ///
      /// \param other the other allocator to move
      policy_allocator& operator=( policy_allocator&& other ) = default;

      /// \brief Copy-assigns this policy_allocator from another allocator
      ///
      /// \note This assignment is only enabled if ExtendedAllocator is
      ///       stateless
      ///
      /// \param other the other allocator to copy
      policy_allocator& operator=( const policy_allocator& other ) = default;

      //-----------------------------------------------------------------------
      // Observers
      //-----------------------------------------------------------------------
    public:

      /// \brief Accesses the tracker from the policy_allocator
      ///
      /// \return the tracker
      const tracker_type& tracker() const noexcept;

      //-----------------------------------------------------------------------
      // Allocation / Deallocation
      //-----------------------------------------------------------------------

      /// \brief Attempts to allocate \p size bytes aligned to a boundary of
      ///        \p align using the underlying allocator
      ///
      /// \return an allocated pointer on success, \c nullptr on failure
      void* try_allocate( std::size_t size, std::size_t align ) noexcept;

      /// \brief Deallocates the pointer \p p with the size \p size
      ///
      /// \param p the pointer to deallocate
      /// \param size the size originally requested to 'try_allocate'
      void deallocate( void* p, std::size_t size );

      //-----------------------------------------------------------------------

      /// \brief Deallocates all memory in this allocator
      ///
      /// \note This function is only enabled if the underlying
      ///       \c ExtendedAllocator supports it
      template<typename U = ExtendedAllocator, typename = std::enable_if_t<allocator_can_truncate_deallocations<U>::value>>
      void deallocate_all();

      //-----------------------------------------------------------------------
      // Observers
      //-----------------------------------------------------------------------
    public:

      /// \brief Checks if \p p is owned by the underlying allocator
      ///
      /// \note This function is only enabled if the underlying
      ///       \c ExtendedAllocator supports it
      ///
      /// \param p the pointer to check
      /// \return \c true if \p p is owned by this allocator
      template<typename U = ExtendedAllocator, typename = std::enable_if_t<allocator_knows_ownership<U>::value>>
      bool owns( const void* p ) const noexcept;

      /// \brief Retrieves info about this allocator
      ///
      ///
      /// \note This function is only enabled if the underlying
      ///       \c ExtendedAllocator supports it
      ///
      /// \return the info about this allocator
      template<typename U = ExtendedAllocator, typename = std::enable_if_t<allocator_has_info<U>::value>>
      allocator_info info() const noexcept;

      //----------------------------------------------------------------------
      // Capacity
      //----------------------------------------------------------------------
    public:

      /// \brief Gets the maximum size allocateable from this allocator
      ///
      /// \note the max_size does not guarantee that the size is able to be
      ///       allocated at the time the value is queried; it just means that
      ///       it is the maximum size that the allocator is capable of
      ///       allocating from an empty state.
      ///
      /// \param alloc the allocator to get the max size from
      /// \return the amount of bytes available for the largest possible
      ///         allocation
      template<typename U = ExtendedAllocator, typename = std::enable_if_t<allocator_has_max_size<U>::value>>
      std::size_t max_size() const noexcept;

      /// \brief Gets the minimum size allocateable from this allocator
      ///
      /// \note The default is 1, if this function is not defined
      ///
      /// \param alloc the allocator to get the min size from
      /// \return the minimum amount of bytes able to allocated
      template<typename U = ExtendedAllocator, typename = std::enable_if_t<allocator_has_min_size<U>::value>>
      std::size_t min_size() const noexcept;
    };

  } // namespace memory
} // namespace bit

#include "detail/policy_allocator.inl"

#endif /* BIT_MEMORY_ALLOCATORS_POLICY_ALLOCATOR_HPP */
