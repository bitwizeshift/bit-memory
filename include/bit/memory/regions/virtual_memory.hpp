/**
 * \file virtual_memory.hpp
 *
 * \brief This header provides an implementation of virtual memory access
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */
#ifndef BIT_MEMORY_REGIONS_VIRTUAL_MEMORY_HPP
#define BIT_MEMORY_REGIONS_VIRTUAL_MEMORY_HPP

#include "../utilities/memory_block.hpp"       // memory_block
#include "../utilities/memory_block_cache.hpp" // memory_block_cache

#include <cstdint>   // std::size_t & std::ptrdiff_t
#include <cstddef>   // std::size_t
#include <stdexcept> // std::out_of_range

namespace bit {
  namespace memory {

    //------------------------------------------------------------------------
    // Global Constants
    //------------------------------------------------------------------------

    /// \brief Retrieves the page size of the virtual memory
    ///
    /// \return the page size of the virtual memory
    std::size_t virtual_memory_page_size() noexcept;

    //------------------------------------------------------------------------
    // Global Functions
    //------------------------------------------------------------------------

    /// \brief Reserves \p n pages of virtual memory.
    ///
    /// Each page is \ref virtual_memory_page_size in size. The memory may not
    /// be used until after it has first been commited with a call to
    /// \ref virtual_memory_commit
    ///
    /// \param n the number of pages to reserve
    /// \return pointer to the reserved memory
    void* virtual_memory_reserve( std::size_t n ) noexcept;

    /// \brief Commits \p n pages of memory to virtual memory
    ///
    /// \param memory Memory pointing to a page to commit
    /// \param n The number of pages to commit
    /// \return pointer to the committed memory
    void* virtual_memory_commit( void* memory, std::size_t n ) noexcept;

    /// \brief Decommits \p n pages of memory to virtual memory
    ///
    /// \param memory Memory pointing to a page to decommit
    /// \param n The number of pages to decommit
    void virtual_memory_decommit( void* memory, std::size_t n ) noexcept;

    /// \brief Releases \p n pages of virtual memory
    ///
    /// \param memory The memory originally returned from virtual_memory_reserve
    /// \param n The number of pages to release
    void virtual_memory_release( void* memory, std::size_t n ) noexcept;

    //------------------------------------------------------------------------
    // Classes
    //------------------------------------------------------------------------

    //////////////////////////////////////////////////////////////////////////
    /// \brief This class is a small wrapper around virtual memory access
    ///        that uses RAII to allocate and free the memory.
    ///
    /// This class can be used to access memory_blocks that contain the
    /// bounds of each virtual page.
    //////////////////////////////////////////////////////////////////////////
    class virtual_memory
    {
      //----------------------------------------------------------------------
      // Constructors
      //----------------------------------------------------------------------
    public:

      /// \brief Constructs a virtual_memory object containing \p pages
      ///
      /// \param pages The number of pages
      explicit virtual_memory( std::size_t pages );

      /// Deleted copy constructor
      virtual_memory( const virtual_memory& ) = delete;

      /// \brief Move-construct a virtual_memory object
      ///
      /// \param other the pages to move
      virtual_memory( virtual_memory&& other ) noexcept;

      /// \brief Destructs and releases all pages
      ~virtual_memory();

      //----------------------------------------------------------------------

      /// Deleted copy assignment
      virtual_memory& operator=( const virtual_memory& ) = delete;

      /// \brief Move-assigns a virtual_memory object
      ///
      /// \param other the page to move
      /// \return a reference to \c (*this)
      virtual_memory& operator=( virtual_memory&& other ) noexcept;

      //----------------------------------------------------------------------
      // Modifiers
      //----------------------------------------------------------------------
    public:

      /// \brief Commits the \p n'th page to virtual memory
      ///
      /// \param n the page number to commit
      void commit( std::ptrdiff_t n ) noexcept;

      /// \brief Decommits the \p n'th page
      ///
      /// \param n the page number to decommit
      void decommit( std::ptrdiff_t n ) noexcept;

      /// \brief Releases the virtual memory controlled by this class
      ///
      /// The underlying data is \c nullptr after this call
      ///
      /// \return a pointer to the memory
      void* release() noexcept;

      //----------------------------------------------------------------------
      // Observers
      //----------------------------------------------------------------------
    public:

      /// \brief Gets the underlying memory
      ///
      /// \return a pointer to the memory
      void* get() const noexcept;

      /// \brief Returns the size of this virtual_memory class in bytes
      ///
      /// \return the size of the virtual_memory
      std::size_t size() const noexcept;

      /// \brief Returns the number of pages in this virtual_memory
      ///
      /// \return the number of pages
      std::size_t pages() const noexcept;

      //----------------------------------------------------------------------
      // Element Access
      //----------------------------------------------------------------------
    public:

      /// \brief Accesses the virtual memory page at index \p n
      ///
      /// \throws std::out_of_range if index is greater than \c pages()
      /// \param n the nth block to access
      /// \return the memory block
      memory_block at( std::ptrdiff_t n ) const;

      /// \brief Accesses the virtual memory block at index \p n
      ///
      /// \param n the nth block to access
      /// \return the memory block
      memory_block operator[]( std::ptrdiff_t n ) const noexcept;

      //----------------------------------------------------------------------
      // Private Members
      //----------------------------------------------------------------------
    private:

      void*       m_data;
      std::size_t m_pages;
    };

  } // namespace memory
} // namespace bit

#include "detail/virtual_memory.inl"

#endif /* BIT_MEMORY_REGIONS_VIRTUAL_MEMORY_HPP */
