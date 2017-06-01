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

#include <cstddef>   // std::size_t
#include <algorithm> // std::foreach

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

    /// \brief The type used by leak handler
    using leak_handler_t = void (*)(const allocator_info&, const void*, std::ptrdiff_t);

    /// \brief The type used by stomp handler
    using stomp_handler_t = void (*)(const allocator_info&, const void*, std::ptrdiff_t);

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

    //----------------------------------------------------------------------
    // Debug Allocator Adapters
    //----------------------------------------------------------------------

    //////////////////////////////////////////////////////////////////////////
    /// \brief A block allocator adapter that tags allocated and freed
    ///        memory blocks.
    ///
    /// All allocated blocks are tagged with the pattern
    /// \c debug_tag::allocated_block_byte, and deallocated blocks are tagged
    /// with the pattern \c debug_tag::freed_block_byte
    ///
    /// \satisfies BlockAllocator
    //////////////////////////////////////////////////////////////////////////
    template<typename BlockAllocator>
    class debug_block_allocator
    {
      //----------------------------------------------------------------------
      // Public Member Types
      //----------------------------------------------------------------------
    public:

      static constexpr std::size_t block_size = BlockAllocator::block_size;

      //----------------------------------------------------------------------
      // Constructor
      //----------------------------------------------------------------------
    public:

      /// \brief Constructs a debug_block_allocator from another allocator
      ///
      /// \param alloc the underlying allocator to construct out of
      template<typename...Args>
      explicit debug_block_allocator( Args&&...args );

      /// \brief Copy-constructs a debug_block_allocator from another one
      ///
      /// \param other the other allocator to copy
      debug_block_allocator( const debug_block_allocator& other ) = default;

      /// \brief Move-constructs a debug_block_allocator from another one
      ///
      /// \param other the other allocator to move
      debug_block_allocator( debug_block_allocator&& other ) noexcept = default;

      //----------------------------------------------------------------------

      /// \brief Destructs the block allocator
      ~debug_block_allocator();

      //----------------------------------------------------------------------

      /// \brief Copy-assigns a debug_block_allocator from another one
      ///
      /// \param other the other allocator to copy
      /// \return a reference to \c (*this)
      debug_block_allocator& operator=( const debug_block_allocator& other ) = default;

      /// \brief Move-assigns a debug_block_allocator from another one
      ///
      /// \param other the other allocator to move
      /// \return a reference to \c (*this)
      debug_block_allocator& operator=( debug_block_allocator&& other ) noexcept = default;

      //----------------------------------------------------------------------
      // Block Allocations
      //----------------------------------------------------------------------
    public:

      /// \brief Allocates a block from the underlying allocator, tagging the
      ///        memory block with a specific pattern
      ///
      /// \return the memory block
      owner<memory_block> allocate_block();

      /// \brief Deallocates a block from the underlying container, tagging
      ///        the memroy block with a specified pattern first
      ///
      /// This function stores the memory block in the cache for later use
      ///
      /// \param block the block to deallocate
      void deallocate_block( owner<memory_block> block );

      //----------------------------------------------------------------------
      // Private Member Types
      //----------------------------------------------------------------------
    private:

      struct storage_type : BlockAllocator
      {
        template<typename...Args>
        storage_type(Args&&...args)
          : BlockAllocator( std::forward<Args>(args)... ),
            size(0),
            allocations(0)
        {

        }
        std::ptrdiff_t size;        ///< The total size during debugging
        std::ptrdiff_t allocations; ///< The number of allocations
      };

      //----------------------------------------------------------------------
      // Private Members
      //----------------------------------------------------------------------
    private:

      storage_type m_storage;
    };

    //////////////////////////////////////////////////////////////////////////
    /// \brief An allocator adapter that, in addition to tagging allocated and
    ///        freed blocks, also provides additional tagging for fencing and
    ///        padded blocks
    ///
    /// All allocated bytes are tagged with the pattern
    /// \c debug_tag::allocated_byte, and deallocated are tagged with the
    /// pattern \c debug_tag::freed_byte. If any fences are specified, the
    /// beginning fence is tagged with \c debug_tag::fence_start_byte , and
    /// the ending fence is tagged with \c debug_tag::fence_end_byte.
    ///
    /// \satisfies Allocator
    //////////////////////////////////////////////////////////////////////////
    template<typename Allocator>
    class debug_allocator
    {
      //----------------------------------------------------------------------
      // Constructors / Destructor / Assignment
      //----------------------------------------------------------------------
    public:

      /// \brief Constructs a debug_allocator from another allocator with a
      ///        specified fence
      ///
      /// \param start the size of the starting fence
      /// \param end   the size of the ending fence
      /// \param alloc the underlying allocator to construct out of
      template<typename...Args>
      explicit debug_allocator( std::size_t start, std::size_t end, Args&&...args );

      /// \brief Copy-constructs a debug_allocator from another one
      ///
      /// \param other the other allocator to copy
      debug_allocator( const debug_allocator& other ) = default;

      /// \brief Move-constructs a debug_allocator from another one
      ///
      /// \param other the other allocator to move
      debug_allocator( debug_allocator&& other ) noexcept = default;

      //----------------------------------------------------------------------

      /// \brief Destructs the block allocator
      ~debug_allocator();

      //----------------------------------------------------------------------

      /// \brief Copy-assigns a debug_allocator from another one
      ///
      /// \param other the other allocator to copy
      /// \return a reference to \c (*this)
      debug_allocator& operator=( const debug_allocator& other ) = default;

      /// \brief Move-assigns a debug_allocator from another one
      ///
      /// \param other the other allocator to move
      /// \return a reference to \c (*this)
      debug_allocator& operator=( debug_allocator&& other ) noexcept = default;

      //----------------------------------------------------------------------
      // Memory Allocations
      //----------------------------------------------------------------------
    public:

      /// \brief Allocates memory that will be suitably aligned to the
      ///        specified byte boundary.
      ///
      /// \param bytes the number of bytes to allocate
      /// \param align the alignment of the allocation
      /// \param offset the offset of the alignment
      /// \param [out] padding the number of bytes used for padding
      /// \return the allocated memory
      void* allocate( std::size_t bytes,
                      std::size_t align,
                      std::size_t offset = 0,
                      std::ptrdiff_t* padding = nullptr );

      /// \brief Deallocates a pointer previously allocated from
      ///        a call to allocate
      ///
      /// \param ptr the pointer to deallocate
      /// \param bytes the size of the allocation
      void deallocate( void* ptr, std::size_t bytes );

      //----------------------------------------------------------------------
      // Private Member Types
      //----------------------------------------------------------------------
    private:

      struct storage_type : Allocator
      {
        template<typename...Args>
        storage_type( std::size_t start, std::size_t end,  Args&&...args )
          : Allocator( std::forward<Args>(args)... ),
            fence_start_size(start),
            fence_end_size(end),
            size(0),
            allocations(0)
        {

        }

        std::size_t    fence_start_size; ///< The number of bytes for the fence start
        std::size_t    fence_end_size;   ///< The number of bytes for the fence end
        std::ptrdiff_t size;             ///< The total size during debugging
        std::ptrdiff_t allocations;      ///< The number of allocations
      };

      //----------------------------------------------------------------------
      // Private Members
      //----------------------------------------------------------------------
    private:

      storage_type m_storage;
    };

  } // namespace memory
} // namespace bit

#include "detail/debugging.inl"

#endif /* BIT_MEMORY_DEBUGGING_HPP */
