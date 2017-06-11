/**
 * \file memory.hpp
 *
 * \brief todo: fill in documentation
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */

#ifndef BIT_MEMORY_MEMORY_HPP
#define BIT_MEMORY_MEMORY_HPP

#include <cstddef> // std::size_t
#include <utility> // std::forward
#include <cstdint> // std::uintN_t/std::intN_t

namespace bit {
  namespace memory {

    enum class byte : unsigned char{};

    //------------------------------------------------------------------------
    // Operators
    //------------------------------------------------------------------------

#ifndef BIT_DOXYGEN_BUILD
    template<typename IntT, typename = std::enable_if_t<std::is_integral<IntT>::value>>
#else
    template<typename IntT>
#endif
    constexpr byte operator<<(byte lhs, IntT shift) noexcept;

#ifndef BIT_DOXYGEN_BUILD
    template<typename IntT, typename = std::enable_if_t<std::is_integral<IntT>::value>>
#else
    template<typename IntT>
#endif
    constexpr byte operator>>(byte lhs, IntT shift) noexcept;
    constexpr byte operator|(byte lhs, byte rhs) noexcept;
    constexpr byte operator&(byte lhs, byte rhs) noexcept;
    constexpr byte operator^(byte lhs, byte rhs) noexcept;
    constexpr byte operator~(byte lhs) noexcept;

    //------------------------------------------------------------------------

#ifndef BIT_DOXYGEN_BUILD
    template<typename IntT, typename = std::enable_if_t<std::is_integral<IntT>::value>>
#else
    template<typename IntT>
#endif
    constexpr byte& operator<<=(byte& lhs, IntT shift) noexcept;

#ifndef BIT_DOXYGEN_BUILD
    template<typename IntT, typename = std::enable_if_t<std::is_integral<IntT>::value>>
#else
    template<typename IntT>
#endif
    constexpr byte& operator>>=(byte& lhs, IntT shift) noexcept;

    byte& operator|=(byte& lhs, byte rhs) noexcept;
    byte& operator&=(byte& lhs, byte rhs) noexcept;
    byte& operator^=(byte& lhs, byte rhs) noexcept;

    //------------------------------------------------------------------------

    /// \brief Identity alias used for denoting ownership on API calls
    ///
    /// A function returning a type marked \c owner passes any cleanup
    /// responsibility to the caller.
    template<typename T>
    using owner = T;

    inline namespace literals {
      inline namespace byte_literals {

        constexpr byte operator ""_byte( unsigned long long b ) noexcept;
        constexpr std::size_t operator ""_b( unsigned long long b ) noexcept;
        constexpr std::size_t operator ""_kb( unsigned long long b ) noexcept;
        constexpr std::size_t operator ""_mb( unsigned long long b ) noexcept;
        constexpr std::size_t operator ""_gb( unsigned long long b ) noexcept;

      } // inline namespace byte_literals
    } // inline namespace literals

    /// \brief Constructs an instance of type \p T with the given \p args
    ///        at the memory location specified in \p ptr
    ///
    /// \param ptr     The memory location to construct into
    /// \param args... The arguments to supply to T's constructor
    /// \return Pointer to the initialized memory (cast of \p ptr)
    template<typename T, typename...Args>
    T* uninitialized_construct_at( void* ptr, Args&&...args );

    template<typename T>
    T* uninitialized_construct_array_at( void* p, std::size_t n );

    template<typename T>
    void destroy_at( T* p );

    template<typename T>
    void destroy_array_at( T* p, std::size_t n );

    template<typename IntT>
    struct packed_integral_type
    {
      //----------------------------------------------------------------------
      // Public Member Types
      //----------------------------------------------------------------------
    public:

      using int_type  = IntT;
      using byte_type = unsigned char;

      //----------------------------------------------------------------------
      // Constructors / Assignment
      //----------------------------------------------------------------------
    public:

      packed_integral_type( int_type i );
      packed_integral_type( byte_type* b );

      packed_integral_type( const packed_integral_type& other ) noexcept;
      packed_integral_type( packed_integral_type&& other ) noexcept;

      //----------------------------------------------------------------------

      packed_integral_type& operator=( int_type i );
      packed_integral_type& operator=( byte_type* b );

      //----------------------------------------------------------------------
      // Observers
      //----------------------------------------------------------------------
    public:

      int_type as_int() noexcept;

      byte_type const* as_bytes() const noexcept;

      //----------------------------------------------------------------------
      // Private Members
      //----------------------------------------------------------------------
    private:

      union storage_type
      {
        byte_type  bytes[sizeof(IntT)];
        byte_type* data;
      };

      byte_type* m_storage;
      bool       m_is_internal;
    };

    using packed_uint8_t  = packed_integral_type<std::uint8_t>;
    using packed_uint16_t = packed_integral_type<std::uint16_t>;
    using packed_uint32_t = packed_integral_type<std::uint32_t>;
    using packed_uint64_t = packed_integral_type<std::uint64_t>;

    using packed_int8_t  = packed_integral_type<std::int8_t>;
    using packed_int16_t = packed_integral_type<std::int16_t>;
    using packed_int32_t = packed_integral_type<std::int32_t>;
    using packed_int64_t = packed_integral_type<std::int64_t>;

  } // namespace memory
} // namespace bit

#include "detail/memory.inl"

#endif /* BIT_MEMORY_MEMORY_HPP */
