/**
 * \file stack_allocator.hpp
 *
 * \brief This header contains the definition of a stack allocator for
 *        temporary, automatic reclamation
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */
#ifndef BIT_MEMORY_ALLOCATORS_STACK_ALLOCATOR_HPP
#define BIT_MEMORY_ALLOCATORS_STACK_ALLOCATOR_HPP

#include "detail/named_allocator.hpp" // detail::named_allocator

#include "../macros.hpp"            // BIT_MEMORY_UNLIKELY, BIT_MEMORY_UNUSED
#include "../byte.hpp"              // byte
#include "../pointer_utilities.hpp" // offset_align_forward

#include <cstddef>     // std::size_t, std::max_align_t
#include <type_traits> // std::true_type, std::integral_constant
#include <cassert>     // assert

namespace bit {
  namespace memory {

    ///////////////////////////////////////////////////////////////////////////
    /// \brief A stack-allocation strategy that allocates off of stack-memory
    ///
    /// This offers two different forms of deallocations; either truncated
    /// deallocations via deallocate_all, or individual deallocations through
    /// deallocate(...) calls. The order of deallocation must be the inverse
    /// of the order of construction, otherwise it is undefined behaviour.
    ///
    /// \note Since this allocator uses stack memory, care should be taken to
    ///       avoid overflowing the stack
    ///
    /// \tparam Size The size of the storage in this allocator
    /// \tparam Align the alignment of the storage
    ///
    /// \satisfies ExtendedAllocator
    ///////////////////////////////////////////////////////////////////////////
    template<std::size_t Size, std::size_t Align = alignof(std::max_align_t)>
    class stack_allocator
    {
      //-----------------------------------------------------------------------
      // Public Member Types
      //-----------------------------------------------------------------------
    public:

      using max_alignment            = std::integral_constant<std::size_t,Align>;
      using can_truncate_allocations = std::true_type;

      //-----------------------------------------------------------------------
      // Constructors
      //-----------------------------------------------------------------------
    public:

      /// \brief Default-constructs a stack_allocator
      stack_allocator() noexcept;

      // Deleted move constructor.
      stack_allocator( stack_allocator&& other ) = delete;

      // Deleted copy constructor
      stack_allocator( const stack_allocator& other ) = delete;

      //----------------------------------------------------------------------

      /// \brief Deallocates all memory from the stack
      ~stack_allocator();

      //----------------------------------------------------------------------

      // Deleted copy assignment
      stack_allocator& operator=( const stack_allocator& ) = delete;

      // Deleted move assignment
      stack_allocator& operator=( stack_allocator&& other ) = delete;

      //-----------------------------------------------------------------------
      // Allocations / Deallocations
      //-----------------------------------------------------------------------
    public:

      /// \brief Tries to allocate memory of size \p size, aligned to the
      ///        boundary \p align, offset by \p offset
      ///
      /// \param size the size of the allocation
      /// \param align the requested alignment of the allocation
      /// \param offset the amount to offset the alignment by
      /// \return the allocated pointer on success, \c nullptr on failure
      void* try_allocate( std::size_t size,
                          std::size_t align,
                          std::size_t offset = 0 ) noexcept;

      /// \brief Does nothing for linear_allocator. Use deallocate_all
      ///
      /// \param p the pointer
      /// \param size the size of the allocation
      void deallocate( void* p, std::size_t size );

      /// \brief Deallocates all memory in this allocator
      void deallocate_all();

      //-----------------------------------------------------------------------
      // Observers
      //-----------------------------------------------------------------------
    public:

      /// \brief Checks whether \p linear_allocator contains the pointer \p p
      ///
      /// \param p the pointer to check
      /// \return \c true if \p p is contained in this allocator
      bool owns( void* p ) const noexcept;

      //-----------------------------------------------------------------------
      // Private Members
      //-----------------------------------------------------------------------
    private:

      alignas(Align) char m_storage[Size];
      void* m_current;

      template<std::size_t S, std::size_t A>
      friend bool operator==( const stack_allocator<S,A>&, const stack_allocator<S,A>& ) noexcept;
    };

    //-------------------------------------------------------------------------
    // Comparison
    //-------------------------------------------------------------------------

    template<std::size_t S, std::size_t A>
    bool operator==( const stack_allocator<S,A>& lhs, const stack_allocator<S,A>& rhs ) noexcept;

    template<std::size_t S, std::size_t A>
    bool operator!=( const stack_allocator<S,A>& lhs, const stack_allocator<S,A>& rhs ) noexcept;

    //-------------------------------------------------------------------------
    // Utilities
    //-------------------------------------------------------------------------

    template<std::size_t Size, std::size_t Align = alignof(std::max_align_t)>
    using named_stack_allocator = detail::named_allocator<stack_allocator<Size,Align>>;

  } // namespace memory
} // namespace bit

#include "detail/stack_allocator.inl"

#endif /* BIT_MEMORY_ALLOCATORS_STACK_ALLOCATOR_HPP */
