/**
 * \file memory_block.hpp
 *
 * \brief This header contains implementation for both the memory_block and
 *        the memory_block_cache.
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */

#ifndef BIT_MEMORY_MEMORY_BLOCK_HPP
#define BIT_MEMORY_MEMORY_BLOCK_HPP

#include <cstdlib> // std::size_t
#include <utility> // std::swap

namespace bit {
  namespace memory {
    namespace detail {
      struct memory_block_arg{};
    }

    /// \brief An instance to represent null memory blocks
#ifndef BIT_DOXYGEN_BUILD
    inline void nullblock( detail::memory_block_arg& ){}
#else
    void nullblock( /* implementation defined */ );
#endif

    /// \brief The type of \ref nullblock
    using nullblock_t = decltype(*nullblock);

    //////////////////////////////////////////////////////////////////////////
    /// \brief Wrapper around a block of memory, containing both the size
    ///        and the address of the memory block.
    ///
    /// \ingroup memory
    //////////////////////////////////////////////////////////////////////////
    class memory_block
    {
      //----------------------------------------------------------------------
      // Constructors / Assignment
      //----------------------------------------------------------------------
    public:

      /// \brief Constructs a memory block pointing to null with 0 size
      constexpr explicit memory_block() noexcept;

      /// \copydoc memory_block()
      constexpr memory_block( nullblock_t ) noexcept;

      /// \brief Constructs a memory_block at the specified memory address,
      ///        \p data, with the specified size \p size
      ///
      /// \param data The memory address to start the block
      /// \param size The size of the memory block
      constexpr explicit memory_block( void* data, std::size_t size ) noexcept;

      /// \brief Copy-constructs a memory_block by copying another memory_block
      ///
      /// \param other the other memory_block to copy
      constexpr memory_block( const memory_block& other ) noexcept = default;

      /// \brief Move-constructs a memory_block by copying another memory_block
      ///
      /// \param other the other memory_block to move
      constexpr memory_block( memory_block&& other ) noexcept = default;

      //----------------------------------------------------------------------

      /// \brief Copy-assigns a memory_block from another memory_block
      ///
      /// \param other the other memory_block to copy
      /// \return reference to \c (*this)
      memory_block& operator=( const memory_block& other ) noexcept = default;

      /// \brief Move-assigns a memory_block from another memory_block
      ///
      /// \param other the other memory_block to move
      /// \return reference to \c (*this)
      memory_block& operator=( memory_block&& other ) noexcept = default;

      //----------------------------------------------------------------------
      // Element Access
      //----------------------------------------------------------------------
    public:

      /// \brief Retrieves the pointer to the data
      ///
      /// \return the pointer to the data
      constexpr void* data() noexcept;

      /// \copydoc memory_block::data()
      constexpr const void* data() const noexcept;

      //----------------------------------------------------------------------
      // Observers
      //----------------------------------------------------------------------
    public:

      /// \brief Retrieves the size of a data
      ///
      /// \return the size of the data
      constexpr std::size_t size() const noexcept;

      /// \brief Evaluates whether \p ptr exists within this memory_block
      ///
      /// \param ptr the pointer to evaluate
      /// \return \c true whether \p ptr
      bool contains( const void* ptr ) const noexcept;

      /// \brief Contextually convertible to \c bool
      constexpr explicit operator bool() const noexcept;

      //----------------------------------------------------------------------
      // Mutators
      //----------------------------------------------------------------------
    public:

      /// \brief Swaps this memory_block with \p other
      ///
      /// \param other the other entry to swap
      void swap( memory_block& other ) noexcept;

      //----------------------------------------------------------------------
      // Private Member Types
      //----------------------------------------------------------------------
    private:

      void*       m_data; ///< Pointer to the start of the memory block
      std::size_t m_size; ///< The size of the memory block
    };

    //------------------------------------------------------------------------
    // Free Functions
    //------------------------------------------------------------------------

    /// \brief Swaps two memory blocks
    ///
    /// \param lhs the left block to swap
    /// \param rhs the right block to swap
    void swap( memory_block& lhs, memory_block& rhs ) noexcept;

    constexpr bool operator == ( const memory_block& lhs, const memory_block& rhs ) noexcept;
    constexpr bool operator == ( nullblock_t, const memory_block& rhs ) noexcept;
    constexpr bool operator == ( const memory_block& lhs, nullblock_t ) noexcept;

    constexpr bool operator != ( const memory_block& lhs, const memory_block& rhs ) noexcept;
    constexpr bool operator != ( nullblock_t, const memory_block& rhs ) noexcept;
    constexpr bool operator != ( const memory_block& lhs, nullblock_t ) noexcept;

  } // namespace memory
} // namespace bit

#include "detail/memory_block.inl"

#endif /* BIT_MEMORY_MEMORY_BLOCK_HPP */
