/*****************************************************************************
 * \file
 * \brief This header contains functions and global new operators for
 *        allocating blocks of aligned memory
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
#ifndef BIT_MEMORY_REGIONS_ALIGNED_HEAP_MEMORY_HPP
#define BIT_MEMORY_REGIONS_ALIGNED_HEAP_MEMORY_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

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

#endif /* BIT_MEMORY_REGIONS_ALIGNED_HEAP_MEMORY_HPP */
