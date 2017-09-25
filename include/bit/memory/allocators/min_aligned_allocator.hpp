/**
 * \file min_aligned_allocator.hpp
 *
 * \brief This header contains the definition for an allocator wrapper that
 *        guarantees the underlying allocator always allocates entries that
 *        are suitably aligned for a specific boundary
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */
#ifndef BIT_MEMORY_ALLOCATORS_MIN_ALIGNED_ALLOCATOR_HPP
#define BIT_MEMORY_ALLOCATORS_MIN_ALIGNED_ALLOCATOR_HPP

#include "../memory.hpp"             // owner
#include "allocator_traits.hpp"      // allocator_traits
#include "../detail/ebo_storage.hpp" // ebo_storage

#include <cstddef> // std::size_t
#include <utility> // std::move
#include <tuple>   // std::forward_as_tuple

namespace bit {
  namespace memory {

  ///////////////////////////////////////////////////////////////////////////
  /// \brief This allocator wrapper ensures that the underlying allocator is
  ///        always suitably aligned to a minimum alignment boundary
  ///
  /// \satisfies Allocator
  /// \tparam Allocator the allocator to wrap
  /// \tparam MinAlign the minimum alignment to expect
  ///////////////////////////////////////////////////////////////////////////
  template<typename Allocator, std::size_t MinAlign>
  class min_aligned_allocator
    : private detail::ebo_storage<Allocator>
  {
    using traits_type = allocator_traits<Allocator>;
    using base_type = detail::ebo_storage<Allocator>;

    static_assert( MinAlign <= traits_type::max_alignment, "Minimum alignment cannot exceed the allocator's max alignment" );

    //-----------------------------------------------------------------------
    // Public Static Members
    //-----------------------------------------------------------------------
  public:

    using is_always_equal   = typename traits_type::is_always_equal;
    using is_stateless      = typename traits_type::is_stateless;
    using default_alignment = std::integral_constant<std::size_t,MinAlign>;
    using max_alignment     = typename traits_type::max_alignment;

    //-----------------------------------------------------------------------
    // Constructor / Assignment
    //-----------------------------------------------------------------------
  public:

    /// \brief Default-constructs a min_aligned_allocator
    min_aligned_allocator();

    /// \brief Default-constructs a aligned_allocator
    template<typename Arg0, typename...Args, typename = std::enable_if_t<!std::is_same<std::decay_t<Arg0>,min_aligned_allocator>::value>>
    min_aligned_allocator( Arg0&& arg0, Args&&...args );

    /// \brief Move-constructs a min_aligned_allocator from another allocator
    ///
    /// \param other the other min_aligned_allocator to move
    min_aligned_allocator( min_aligned_allocator&& other ) noexcept = default;

    // Deleted copy constructor
    min_aligned_allocator( const min_aligned_allocator& other ) = delete;

    //-----------------------------------------------------------------------

    /// \brief Move-assigns a min_aligned_allocator from another allocator
    ///
    /// \param other the other allocator to move assign
    /// \return reference to \c (*this)
    min_aligned_allocator& operator=( min_aligned_allocator&& other ) noexcept = default;

    // Deleted copy assignment
    min_aligned_allocator& operator=( const min_aligned_allocator& other ) = delete;

    //-----------------------------------------------------------------------
    // Allocations / Deallocation
    //-----------------------------------------------------------------------
  public:

    /// \brief Allocates aligned memory of size \p size, with alignment to a
    ///        boundary of at least \p align
    ///
    /// \param size the size of this allocation
    /// \param align the requested alignment
    /// \return the allocated pointer
    owner<void*> allocate( std::size_t size, std::size_t align );

    /// \brief Allocates aligned memory of size \p size, with alignment to a
    ///        boundary of at least \p align
    ///
    /// \param size the size of this allocation
    /// \param align the requested alignment
    /// \return the allocated pointer, or nullptr on failure
    void* try_allocate( std::size_t size, std::size_t align ) noexcept;

    /// \brief Deallocates a pointer \p p with the allocation size of \p size
    ///
    /// \param p the pointer to deallocate
    /// \param size the size to deallocate
    void deallocate( owner<void*> p, std::size_t size );

    //-----------------------------------------------------------------------
    // Naming
    //-----------------------------------------------------------------------
  public:

    /// \brief Gets the name of the underlying allocator
    ///
    /// \return the name of the underlying allocator
    const char* name() const noexcept;

    /// \brief Sets the name of the underlying allocator
    ///
    /// \param name the name to set to the underlying allocator
    void set_name( const char* name ) noexcept;

    //-----------------------------------------------------------------------
    // Capacity
    //-----------------------------------------------------------------------
  public:

    /// \brief Gets the maximum size the underlying allocator can allocate
    ///
    /// \return the maximum size
    std::size_t max_size() const noexcept;

    /// \brief Gets the current number of bytes the underlying allocator has
    ///        used
    ///
    /// \return the number of bytes used
    std::size_t used() const noexcept;

    //-----------------------------------------------------------------------
    // Comparators
    //-----------------------------------------------------------------------
  public:

    bool operator==( const min_aligned_allocator& rhs ) const noexcept;
    bool operator!=( const min_aligned_allocator& rhs ) const noexcept;

  };

  } // namespace memory
} // namespace bit

#include "detail/min_aligned_allocator.inl"

#endif /* BIT_MEMORY_ALLOCATORS_MIN_ALIGNED_ALLOCATOR_HPP */
