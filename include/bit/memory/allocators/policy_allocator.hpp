/**
 * \file policy_allocator.hpp
 *
 * \brief This header contains the definition of a policy-based allocator
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */
#ifndef BIT_MEMORY_ALLOCATORS_POLICY_ALLOCATOR_HPP
#define BIT_MEMORY_ALLOCATORS_POLICY_ALLOCATOR_HPP

#include "../detail/ebo_storage.hpp"         // detail::ebo_storage
#include "../concepts/Allocator.hpp"         // Allocator
#include "../concepts/ExtendedAllocator.hpp" // ExtendedAllocator

#include "../debugging.hpp"      // debug_tag_...
#include "../errors.hpp"         // get_leak_handler, get_out_of_memory_handler, etc
#include "../allocator_info.hpp" // allocator_info
#include "../macros.hpp"         // BIT_MEMORY_UNLIKELY

#include "../allocator_traits.hpp"  // allocator_traits

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
      : private detail::ebo_storage<ExtendedAllocator,
                                    MemoryTagger,
                                    MemoryTracker,
                                    BoundsChecker,
                                    BasicLockable>
    {

      using base_type = detail::ebo_storage<ExtendedAllocator,
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

      using is_always_equal            = allocator_is_always_equal<ExtendedAllocator>;
      using default_alignment          = allocator_default_alignment<ExtendedAllocator>;
      using max_alignment              = allocator_max_alignment<ExtendedAllocator>;
      using can_truncate_deallocations = allocator_can_truncate_deallocations<ExtendedAllocator>;
      using is_stateless = std::integral_constant<bool,is_stateless<ExtendedAllocator>::value &&
                                                       std::is_empty<MemoryTagger>::value &&
                                                       std::is_empty<MemoryTracker>::value &&
                                                       std::is_empty<BoundsChecker>::value &&
                                                       std::is_empty<BasicLockable>::value>;

      using lock_type    = BasicLockable;
      using tracker_type = MemoryTracker;

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
      template<typename U = ExtendedAllocator, typename = std::enable_if_t<allocator_can_truncate_deallocations<U>::value>>
      void deallocate_all();

      //-----------------------------------------------------------------------
      // Observers
      //-----------------------------------------------------------------------
    public:

      template<typename U = ExtendedAllocator, typename = std::enable_if_t<allocator_knows_ownership<U>::value>>
      bool owns( const void* p ) const noexcept;

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

      //-----------------------------------------------------------------------
      // Private Members
      //-----------------------------------------------------------------------
    private:

      bool equals( const policy_allocator& other ) const noexcept;

      template<typename Allocator, typename Tagger, typename Tracker,typename Checker, typename Lock>
      friend bool operator==( const policy_allocator<Allocator,Tagger,Tracker,Checker,Lock>& lhs,
                              const policy_allocator<Allocator,Tagger,Tracker,Checker,Lock>& rhs ) noexcept;
    };

    //-------------------------------------------------------------------------
    // Comparison
    //-------------------------------------------------------------------------

    template<typename Allocator, typename Tagger, typename Tracker,typename Checker, typename Lock>
    bool operator==( const policy_allocator<Allocator,Tagger,Tracker,Checker,Lock>& lhs,
                     const policy_allocator<Allocator,Tagger,Tracker,Checker,Lock>& rhs ) noexcept;

    template<typename Allocator, typename Tagger, typename Tracker,typename Checker, typename Lock>
    bool operator!=( const policy_allocator<Allocator,Tagger,Tracker,Checker,Lock>& lhs,
                     const policy_allocator<Allocator,Tagger,Tracker,Checker,Lock>& rhs ) noexcept;

  } // namespace memory
} // namespace bit

#include "detail/policy_allocator.inl"

#endif /* BIT_MEMORY_ALLOCATORS_POLICY_ALLOCATOR_HPP */
