/**
 * \file aligned_memory.hpp
 *
 * \brief todo: fill in documentation
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */

#ifndef BIT_MEMORY_ALIGNED_MEMORY_HPP
#define BIT_MEMORY_ALIGNED_MEMORY_HPP

#include "memory.hpp"
#include "memory_block.hpp"
#include "memory_block_cache.hpp"
#include "cached_block_allocator.hpp"

namespace bit {
  namespace memory {

    //------------------------------------------------------------------------
    // Aligned Memory Allocations
    //------------------------------------------------------------------------

    /// \brief Allocates \p size bytes aligned to \p align alignment
    ///        boundary
    ///
    /// \note This function returns \c nullptr on failure
    ///
    /// \param size the size of the allocation
    /// \param align the alignment to perform
    /// \return pointer to the allocated memory
    void* aligned_malloc( std::size_t size, std::size_t align ) noexcept;

    /// \brief Frees memory allocated by a call to \ref aligned_malloc
    ///
    /// \param ptr the pointer to free
    void aligned_free( void* ptr );

    //------------------------------------------------------------------------
    // Aligned Offset Memory Allocations
    //------------------------------------------------------------------------

    /// \brief Allocates \p size bytes aligned to \p align alignment
    ///        boundary with the specified \p offset
    ///
    /// \note This function returns \c nullptr on failure
    ///
    /// \param size the size of the allocation
    /// \param align the alignment to perform
    /// \param offset the offset for the alignment
    /// \return pointer to the allocated memory
    void* aligned_offset_malloc( std::size_t size, std::size_t align, std::size_t offset ) noexcept;

    /// \brief Frees memory allocated by a call to \ref aligned_offset_malloc
    ///
    /// \param ptr the pointer to free
    void aligned_offset_free( void* ptr );

    //------------------------------------------------------------------------
    // Block Allocators
    //------------------------------------------------------------------------

    //////////////////////////////////////////////////////////////////////////
    /// \brief A raw allocator that allocates over-aligned memory
    ///
    /// \tparam Size The size of the block
    /// \tparam Align The alignment of block
    //////////////////////////////////////////////////////////////////////////
    template<std::size_t Size,std::size_t Align>
    class aligned_block_allocator
    {
      //----------------------------------------------------------------------
      // Public Members
      //----------------------------------------------------------------------
    public:

      static constexpr std::size_t block_size = Size;
      static constexpr std::size_t align_size = Align;

      //----------------------------------------------------------------------
      // Block Allocations
      //----------------------------------------------------------------------
    public:

      /// \brief Allocates a memory_block of size \ref block_size with
      ///        alignment \p Align
      ///
      /// \return a newly allocated memory_block
      owner<memory_block> allocate_block() noexcept;

      /// \brief Deallocates a given memory_block
      ///
      /// \param block the block to deallocate
      void deallocate_block( owner<memory_block> block ) noexcept;

    };

    /// \brief A cached version of aligned_block_allocator
    ///
    /// \tparam Size The size of the block
    /// \tparam Align The alignment of block
    template<std::size_t Size,std::size_t Align>
    using cached_aligned_block_allocator = cached_block_allocator<aligned_block_allocator<Size,Align>>;

    //------------------------------------------------------------------------

    //////////////////////////////////////////////////////////////////////////
    /// \brief A raw allocator that allocates over-aligned memory with a
    ///        given \p Offset
    ///
    /// \tparam Size The size of the block
    /// \tparam Align The alignment of block
    /// \tparam Offset The offset from the alignment
    ///
    /// \satisfies BlockAllocator
    //////////////////////////////////////////////////////////////////////////
    template<std::size_t Size,std::size_t Align, std::size_t Offset>
    class aligned_offset_block_allocator
    {
      //----------------------------------------------------------------------
      // Public Members
      //----------------------------------------------------------------------
    public:

      static constexpr std::size_t block_size  = Size;
      static constexpr std::size_t align_size  = Align;
      static constexpr std::size_t offset_size = Offset;

      //----------------------------------------------------------------------
      // Block Allocations
      //----------------------------------------------------------------------
    public:

      /// \brief Allocates a memory_block of size \ref block_size with
      ///        alignment \p Align with an offset of \ref Offset
      ///
      /// \return a newly allocated memory_block
      owner<memory_block> allocate_block() noexcept;

      /// \brief Deallocates a given memory_block
      ///
      /// \param block the block to deallocate
      void deallocate_block( owner<memory_block> block ) noexcept;

    };

    /// \brief A cached version of aligned_offset_block_allocator
    ///
    /// \tparam Size The size of the block
    /// \tparam Align The alignment of block
    /// \tparam Offset The offset from the alignment
    template<std::size_t Size,std::size_t Align,std::size_t Offset>
    using cached_aligned_offset_block_allocator = cached_block_allocator<aligned_offset_block_allocator<Size,Align,Offset>>;

    enum class align_val_t : std::size_t{};

  } // namespace memory
} // namespace bit

//----------------------------------------------------------------------------
// Aligned new
//----------------------------------------------------------------------------

void* operator new( std::size_t size,
                    bit::memory::align_val_t alignment );

//----------------------------------------------------------------------------

void* operator new( std::size_t size,
                    bit::memory::align_val_t alignment,
                    const std::nothrow_t&) noexcept;

//----------------------------------------------------------------------------
// Aligned delete
//----------------------------------------------------------------------------

void operator delete( void* ptr,
                      bit::memory::align_val_t alignment) noexcept;

//----------------------------------------------------------------------------

void operator delete( void* ptr,
                      bit::memory::align_val_t alignment,
                      std::nothrow_t& ) noexcept;

//----------------------------------------------------------------------------

void operator delete( void* ptr,
                      bit::memory::align_val_t alignment,
                      std::size_t size ) noexcept;

//----------------------------------------------------------------------------
// Aligned new[]
//----------------------------------------------------------------------------

void* operator new[]( std::size_t size,
                      bit::memory::align_val_t alignment );

//----------------------------------------------------------------------------

void* operator new[]( std::size_t size,
                      bit::memory::align_val_t alignment,
                      const std::nothrow_t& ) noexcept;

//----------------------------------------------------------------------------
// Aligned delete[]
//----------------------------------------------------------------------------

void operator delete[]( void* ptr,
                        bit::memory::align_val_t alignment ) noexcept;

//----------------------------------------------------------------------------

void operator delete[]( void* ptr,
                        bit::memory::align_val_t alignment,
                        const std::nothrow_t& ) noexcept;

//----------------------------------------------------------------------------

void operator delete[]( void* ptr,
                        bit::memory::align_val_t alignment,
                        std::size_t size ) noexcept;

// Include inline definitions
#include "detail/aligned_memory.inl"

#endif /* BIT_MEMORY_ALIGNED_MEMORY_HPP */
