/**
 * \file debug_allocator.hpp
 *
 * \brief todo: fill in documentation
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */

#ifndef BIT_MEMORY_DEBUG_ALLOCATOR_HPP
#define BIT_MEMORY_DEBUG_ALLOCATOR_HPP

#include "memory.hpp"
#include "debugging.hpp"

namespace bit {
  namespace memory {

    //////////////////////////////////////////////////////////////////////////
    /// \brief An allocator adapter that, in addition to tagging allocated and
    ///        freed blocks, also provides additional tagging for fencing and
    ///        padded blocks
    ///
    /// All allocated bytes are tagged with the pattern
    /// \c debug_tag::allocated_byte, and deallocated are tagged with the
    /// pattern \c debug_tag::freed_byte. If any fences are specified, the
    /// beginning fence is tagged with \c debug_tag::fence_start_byte , and
    /// the ending fence is tagged with \c debug_tag::fence_end_byte.
    ///
    /// \satisfies Allocator
    //////////////////////////////////////////////////////////////////////////
    template<typename Allocator>
    class debug_allocator
    {
      static_assert( is_allocator<Allocator>::value, "'Allocator' must satisfy the Allocator concept" );

      //----------------------------------------------------------------------
      // Constructors / Destructor / Assignment
      //----------------------------------------------------------------------
    public:

      /// \brief Constructs a debug_allocator from another allocator with a
      ///        specified fence
      ///
      /// \param start the size of the starting fence
      /// \param end   the size of the ending fence
      /// \param alloc the underlying allocator to construct out of
      template<typename...Args, decltype(Allocator( std::declval<Args>()... ), void())* = nullptr>
      explicit debug_allocator( std::size_t start, std::size_t end, Args&&...args );

      /// \brief Copy-constructs a debug_allocator from another one
      ///
      /// \param other the other allocator to copy
      debug_allocator( const debug_allocator& other ) = default;

      /// \brief Move-constructs a debug_allocator from another one
      ///
      /// \param other the other allocator to move
      debug_allocator( debug_allocator&& other ) noexcept = default;

      //----------------------------------------------------------------------

      /// \brief Destructs the block allocator
      ~debug_allocator();

      //----------------------------------------------------------------------

      /// \brief Copy-assigns a debug_allocator from another one
      ///
      /// \param other the other allocator to copy
      /// \return a reference to \c (*this)
      debug_allocator& operator=( const debug_allocator& other ) = default;

      /// \brief Move-assigns a debug_allocator from another one
      ///
      /// \param other the other allocator to move
      /// \return a reference to \c (*this)
      debug_allocator& operator=( debug_allocator&& other ) noexcept = default;

      //----------------------------------------------------------------------
      // Memory Allocations
      //----------------------------------------------------------------------
    public:

      /// \brief Allocates memory that will be suitably aligned to the
      ///        specified byte boundary.
      ///
      /// \param bytes the number of bytes to allocate
      /// \param align the alignment of the allocation
      /// \param offset the offset of the alignment
      /// \param [out] padding the number of bytes used for padding
      /// \return the allocated memory
      void* allocate( std::size_t bytes,
                      std::size_t align,
                      std::size_t offset = 0,
                      std::ptrdiff_t* padding = nullptr );

      /// \brief Deallocates a pointer previously allocated from
      ///        a call to allocate
      ///
      /// \param ptr the pointer to deallocate
      /// \param bytes the size of the allocation
      void* deallocate( void* ptr, std::size_t bytes );

      //----------------------------------------------------------------------
      // Private Member Types
      //----------------------------------------------------------------------
    private:

      struct storage_type : Allocator
      {
        template<typename...Args>
        storage_type( std::size_t start, std::size_t end,  Args&&...args )
          : Allocator( std::forward<Args>(args)... ),
            fence_start_size(start),
            fence_end_size(end),
            size(0),
            allocations(0)
        {

        }

        std::size_t    fence_start_size; ///< The number of bytes for the fence start
        std::size_t    fence_end_size;   ///< The number of bytes for the fence end
        std::ptrdiff_t size;             ///< The total size during debugging
        std::ptrdiff_t allocations;      ///< The number of allocations
      };

      //----------------------------------------------------------------------
      // Private Members
      //----------------------------------------------------------------------
    private:

      storage_type m_storage;
    };

  } // namespace memory
} // namespace bit

#include "detail/debug_allocator.inl"

#endif /* BIT_MEMORY_DEBUG_ALLOCATOR_HPP */
