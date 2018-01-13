/**
 * \file allocator_tagger.hpp
 *
 * \brief This header contains the definition of a debug tagger that tags
 *        allocations with specific byte-patterns.
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */
#ifndef BIT_MEMORY_TAGGERS_DEBUG_MEMORY_TAGGER_HPP
#define BIT_MEMORY_TAGGERS_DEBUG_MEMORY_TAGGER_HPP

#include "../utilities/debugging.hpp" // debug_tag_allocated_bytes

#include "../concepts/MemoryTagger.hpp" // is_memory_tagger

#include <cstddef>          // std::size_t

namespace bit {
  namespace memory {

    ///////////////////////////////////////////////////////////////////////////
    /// \brief This tagger tags allocations with a specific \ref debug_tag
    ///        byte-pattern on allocations and deallocations.
    ///
    /// \satisfies{MemoryTagger}
    ///////////////////////////////////////////////////////////////////////////
    class allocator_tagger
    {
      //-----------------------------------------------------------------------
      // Tagging
      //-----------------------------------------------------------------------
    public:

      void tag_allocation( void* p, std::size_t size ) noexcept;
      void tag_deallocation( void* p, std::size_t size ) noexcept;
    };

    static_assert( is_memory_tagger_v<allocator_tagger>,
                   "allocator_tagger must satisfy MemoryTagger" );

  } // namespace memory
} // namespace debug

#include "detail/allocator_tagger.inl"

#endif /* BIT_MEMORY_TAGGERS_DEBUG_MEMORY_TAGGER_HPP */
