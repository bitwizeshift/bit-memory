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

    class allocator_info
    {
      //----------------------------------------------------------------------
      // Constructors
      //----------------------------------------------------------------------
    public:

      allocator_info( const char* name, const void* allocator );

      //----------------------------------------------------------------------
      // Observers
      //----------------------------------------------------------------------
    public:

      const char* name() const noexcept;

      const void* allocator() const noexcept;

      //----------------------------------------------------------------------
      // Private Members
      //----------------------------------------------------------------------
    private:

      const char* m_name;      ///< A non-owned null-terminated byte-string to
                               ///< a name for this debugger
      const void* m_allocator; ///< Raw pointer to an allocator
    };

    //------------------------------------------------------------------------
    // Comparisons
    //------------------------------------------------------------------------

    bool operator==( const allocator_info& lhs, const allocator_info& rhs ) noexcept;
    bool operator!=( const allocator_info& lhs, const allocator_info& rhs ) noexcept;

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

    //------------------------------------------------------------------------
    // Types
    //------------------------------------------------------------------------

    /// \brief The type used by the leak handler
    using leak_handler_t = void(*)(const allocator_info&, const void*, std::ptrdiff_t);

    /// \brief The type used by the stomp handler
    using stomp_handler_t = void(*)(const allocator_info&, const void*, std::ptrdiff_t);

    /// \brief The type used by the double delete handler
    using double_delete_handler_t = void(*)(const allocator_info&, const void*, std::ptrdiff_t);

    /// \brief The type used by the out-of-memory handler
    using out_of_memory_handler_t = void(*)(const allocator_info&, std::size_t );

    //------------------------------------------------------------------------
    // Leak Handler
    //------------------------------------------------------------------------

    /// \brief Makes f the new global memory leak handler function and returns
    ///        the previously installed memory::leak_handler.
    ///
    /// \param f the new handler
    /// \return the old handler
    leak_handler_t set_leak_handler(leak_handler_t f) noexcept;

    /// \brief Gets the current leak handler
    ///
    /// \return the old handler
    leak_handler_t get_leak_handler() noexcept;

    /// \brief Handles memory leaks from an allocator identified by
    ///        the \ref allocator_info object
    ///
    /// This invokes the handler stored by \ref set_leak_handler
    ///
    /// \param info the allocator_info object
    /// \param size the size of the leak
    void leak_handler( const allocator_info& info, const void* ptr, std::ptrdiff_t size );

    //------------------------------------------------------------------------
    // Stomp Handler
    //------------------------------------------------------------------------

    /// \brief Makes f the new global memory stomp handler function and returns
    ///        the previously installed memory::stomp_handler.
    ///
    /// \param f the new handler
    /// \return the old handler
    stomp_handler_t set_stomp_handler(stomp_handler_t f) noexcept;

    /// \brief Gets the current stomp handler
    ///
    /// \return the old handler
    stomp_handler_t get_stomp_handler() noexcept;

    /// \brief Handles memory stomps from an allocator identified by
    ///        the \ref allocator_info object
    ///
    /// This invokes the handler stored by \ref set_stomp_handler
    ///
    /// \param info the allocator_info object
    /// \param ptr a pointer to the object that detected the stomp
    /// \param size the number of bytes stomped
    void stomp_handler( const allocator_info& info, const void* ptr, std::ptrdiff_t size );

    //------------------------------------------------------------------------
    // Double Delete Handler
    //------------------------------------------------------------------------

    /// \brief Makes f the new global double-delete handler function and returns
    ///        the previously installed memory::double_delete_handler.
    ///
    /// \param f the new handler
    /// \return the old handler
    double_delete_handler_t set_double_delete_handler(double_delete_handler_t f) noexcept;

    /// \brief Gets the current double-delete handler
    ///
    /// \return the old handler
    double_delete_handler_t get_double_delete_handler() noexcept;

    /// \brief Handles double-deletions from an allocator identified by
    ///        the \ref allocator_info object
    ///
    /// This invokes the handler stored by \ref set_double_delete_handler
    ///
    /// \param info the allocator_info object
    /// \param ptr a pointer to the object that was double-deleted
    /// \param size the number of bytes double-deleted
    void double_delete_handler( const allocator_info& info, const void* ptr, std::ptrdiff_t size );

    //------------------------------------------------------------------------
    // Out-of-memory Handler
    //------------------------------------------------------------------------

    /// \brief Makes f the new global out-of-memory handler function and returns
    ///        the previously installed memory::out_of_memory_handler.
    ///
    /// \param f the new handler
    /// \return the old handler
    out_of_memory_handler_t set_out_of_memory_handler(out_of_memory_handler_t f) noexcept;

    /// \brief Gets the current out-of-memory handler
    ///
    /// \return the old handler
    out_of_memory_handler_t get_out_of_memory_handler() noexcept;

    /// \brief Handles out-of-memory requests
    ///
    /// This invokes the handler stored by \ref set_out_of_memory_handler
    ///
    /// \param info the allocator_info object
    /// \param size the size of the memory request
    void out_of_memory_handler( const allocator_info& info, std::size_t size );

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

#include "detail/debugging.inl"

#endif /* BIT_MEMORY_DEBUGGING_HPP */
