/**
 * \file named_block_allocator.hpp
 *
 * \brief This header contains a composite operator that allows naming
 *        of the underlying block allocator
 *
 * \todo Consider merging with 'allocators/detail/named_allocator.hpp
 *       Merging reduces code duplication; however introduces a
 *       coupling between code indented for Allocators to code intended
 *       for BlockAllocators -- which diverge in development.
 *
 * \note This is an internal header file, included by other library headers.
 *       Do not attempt to use it directly.
 */
#ifndef BIT_MEMORY_BLOCK_ALLOCATORS_DETAIL_NAMED_BLOCK_ALLOCATOR_HPP
#define BIT_MEMORY_BLOCK_ALLOCATORS_DETAIL_NAMED_BLOCK_ALLOCATOR_HPP


#include "../../utilities/allocator_info.hpp" // allocator_info

#include <type_traits> // std::enable_if_t, std::is_constructible
#include <utility>     // std::forward

namespace bit {
  namespace memory {
    namespace detail {

      /////////////////////////////////////////////////////////////////////////
      /// \brief A wrapper around block allocators to provide a name to their
      ///        allocator info.
      ///
      /// The underlying allocator must not be marked 'final', otherwise
      /// this will not compile.
      ///
      /// \tparam BlockAllocator the underlying Allocator
      /////////////////////////////////////////////////////////////////////////
      template<typename BlockAllocator>
      class named_block_allocator : public BlockAllocator
      {
        //---------------------------------------------------------------------
        // Public Member Types
        //---------------------------------------------------------------------
      public:

        /// Named allocators cannot be stateless, since they have unique names
        using is_stateless = std::false_type;

        //---------------------------------------------------------------------
        // Constructors
        //---------------------------------------------------------------------
      public:

        /// \brief constructs a named block allocator
        ///
        /// \param str the string name of the allocator
        /// \param args arguments to forward to the underlying allocator
        template<typename...Args, typename = std::enable_if_t<std::is_constructible<BlockAllocator,Args...>::value>>
        named_block_allocator( const char* str, Args&&...args );

        //---------------------------------------------------------------------
        // Observers
        //---------------------------------------------------------------------
      public:

        /// \brief Gets the info about this block allocator
        ///
        /// \return the info for this allocator
        allocator_info info() const noexcept;

        //---------------------------------------------------------------------
        // Private Members
        //---------------------------------------------------------------------
      private:

        const char* m_name;
      };

    } // namespace detail
  } // namespace memory
} // namespace bit

#include "named_block_allocator.inl"

#endif /* BIT_MEMORY_BLOCK_ALLOCATORS_DETAIL_NAMED_BLOCK_ALLOCATOR_HPP */
