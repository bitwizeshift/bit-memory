/**
 * \file debugging.hpp
 *
 * \brief todo: fill in documentation
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */
#ifndef BIT_MEMORY_DEBUGGING_HPP
#define BIT_MEMORY_DEBUGGING_HPP

#include "memory.hpp"       // owner
#include "memory_block.hpp" // memory_block
#include "concepts.hpp"

#include <cstddef>   // std::size_t

namespace bit {
  namespace memory {

    //////////////////////////////////////////////////////////////////////////
    /// \brief This enumeration contains all tag byte marks that are used for
    ///        debugging allocations.
    //////////////////////////////////////////////////////////////////////////
    enum class debug_tag : unsigned char
    {
        allocated_block_byte = 0xAB, //!< Memory allocated in a block, but not
                                     //!< from an allocator
        freed_block_byte     = 0xFB, //!< Memory freed
        allocated_byte       = 0xCD, //!< Memory allocated by an allocator
        freed_byte           = 0xDD, //!< Memory freed by an allocator
        padding_byte         = 0xED, //!< byte mark for generated padding for
                                     //!< alignments or offsets
        fence_start_byte     = 0x19, //!< byte mark for low-order addresses to
                                     //!< determine memory stomps
        fence_end_byte       = 0x91, //!< byte mark for high-order addresses to
                                     //!< determine memory stomps
    };

    //////////////////////////////////////////////////////////////////////////
    /// \brief A range of bytes for iterating all values for debugging
    ///
    //////////////////////////////////////////////////////////////////////////
    struct byte_range
    {
      //----------------------------------------------------------------------
      // Constructor
      //----------------------------------------------------------------------
    public:

      /// \brief Constructs a byte range from a start and end pointer
      ///
      /// \param first the starting pointer
      /// \param last the end pointer
      byte_range( const byte* first, const byte* last ) noexcept;

      //----------------------------------------------------------------------
      // Iterators
      //----------------------------------------------------------------------
    public:

      /// \brief Gets the beginning iterator for this byte range
      ///
      /// \return the starting byte pointer
      const byte* begin() const noexcept;

      /// \brief Gets the end iterator for this byte range
      ///
      /// \return the end byte pointer
      const byte* end() const noexcept;

      //----------------------------------------------------------------------
      // Private Members
      //----------------------------------------------------------------------
    private:

      const byte* m_first;
      const byte* m_last;
    };

    //------------------------------------------------------------------------
    // Utilities
    //------------------------------------------------------------------------

    /// \brief Makes a range of bytes from a given memory block
    ///
    /// \param block the memory block
    /// \return the byte range
    byte_range make_byte_range( memory_block block ) noexcept;

    /// \brief Makes a range of bytes from a given piece of raw data
    ///
    /// \param data pointer to the raw data
    /// \param n the size
    /// \return the byte range
    byte_range make_byte_range( const void* data, std::size_t n ) noexcept;

  } // namespace memory
} // namespace bit

#endif /* BIT_MEMORY_DEBUGGING_HPP */
