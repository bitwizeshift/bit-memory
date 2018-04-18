/*****************************************************************************
 * \file
 * \brief This header contains a null (no-op) MemoryTagger
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
#ifndef BIT_MEMORY_POLICIES_TAGGERS_NULL_TAGGER_HPP
#define BIT_MEMORY_POLICIES_TAGGERS_NULL_TAGGER_HPP

#include "../../concepts/MemoryTagger.hpp" // is_memory_tagger

#include <cstddef> // std::size_t

namespace bit {
  namespace memory {

    ///////////////////////////////////////////////////////////////////////////
    /// \brief This class is a no-op MemoryTagger; for use in composition in
    ///        the arena_allocator
    ///
    /// \satisfies{MemoryTagger}
    ///////////////////////////////////////////////////////////////////////////
    class null_tagger
    {
      //-----------------------------------------------------------------------
      // Tagging
      //-----------------------------------------------------------------------
    public:

      void tag_allocation( void*, std::size_t ){}
      void tag_deallocation( void*, std::size_t ){}
    };

    static_assert( is_memory_tagger_v<null_tagger>,
                   "null_tagger must satisfy MemoryTagger" );

  } // namespace memory
} // namespace bit

#endif /* BIT_MEMORY_POLICIES_TAGGERS_NULL_TAGGER_HPP */
