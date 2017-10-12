/**
 * \file allocator_traits.hpp
 *
 * \brief This header contains the trait type \c allocator_traits that
 *        determines or infers default values for the Allocator concept
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */
#ifndef BIT_MEMORY_ALLOCATORS_ALLOCATOR_TRAITS_HPP
#define BIT_MEMORY_ALLOCATORS_ALLOCATOR_TRAITS_HPP

#include "concepts/Stateless.hpp"         // is_stateless
#include "concepts/Allocator.hpp"         // is_allocator, allocator_has_allocate,
#include "concepts/ExtendedAllocator.hpp" // allocator_has_extended_try_allocate

#include "allocator_reference.hpp" // allocator_reference
#include "allocator_info.hpp"      // allocator_info
#include "errors.hpp"              // get_out_of_memory_handler
#include "macros.hpp"              // BIT_MEMORY_UNUSED
#include "owner.hpp"               // owner

#include <type_traits> // std::true_type, std::false_type, etc
#include <cstddef>     // std::size_t, std::ptrdiff_t
#include <limits>      // std::numeric_limits
#include <memory>      // std::addressof

namespace bit {
  namespace memory {

    //////////////////////////////////////////////////////////////////////////
    /// \brief The allocator_traits class template provides a standardized
    ///        way to access allocator functionality
    ///
    /// This will infer defaults for any optional values that may not be
    /// present in the allocator
    ///
    /// \tparam Allocator the allocator type. Must satisfy Allocator concept
    //////////////////////////////////////////////////////////////////////////
    template<typename Allocator>
    class allocator_traits
    {

      static_assert( is_allocator<Allocator>::value, "Allocator must be an Allocator" );

      //----------------------------------------------------------------------
      // Public Members
      //----------------------------------------------------------------------
    public:

      using size_type       = allocator_size_type_t<Allocator>;
      using difference_type = allocator_difference_type_t<Allocator>;
      using pointer         = allocator_pointer_t<Allocator>;
      using const_pointer   = allocator_const_pointer_t<Allocator>;

      // qualified 'bit::memory::' to avoid '-fpermissive' errors with gcc
      using is_stateless               = bit::memory::is_stateless<Allocator>;
      using default_alignment          = allocator_default_alignment<Allocator>;
      using max_alignment              = allocator_max_alignment<Allocator>;
      using can_truncate_deallocations = allocator_can_truncate_deallocations<Allocator>;
      using knows_ownership            = allocator_knows_ownership<Allocator>;

      //----------------------------------------------------------------------
      // Allocation
      //----------------------------------------------------------------------
    public:

      /// \brief Attempts to allocate memory of at least \p size bytes,
      ///        aligned to \p align boundary
      ///
      /// On failure, this function returns \p nullptr
      ///
      /// \param alloc the allocator to allocate from
      /// \param size the size of the allocation
      /// \param align the alignment of the allocation
      /// \return the pointer to the allocated memory
      static pointer try_allocate( Allocator& alloc,
                                   size_type size,
                                   size_type align ) noexcept;

      /// \brief Attempts to allocate memory of at least \p size bytes,
      ///        aligned to \p align boundary with an offset of \p offset bytes
      ///
      /// On failure, this function returns \p nullptr
      ///
      /// \param alloc the allocator to allocate from
      /// \param size the size of the allocation
      /// \param align the alignment of the allocation
      /// \param offset the offset of the allocation
      /// \return the pointer to the allocated memory
      template<typename U = Allocator, typename = std::enable_if<is_extended_allocator<U>::value>>
      static pointer try_allocate( Allocator& alloc,
                                   size_type size,
                                   size_type align,
                                   size_type offset ) noexcept;

      /// \brief Allocates memory of at least \p size bytes, aligned to \p
      ///        align boundary
      ///
      /// On failure, this function may throw or invoke the out_of_memory
      /// handler before returning \p nullptr
      ///
      /// \param alloc the allocator to allocate from
      /// \param size the size of the allocation
      /// \param align the alignment of the allocation
      /// \return the pointer to the allocated member
      static pointer allocate( Allocator& alloc,
                               size_type size,
                               size_type align );

      /// \brief Allocates memory of at least \p size bytes, aligned to \p
      ///        align boundary
      ///
      /// On failure, this function may throw or invoke the out_of_memory
      /// handler before returning \p nullptr
      ///
      /// \param alloc the allocator to allocate from
      /// \param size the size of the allocation
      /// \param align the alignment of the allocation
      /// \param offset the offset of the allocation
      /// \return the pointer to the allocated member
      template<typename U = Allocator, typename = std::enable_if<allocator_has_extended_allocate<U>::value>>
      static pointer allocate( Allocator& alloc,
                               size_type size,
                               size_type align,
                               size_type offset );

