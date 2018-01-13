/**
 * \file allocator_info.hpp
 *
 * \brief This header contains the generic identifier used for identifying
 *        unique named allocators
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */
#ifndef BIT_MEMORY_UTILITIES_ALLOCATOR_INFO_HPP
#define BIT_MEMORY_UTILITIES_ALLOCATOR_INFO_HPP

namespace bit {
  namespace memory {

    ///////////////////////////////////////////////////////////////////////////
    /// \brief This type is used to hold the generic information for a given
    ///        allocator
    ///////////////////////////////////////////////////////////////////////////
    class allocator_info
    {
      //-----------------------------------------------------------------------
      // Constructors
      //-----------------------------------------------------------------------
    public:

      /// \brief Constructs an allocator_info with a given \p name and the
      ///        \p address of the allocator
      ///
      /// \param name the name of the allocator
      /// \param address the (possibly null) address of the allocator
      allocator_info( const char* name, const void* address );

      //-----------------------------------------------------------------------
      // Observers
      //-----------------------------------------------------------------------
    public:

      /// \brief Gets the name of the allocator
      ///
      /// \return the name of the allocator
      const char* name() const noexcept;

      /// \brief Gets the address of the allocator
      ///
      /// \return the address of the allocator
      const void* address() const noexcept;

      //-----------------------------------------------------------------------
      // Private Members
      //-----------------------------------------------------------------------
    private:

      const char* m_name;     ///< A non-owned null-terminated byte-string to
                              ///< a name for this debugger
      const void* m_address; ///< Raw pointer to an allocator
    };

    //-------------------------------------------------------------------------
    // Comparisons
    //-------------------------------------------------------------------------

    bool operator==( const allocator_info& lhs, const allocator_info& rhs ) noexcept;
    bool operator!=( const allocator_info& lhs, const allocator_info& rhs ) noexcept;


  } // namespace memory
} // namespace bit

#include "detail/allocator_info.inl"

#endif /* BIT_MEMORY_UTILITIES_ALLOCATOR_INFO_HPP */
