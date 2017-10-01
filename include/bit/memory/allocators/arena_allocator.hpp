/**
 * \file arena_allocator.hpp
 *
 * \brief This header contains the definition of a policy-based arena allocator
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */
#ifndef BIT_MEMORY_ALLOCATORS_ARENA_ALLOCATOR_HPP
#define BIT_MEMORY_ALLOCATORS_ARENA_ALLOCATOR_HPP

#include "../detail/ebo_storage.hpp"               // detail::ebo_storage
#include "../detail/allocator_function_traits.hpp" // detail::allocator_has_xyz

#include "../memory.hpp"         // byte
#include "../debugging.hpp"      // debug_tag_...
#include "../errors.hpp"         // get_leak_handler, get_out_of_memory_handler, etc
#include "../allocator_info.hpp" // allocator_info
#include "../macros.hpp"         // BIT_MEMORY_UNLIKELY

#include "allocator_traits.hpp"  // allocator_traits

#include <cstddef> // std::size_t, std::ptrdiff_t
#include <mutex>   // std::lock_guard

namespace bit {
  namespace memory {

    ///////////////////////////////////////////////////////////////////////////
    /// \brief This allocator manages arena-based memory allocation strategies
    ///        using policy-based-design.
    ///
    /// It is comprised of debugger types that better help memory allocation
    /// strategies, while also supporting
    ///
    /// \note Since this is an arena allocator, it is not possible for all of
    ///       the underlying policies to be stateless (since something must
    ///       keep track of the used blocks); thus this class does not default
    ///       itself to being stateless
    ///
    /// \satisfies Allocator
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
    class arena_allocator
    {
      //-----------------------------------------------------------------------
      // Public Member Types
      //-----------------------------------------------------------------------
    public:

      // Inherit the various traits specified by the allocation policy;
      // otherwise default back to the same defaults that the allocator_traits
      // will choose.

      using is_always_equal            = detail::allocator_is_always_equal<ExtendedAllocator>;
      using default_alignment          = detail::allocator_default_alignment<ExtendedAllocator>;
      using max_alignment              = detail::allocator_max_alignment<ExtendedAllocator>;
      using can_truncate_deallocations = detail::allocator_can_truncate_deallocations<ExtendedAllocator>;

      using lock_type    = BasicLockable;
      using tracker_type = MemoryTracker;

      //-----------------------------------------------------------------------
      // Constructor / Destructor
      //-----------------------------------------------------------------------
    public:

      /// \brief Constructs an arena_allocator by forwarding all arguments to
      ///        the underlying ExtendedAllocator
      ///
      /// \param args the arguments to forward to the ExtendedAllocator
      template<typename...Args, typename = std::enable_if_t<std::is_constructible<ExtendedAllocator,Args...>::value>>
      arena_allocator( Args&&...args );

      /// \brief Move-constructs this arena_allocator from another allocator
      ///
      /// \param other the other allocator to move
      arena_allocator( arena_allocator&& other ) = default;

      /// \brief Move-assigns this arena_allocator from another allocator
      ///
      /// \param other the other allocator to move
      /// \return reference to \c (*this)
      arena_allocator& operator=( arena_allocator&& other ) = default;

      /// \brief Destructs the arena allocator, checking for any leaks
      ~arena_allocator();

      //-----------------------------------------------------------------------
      // Observers
      //-----------------------------------------------------------------------
    public:

      /// \brief Accesses the tracker from the arena_allocator
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
      template<typename U = ExtendedAllocator, typename = std::enable_if_t<detail::allocator_has_deallocate_all<U>::value>>
      void deallocate_all();

      //-----------------------------------------------------------------------
      // Observers
      //-----------------------------------------------------------------------
    public:

      template<typename U = ExtendedAllocator, typename = std::enable_if_t<detail::allocator_has_owns<U>::value>>
      bool owns( const void* p ) const noexcept;

      template<typename U = ExtendedAllocator, typename = std::enable_if_t<detail::allocator_has_info<U>::value>>
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
      template<typename U = ExtendedAllocator, typename = std::enable_if_t<detail::allocator_has_max_size<U>::value>>
      std::size_t max_size() const noexcept;

      /// \brief Gets the minimum size allocateable from this allocator
      ///
      /// \note The default is 1, if this function is not defined
      ///
      /// \param alloc the allocator to get the min size from
      /// \return the minimum amount of bytes able to allocated
      template<typename U = ExtendedAllocator, typename = std::enable_if_t<detail::allocator_has_min_size<U>::value>>
      std::size_t min_size() const noexcept;

      //-----------------------------------------------------------------------
      // Private Member Types
      //-----------------------------------------------------------------------
    private:

      using storage_type = detail::ebo_storage<ExtendedAllocator,
                                               MemoryTagger,
                                               MemoryTracker,
                                               BoundsChecker,
                                               BasicLockable>;

      //-----------------------------------------------------------------------
      // Private Members
      //-----------------------------------------------------------------------
    private:

      storage_type m_storage;
    };

  } // namespace memory
} // namespace bit

#include "detail/arena_allocator.inl"

#endif /* BIT_MEMORY_ALLOCATORS_ARENA_ALLOCATOR_HPP */
