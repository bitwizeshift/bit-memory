/**
 * \file offset_ptr.hpp
 *
 * \brief todo: fill in documentation
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */
#ifndef BIT_MEMORY_DETAIL_OFFSET_PTR_HPP
#define BIT_MEMORY_DETAIL_OFFSET_PTR_HPP

#include <cstdlib>     // std::ptrdiff_t
#include <cstdint>     // std::uintptr_t
#include <type_traits> // std::declval

namespace bit {
  namespace memory {

    //////////////////////////////////////////////////////////////////////////
    /// \brief
    ///
    ///
    /// \tparam T The underlying pointer element
    //////////////////////////////////////////////////////////////////////////
    template<typename T>
    class offset_ptr
    {
      //----------------------------------------------------------------------
      // Public Member Types
      //----------------------------------------------------------------------
    public:

      using element_type = T;  ///< The underlying element type
      using pointer      = T*; ///< The pointer type

      //----------------------------------------------------------------------
      // Constructors / Assignment
      //----------------------------------------------------------------------
    public:

      /// \brief
      offset_ptr() noexcept;

      offset_ptr( std::nullptr_t ) noexcept;

      offset_ptr( pointer p ) noexcept;

      template<typename U, typename = std::enable_if_t<std::is_convertible<T*,U*>::value>>
      offset_ptr( U* p ) noexcept;

      offset_ptr( const offset_ptr& other ) noexcept = default;
      offset_ptr( offset_ptr&& other ) noexcept = default;

      template<typename U, typename = std::enable_if_t<std::is_convertible<T*,U*>::value>>
      offset_ptr( const offset_ptr<U>& other ) noexcept;

      template<typename U, typename = std::enable_if_t<std::is_convertible<T*,U*>::value>>
      offset_ptr( offset_ptr<U>&& other ) noexcept;

      //----------------------------------------------------------------------

      offset_ptr& operator=( pointer p ) noexcept;

      template<typename U, typename = std::enable_if_t<std::is_convertible<T*,U*>::value>>
      offset_ptr& operator=( U* p ) noexcept;

      offset_ptr& operator=( const offset_ptr& other ) noexcept = default;
      offset_ptr& operator=( offset_ptr&& other ) noexcept = default;

      template<typename U, typename = std::enable_if_t<std::is_convertible<T*,U*>::value>>
      offset_ptr& operator=( const offset_ptr<U>& other ) noexcept;

      template<typename U, typename = std::enable_if_t<std::is_convertible<T*,U*>::value>>
      offset_ptr& operator=( offset_ptr<U>&& other ) noexcept;

      //----------------------------------------------------------------------
      // Modifiers
      //----------------------------------------------------------------------
    public:

      /// \brief Resets this fat_ptr to a nullptr
      void reset() noexcept;

      /// \brief Resets this fat_ptr to point to the new pointer \p p of size
      ///        \p n
      ///
      /// \param p the pointer
      /// \param n the size of the allocation
      template<typename U, typename = std::enable_if_t<std::is_convertible<T*,U*>::value>>
      void reset( U* p ) noexcept;

      /// \brief Swaps this fat_ptr with the \p other
      ///
      /// \param other the other fat_ptr to swap
      void swap( offset_ptr& other ) noexcept;

      //----------------------------------------------------------------------
      // Observers
      //----------------------------------------------------------------------
    public:

      /// \brief Gets the underlying pointer
      ///
      /// \return the underlying pointer
      element_type* get() const noexcept;

      /// \brief Dereferences the offset_ptr
      ///
      /// \note UB if the pointer is invalid
      ///
      /// \return reference to the pointed element
      element_type& operator*() const noexcept;

      /// \brief Dereferences the offset_ptr
      ///
      /// \note UB if the pointer is invalid
      ///
      /// \return pointer to the element
      element_type* operator->() const noexcept;

      /// \brief Convertible to bool
      ///
      /// \return \c true if this pointer is non-null
      explicit operator bool() const noexcept;

      //----------------------------------------------------------------------
      // Private Members
      //----------------------------------------------------------------------
    private:

      std::ptrdiff_t m_offset;

      template<typename> friend class offset_ptr;

      //----------------------------------------------------------------------
      // Private Utilities
      //----------------------------------------------------------------------
    private:

      template<typename U, typename V>
      static std::ptrdiff_t calculate_offset( U* lhs, V* rhs ) noexcept;

      T* calculate_address( std::true_type ) const noexcept;

      T* calculate_address( std::false_type ) const noexcept;
    };

    //------------------------------------------------------------------------
    // Comparisons
    //------------------------------------------------------------------------

    template<typename T, typename U>
    bool operator == ( const offset_ptr<T>& lhs, const offset_ptr<U>& rhs ) noexcept;

    template<typename T, typename U>
    bool operator != ( const offset_ptr<T>& lhs, const offset_ptr<U>& rhs ) noexcept;

    template<typename T, typename U>
    bool operator < ( const offset_ptr<T>& lhs, const offset_ptr<U>& rhs ) noexcept;

    template<typename T, typename U>
    bool operator > ( const offset_ptr<T>& lhs, const offset_ptr<U>& rhs ) noexcept;

    template<typename T, typename U>
    bool operator <= ( const offset_ptr<T>& lhs, const offset_ptr<U>& rhs ) noexcept;

    template<typename T, typename U>
    bool operator >= ( const offset_ptr<T>& lhs, const offset_ptr<U>& rhs ) noexcept;

    //------------------------------------------------------------------------
    // Utilities
    //------------------------------------------------------------------------

    /// \brief Swaps the instances of two offset_ptr
    ///
    /// \param lhs the left pointer to swap
    /// \param rhs the right pointer to swap
    template<typename T>
    void swap( offset_ptr<T>& lhs, offset_ptr<T>& rhs ) noexcept;

    inline namespace casts {

      /// \brief Performs a static_cast on the underlying fat_ptr type
      ///
      /// \param other the other pointer to cast
      /// \return the newly casted pointer
      template<typename To, typename From>
      offset_ptr<To> static_pointer_cast( const offset_ptr<From>& other ) noexcept;

      /// \brief Performs a dynamic_cast on the underlying fat_ptr type
      ///
      /// \param other the other pointer to cast
      /// \return the newly casted pointer
      template<typename To, typename From>
      offset_ptr<To> dynamic_pointer_cast( const offset_ptr<From>& other ) noexcept;

      /// \brief Performs a const_cast on the underlying fat_ptr type
      ///
      /// \param other the other pointer to cast
      /// \return the newly casted pointer
      template<typename To, typename From>
      offset_ptr<To> const_pointer_cast( const offset_ptr<From>& other ) noexcept;

      /// \brief Performs a reinterpret_cast on the underlying fat_ptr type
      ///
      /// \param other the other pointer to cast
      /// \return the newly casted pointer
      template<typename To, typename From>
      offset_ptr<To> reinterpret_pointer_cast( const offset_ptr<From>& other ) noexcept;

    } // inline namespace casts

  } // namespace memory
} // namespace bit

#include "offset_ptr.inl"

#endif // BIT_MEMORY_DETAIL_OFFSET_PTR_HPP
