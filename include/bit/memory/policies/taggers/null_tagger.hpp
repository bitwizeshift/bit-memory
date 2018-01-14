/**
 * \file null_tagger.hpp
 *
 * \brief This header contains a null (no-op) MemoryTagger
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
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
