/**
 * \file dynamic_size_type.hpp
 *
 * \brief This internal header contains an EBO-optimized type for storing
 *        possibly compile-time size values.
 *
 * \note This is an internal header file, included by other library headers.
 *       Do not attempt to use it directly.
 */
#ifndef BIT_MEMORY_UTILITIES_DYNAMIC_SIZE_TYPE_HPP
#define BIT_MEMORY_UTILITIES_DYNAMIC_SIZE_TYPE_HPP

#include <cstddef>     // std::size_t
#include <type_traits> // std::true_type, std::false_type

namespace bit {
  namespace memory {

    /// \brief A size used to indicate a dynamic value processed at runtime,
    ///        rather than a static value known at compile-time
    constexpr std::size_t dynamic_size = std::size_t(-1);

    ///////////////////////////////////////////////////////////////////////////
    /// \brief This type is used as a compile-time constant for storing
    ///        size values
    ///
    /// If \p Size is dynamic_size, then this type holds a data member.
    ///
    /// \tparam Index the index of the value (used for unique identification
    ///               in ebo)
    /// \tparam Size the size value
    ///////////////////////////////////////////////////////////////////////////
    template<std::size_t Index, std::size_t Size>
    class dynamic_size_type
    {
      //-----------------------------------------------------------------------
      // Constructors / Assignment
      //-----------------------------------------------------------------------
    public:

      /// \brief Default-constructs the dynamic_size_type
      constexpr dynamic_size_type() noexcept = default;

      /// \brief Move-constructs a dynamic_size_type from another one
      ///
      /// \param other the other dynamic_size_type to move
      dynamic_size_type( dynamic_size_type&& other ) noexcept = default;

      /// \brief Copy-constructs a dynamic_size_type from another one
      ///
      /// \param other the other dynamic_size_type to copy
      dynamic_size_type( const dynamic_size_type& other ) noexcept = default;

      //-----------------------------------------------------------------------

      /// \brief Move-assigns a dynamic_size_type from another one
      ///
      /// \param other the other dynamic_size_type to move
      /// \return reference to \c (*this)
      dynamic_size_type& operator=( dynamic_size_type&& other ) noexcept = default;

      /// \brief Copy-assigns a dynamic_size_type from another one
      ///
      /// \param other the other dynamic_size_type to copy
      /// \return reference to \c (*this)
      dynamic_size_type& operator=( const dynamic_size_type& other ) noexcept = default;

      //-----------------------------------------------------------------------
      // Observers
      //-----------------------------------------------------------------------
    public:

      /// \brief Retrieves the value
      ///
      /// \return Size
      constexpr std::size_t value() const noexcept;
    };

    //-------------------------------------------------------------------------

    template<std::size_t Index>
    class dynamic_size_type<Index,dynamic_size>
    {
      //-----------------------------------------------------------------------
      // Constructors / Assignment
      //-----------------------------------------------------------------------
    public:

      /// \brief Constructs a dynamic_size_type from a given \p size
      ///
      /// \param size the size to store
      constexpr explicit dynamic_size_type( std::size_t size ) noexcept;

      /// \brief Move-constructs a dynamic_size_type from another one
      ///
      /// \param other the other dynamic_size_type to move
      dynamic_size_type( dynamic_size_type&& other ) noexcept = default;

      /// \brief Copy-constructs a dynamic_size_type from another one
      ///
      /// \param other the other dynamic_size_type to copy
      dynamic_size_type( const dynamic_size_type& other ) noexcept = default;

      //-----------------------------------------------------------------------

      /// \brief Move-assigns a dynamic_size_type from another one
      ///
      /// \param other the other dynamic_size_type to move
      /// \return reference to \c (*this)
      dynamic_size_type& operator=( dynamic_size_type&& other ) noexcept = default;

      /// \brief Copy-assigns a dynamic_size_type from another one
      ///
      /// \param other the other dynamic_size_type to copy
      /// \return reference to \c (*this)
      dynamic_size_type& operator=( const dynamic_size_type& other ) noexcept = default;

      //-----------------------------------------------------------------------
      // Observers
      //-----------------------------------------------------------------------
    public:

      /// \brief Retrieves the stored size
      ///
      /// \return the stored size
      constexpr std::size_t value() const noexcept;

      //-----------------------------------------------------------------------
      // Private Members
      //-----------------------------------------------------------------------
    private:

      std::size_t m_size;
    };

  } // namespace memory
} // namespace bit

#include "detail/dynamic_size_type.inl"

#endif /* BIT_MEMORY_UTILITIES_DYNAMIC_SIZE_TYPE_HPP */
