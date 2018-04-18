/*****************************************************************************
 * \file
 * \brief This header contains the definition of a debug tagger that tags
 *        allocations with specific byte-patterns.
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
#ifndef BIT_MEMORY_POLICIES_TAGGERS_BLOCK_ALLOCATOR_TAGGER_HPP
#define BIT_MEMORY_POLICIES_TAGGERS_BLOCK_ALLOCATOR_TAGGER_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "../../utilities/debugging.hpp" // debug_tag_allocated_bytes

#include "../../concepts/MemoryTagger.hpp" // is_memory_tagger

#include <cstddef>          // std::size_t

namespace bit {
  namespace memory {

    ///////////////////////////////////////////////////////////////////////////
    /// \brief This tagger tags block allocations with a specific \ref debug_tag
    ///        byte-pattern on allocations and deallocations.
    ///
    /// \satisfies{MemoryTagger}
    ///////////////////////////////////////////////////////////////////////////
    class block_allocator_tagger
    {
      //-----------------------------------------------------------------------
      // Tagging
      //-----------------------------------------------------------------------
    public:

      void tag_allocation( void* p, std::size_t size ) noexcept;
      void tag_deallocation( void* p, std::size_t size ) noexcept;
    };


    static_assert( is_memory_tagger_v<block_allocator_tagger>,
                   "block_allocator_tagger must satisfy MemoryTagger" );

  } // namespace memory
} // namespace debug

#include "detail/block_allocator_tagger.inl"

#endif /* BIT_MEMORY_POLICIES_TAGGERS_BLOCK_ALLOCATOR_TAGGER_HPP */
