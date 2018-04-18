/*****************************************************************************
 * \file
 * \brief This header defines an allocator that falls-back on failure to other
 *        allocators
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
#ifndef BIT_MEMORY_ALLOCATORS_FALLBACK_ALLOCATOR_HPP
#define BIT_MEMORY_ALLOCATORS_FALLBACK_ALLOCATOR_HPP

#include "detail/named_allocator.hpp" // detail::named_allocator

#include "../utilities/ebo_storage.hpp" // detail::ebo_storage
#include "../utilities/macros.hpp"             // BIT_MEMORY_UNUSED
#include "../utilities/owner.hpp"              // owner

#include "../concepts/Allocator.hpp" // allocator_pointer_t, etc

#include "../traits/allocator_traits.hpp" // allocator_traits

#include <tuple>     // std::forward_as_tuple
#include <utility>   // std::forward
#include <cstddef>   // std::size_t
#include <algorithm> // std::max, std::min

namespace bit {
  namespace memory {

    ///////////////////////////////////////////////////////////////////////////
    /// \brief This allocator acts as an aggregate that can fall back to other
    ///        allocators on failure.
    ///
    /// This will attempt to allocate memory from the first allocator in the
    /// sequence, and -- on failure -- will fall-back to the next one in
    /// sequence.
    ///
    /// For deallocations, all allocators except the last one must implement
    /// the 'owns' member function to identify ownership of previously
    /// allocated memory. The last allocator in the sequence is a special-case
    /// that doesn't require ownership, allowing for raw allocators to be used
    /// as the final fallback in the allocation sequence.
    ///
    /// \satisfies{Allocator}
    /// \tparam AllocatorStorages the allocator storages to sequence through
    ///////////////////////////////////////////////////////////////////////////
    template<typename...AllocatorStorages>
    class fallback_allocator
      : private ebo_storage<AllocatorStorages...>
    {

      using base_type = ebo_storage<AllocatorStorages...>;

      // TODO(bitwizeshift): Support pretty-pointers by determining the common
      //                     'pointer', 'const_pointer', and 'size_type' of
      //                     each allocator

      //-----------------------------------------------------------------------
      // Constructors
      //-----------------------------------------------------------------------
    public:

      /// \brief Constructs each allocator in the fallback_allocator in-place,
      ///        constructing each allocator from a single argument
      ///
      /// \param storages the allocator storage types to use
      explicit fallback_allocator( AllocatorStorages...storages ) noexcept;

      /// \brief Move-constructs the fallback_allocator from an existing one
      ///
      /// \note This constructor is only enabled if all allocators are move-
      ///       constructible
      ///
      /// \param other the other fallback_allocator to move
      fallback_allocator( fallback_allocator&& other ) = default;

      /// \brief Copy-constructs the fallback_allocator from an existing one
      ///
      /// \note This constructor is only enabled if all allocators are copy-
      ///       constructible
      ///
      /// \param other the other fallback_allocator to copy
      fallback_allocator( const fallback_allocator& other ) = default;

      //-----------------------------------------------------------------------

      /// \brief Move-assigns the fallback_allocator from an existing one
      ///
      /// \note This operator is only enabled if all allocators are copy-
      ///       assignable
      ///
      /// \param other the other fallback_allocator to move
      fallback_allocator& operator=( fallback_allocator&& other ) = default;

      /// \brief Copy-assigns the fallback_allocator from an existing one
      ///
      /// \note This operator is only enabled if all allocators are copy-
      ///       assignable
      ///
      /// \param other the other fallback_allocator to copy
      fallback_allocator& operator=( const fallback_allocator& other ) = default;

      //-----------------------------------------------------------------------
      // Allocations
      //-----------------------------------------------------------------------
    public:

      /// \brief Attempts to allocate memory, starting with the first allocator
      ///        in sequence, and falling back to further allocators on failure
      ///
      /// \param size the size of the allocation request
      /// \param align the alignment of the allocation request
      /// \return a pointer on success, \c nullptr on failure
      owner<void*> try_allocate( std::size_t size,
                                 std::size_t align ) noexcept;

      /// \brief Deallocates the pointer \p p of size \p size from the
      ///        underlying allocator
      ///
      /// \bug If two allocators inside of this fallback allocator both allocate
      ///      within the same memory region (e.g. inside the same memory block),
      ///      then the origin of \p p may be incorrectly determined and
      ///      deallocated incorrectly.
      ///
      /// \param p the pointer to memory to deallocate
      /// \param size the size of the memory to deallocate
      void deallocate( owner<void*> p, std::size_t size );

      //-----------------------------------------------------------------------
      // Observers
      //-----------------------------------------------------------------------
    public:

      /// \brief Checks if any of the allocators in the fallback allocator owns
      ///        the memory pointer to by \p p
      ///
      /// \param p the pointer to check
      /// \return \c true if the memory is owned by an allocator in this allocator
      bool owns( const void* p ) const noexcept;

      //-----------------------------------------------------------------------
      // Capacity
      //-----------------------------------------------------------------------
    public:

      /// \brief Determines the maximum size of the allocations that this
      ///        fallback_allocator supports
      ///
      /// \return the max size
      std::size_t max_size() const noexcept;

      /// \brief Determines the minimum size of the allocations that this
      ///        fallback_allocator supports
      ///
      /// \return the min size
      std::size_t min_size() const noexcept;

      //-----------------------------------------------------------------------
      // Private Allocation / Deallocation
      //-----------------------------------------------------------------------
    private:

      template<std::size_t Idx>
      void* do_try_allocate( std::integral_constant<std::size_t,Idx>,
                             std::size_t size,
                             std::size_t align ) noexcept;

      void* do_try_allocate( std::integral_constant<std::size_t,sizeof...(AllocatorStorages)-1>,
                             std::size_t size,
                             std::size_t align ) noexcept;

      //-----------------------------------------------------------------------

      template<std::size_t Idx>
      void do_deallocate( std::integral_constant<std::size_t,Idx>,
                          void* p,
                          std::size_t size );

      void do_deallocate( std::integral_constant<std::size_t,sizeof...(AllocatorStorages)-1>,
                          void* p,
                          std::size_t size );

      //-----------------------------------------------------------------------
      // Private Observers
      //-----------------------------------------------------------------------
    private:

      template<std::size_t...Idxs>
      bool do_owns( std::index_sequence<Idxs...>,
                    const void* p ) const noexcept;

      //-----------------------------------------------------------------------
      // Private Capacity
      //-----------------------------------------------------------------------
    private:

      template<std::size_t...Idxs>
      std::size_t do_max_size( std::index_sequence<Idxs...> ) const noexcept;

      template<std::size_t...Idxs>
      std::size_t do_min_size( std::index_sequence<Idxs...> ) const noexcept;

    };

    //-------------------------------------------------------------------------
    // Utilities
    //-------------------------------------------------------------------------

    template<typename...Allocators>
    using named_fallback_allocator = detail::named_allocator<fallback_allocator<Allocators...>>;

    //-------------------------------------------------------------------------

    /// \brief Makes a fallback allocator from the given allocator storages
    ///
    /// \note The order of the storages indicates the order of fallback
    ///
    /// \param storages the storages for the allocators
    /// \return the fallback allocator made with the allocators
    template<typename...AllocatorStorages>
    fallback_allocator<AllocatorStorages...>
      make_fallback_allocator( AllocatorStorages...storages );

    /// \brief Makes a named fallback allocator from the given allocator
    ///        storages
    ///
    /// \note The order of the storages indicates the order of fallback
    ///
    /// \param storages the storages for the allocators
    /// \return the fallback allocator made with the allocators
    template<typename...AllocatorStorages>
    named_fallback_allocator<AllocatorStorages...>
      make_named_fallback_allocator( const char* name,
                                     AllocatorStorages...storages );

  } // namespace memory
} // namespace bit

#include "detail/fallback_allocator.inl"

#endif /* BIT_MEMORY_ALLOCATORS_FALLBACK_ALLOCATOR_HPP */
