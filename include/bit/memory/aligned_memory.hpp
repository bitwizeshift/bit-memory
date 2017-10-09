/**
 * \file aligned_memory.hpp
 *
 * \brief This header contains functions and global new operators for
 *        allocating blocks of aligned memory
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */
#ifndef BIT_MEMORY_ALIGNED_MEMORY_HPP
#define BIT_MEMORY_ALIGNED_MEMORY_HPP

#include <new> // std::nothrow_t

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

#endif /* BIT_MEMORY_ALIGNED_MEMORY_HPP */