      //----------------------------------------------------------------------
      // Deallocation
      //----------------------------------------------------------------------

      /// \brief Deallocates a pointer previously allocated with \p allocate
      ///        or try_allocate
      ///
      /// \param alloc the allocator to allocate from
      /// \param p the pointer to deallocate
      /// \param size the size of the allocation
      static void deallocate( Allocator& alloc, pointer p, size_type size );

      /// \brief Deallocates all memory from the given allocator
      ///
      /// This is only enabled if the underlying allocator supports it
      ///
      /// \param alloc the allocator to deallocate everything from
      template<typename U = Allocator, typename = std::enable_if<can_truncate_deallocations::value>>
      static void deallocate_all( Allocator& alloc );

      //----------------------------------------------------------------------
      // Observers
      //----------------------------------------------------------------------

      /// \brief Checks if the given allocator is known to own the specified
      ///        pointer \p p
      ///
      /// \note This directly invokes \c alloc.owns(p) . It is undefined
      ///       behaviour to invoke this if \c Allocator::owns is not
      ///       defined; branches should instead be taken by using
      ///       \ref knows_ownership
      ///
      /// \param alloc the allocator
      /// \param p the pointer
      static bool owns( const Allocator& alloc, const_pointer p ) noexcept;

      /// \brief Gets the name of the specified allocator
      ///
      /// \note Not all allocators are nameable or have a name specified.
      ///       For these allocators, the string returned is "Unnamed"
      ///
      /// \note The lifetime of the pointer returned is unmanaged, and is NOT
      ///       the responsibility of the caller to free.
      ///
      /// \param alloc the allocator to get the name of
      /// \return the name of the allocator
      static allocator_info info( const Allocator& alloc ) noexcept;

      //----------------------------------------------------------------------
      // Capacity
      //----------------------------------------------------------------------

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
      static size_type max_size( const Allocator& alloc ) noexcept;

      /// \brief Gets the minimum size allocateable from this allocator
      ///
      /// \note The default is 1, if this function is not defined
      ///
      /// \param alloc the allocator to get the min size from
      /// \return the minimum amount of bytes able to allocated
      static size_type min_size( const Allocator& alloc ) noexcept;

      //----------------------------------------------------------------------
      // Private Implementation
      //----------------------------------------------------------------------
    private:

      /// \{
      /// \brief Performs the allocation, either by calling
      ///        \c Allocator::allocate, or by calling into
      ///        \c Allocator::try_allocate and defining a boilerplate
      ///        allocate function
      ///
      /// \param alloc the allocator to allocate
      /// \param size the size of the allocation
      /// \param align the alignment of the allocation
      /// \param offset the offset of the alignment
      /// \return the allocated pointer
      static pointer do_allocate( std::true_type, Allocator& alloc, size_type size, size_type align );
      static pointer do_allocate( std::false_type, Allocator& alloc, size_type size, size_type align );
      static pointer do_allocate( std::true_type, Allocator& alloc, size_type size, size_type align, size_type offset );
      static pointer do_allocate( std::false_type, Allocator& alloc, size_type size, size_type align, size_type offset );
      /// \}

      //----------------------------------------------------------------------

      /// \{
      /// \brief Determines the max size of the allocator, either by
      ///        calling \c Allocator::max_size, or by assuming
      ///        \c std::numeric_limits<size_type>::max()
      ///
      /// \param alloc the allocator
      /// \return the max size
      static size_type do_max_size( std::true_type, const Allocator& alloc );
      static size_type do_max_size( std::false_type, const Allocator& alloc );
      /// \}

      //----------------------------------------------------------------------

      /// \{
      /// \brief Determines the min size of an allocation, either by
      ///        calling \c Allocator::min_size(), or by assuming
      ///        \c 1
      ///
      /// \param alloc the allocator
      /// \return the max size
      static size_type do_min_size( std::true_type, const Allocator& alloc );
      static size_type do_min_size( std::false_type, const Allocator& alloc );
      /// \}

      //----------------------------------------------------------------------

      /// \{
      /// \brief Determines the info for the allocator, either by calling
      ///        \c Allocator::info or by assuming "Unnamed" for the name
      ///
      /// \param alloc the allocator
      /// \return the name of the allocator
      static allocator_info do_info( std::true_type, const Allocator& alloc );
      static allocator_info do_info( std::false_type, const Allocator& alloc );
      /// \}

    };

    template<typename Allocator>
    class allocator_traits<allocator_reference<Allocator>>
      : public allocator_traits<Allocator>
    {

    };

  } // namespace memory
} // namespace bit

#include "detail/allocator_traits.inl"

#endif /* BIT_MEMORY_ALLOCATORS_ALLOCATOR_TRAITS_HPP */
