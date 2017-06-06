/**
 * \file fat_ptr.hpp
 *
 * \brief todo: fill in documentation
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */
#ifndef BIT_MEMORY_DETAIL_SMART_PTR_FAT_PTR_HPP
#define BIT_MEMORY_DETAIL_SMART_PTR_FAT_PTR_HPP

#include <cstdlib> // std::size_t
#include <utility> // std::swap

namespace bit {
  namespace memory {

    //========================================================================
    // fat_ptr<T>
    //========================================================================

    //////////////////////////////////////////////////////////////////////////
    /// \brief A fat pointer that contains the pointer and the size
    ///
    /// This type primarily exists for memory returned from allocators to
    /// simplify record-keeping of allocated memory.
    ///
    /// \tparam T The underlying pointer type
    //////////////////////////////////////////////////////////////////////////
    template<typename T>
    class fat_ptr
    {
      //----------------------------------------------------------------------
      // Public Member Types
      //----------------------------------------------------------------------
    public:

      using element_type = T; ///< The underlying element type

      //----------------------------------------------------------------------
      // Constructors / Assignment
      //----------------------------------------------------------------------
    public:

      /// \brief Constructs a fat_ptr at the specified memory address of size
      ///        n
      ///
      /// \param p the pointer to contain
      /// \param n the size of the memory being pointed to
      constexpr explicit fat_ptr( T* p, std::size_t n ) noexcept;

      /// \brief Copy-constructs a fat_ptr
      ///
      /// \param other the other pointer to copy
      constexpr fat_ptr( const fat_ptr& other ) noexcept = default;

      /// \brief Move-constructs a fat_ptr
      ///
      /// \param other the other pointer to move
      constexpr fat_ptr( fat_ptr&& other ) noexcept = default;

      /// \brief Copy-converts a fat_ptr
      ///
      /// \param other the other pointer to copy convert
      template<typename U>
      constexpr fat_ptr( const fat_ptr<U>& other ) noexcept;

      /// \brief Move-converts a fat_ptr
      ///
      /// \param other the other pointer to move convert
      template<typename U>
      constexpr fat_ptr( fat_ptr<U>&& other ) noexcept;

      //----------------------------------------------------------------------

      /// \brief Copy-assigns a fat_ptr
      ///
      /// \param other the other fat_ptr to copy
      /// \return reference to \c (*this)
      fat_ptr& operator=( const fat_ptr& other ) noexcept = default;

      /// \brief Move-assigns a fat_ptr
      ///
      /// \param other the other fat_ptr to move
      /// \return reference to \c (*this)
      fat_ptr& operator=( fat_ptr&& other ) noexcept = default;

      /// \brief Copy-converts a fat_ptr
      ///
      /// \param other the other fat_ptr to copy convert
      /// \return reference to \c (*this)
      template<typename U>
      fat_ptr& operator=( const fat_ptr<U>& other ) noexcept;

      /// \brief Move-converts a fat_ptr
      ///
      /// \param other the other fat_ptr to move convert
      /// \return reference to \c (*this)
      template<typename U>
      fat_ptr& operator=( fat_ptr<U>&& other ) noexcept;

      //----------------------------------------------------------------------
      // Modifiers
      //----------------------------------------------------------------------
    public:

      /// \brief Resets this fat_ptr to a nullptr
      constexpr void reset() noexcept;

      /// \brief Resets this fat_ptr to point to the new pointer \p p of size
      ///        \p n
      ///
      /// \param p the pointer
      /// \param n the size of the allocation
      template<typename U>
      constexpr void reset( U* p, std::size_t n ) noexcept;

      /// \brief Swaps this fat_ptr with the \p other
      ///
      /// \param other the other fat_ptr to swap
      constexpr void swap( fat_ptr& other ) noexcept;

      //----------------------------------------------------------------------
      // Observers
      //----------------------------------------------------------------------
    public:

      /// \brief Gets the underlying pointer
      ///
      /// \return the underlying pointer
      constexpr element_type* get() const noexcept;

      /// \brief Gets the size of the memory pointed to
      ///
      /// \return the size of the memory pointed to
      constexpr std::size_t size() const noexcept;

      /// \brief Dereferences the fat_ptr
      ///
      /// \note UB if the pointer is invalid
      ///
      /// \return reference to the pointed element
      constexpr element_type& operator*() const noexcept;

      /// \brief Dereferences the fat_ptr
      ///
      /// \note UB if the pointer is invalid
      ///
      /// \return pointer to the element
      constexpr element_type* operator->() const noexcept;

      /// \brief Convertible to bool
      ///
      /// \return \c true if this pointer is non-null
      constexpr explicit operator bool() const noexcept;

      //----------------------------------------------------------------------
      // Private Members
      //----------------------------------------------------------------------
    private:

      T*          m_ptr;  ///< The underlying pointer
      std::size_t m_size; ///< The size of the memory pointed to
    };

    //========================================================================
    // fat_ptr<T[]>
    //========================================================================

    template<typename T>
    class fat_ptr<T[]>
    {
      static_assert( !std::is_void<T>::value, "T cannot be a void" );
      static_assert( !std::is_abstract<T>::value, "T cannot be an abstract type" );

      //----------------------------------------------------------------------
      // Public Member Types
      //----------------------------------------------------------------------
    public:

      using element_type = T; ///< The underlying element type

      //----------------------------------------------------------------------
      // Constructors / Assignment
      //----------------------------------------------------------------------
    public:

      /// \brief Constructs a fat_ptr at the specified memory address of size
      ///        n
      ///
      /// \param p the pointer to contain
      /// \param n the size of the memory being pointed to
      constexpr explicit fat_ptr( T* p, std::size_t n ) noexcept;

      /// \brief Copy-constructs a fat_ptr
      ///
      /// \param other the other pointer to copy
      constexpr fat_ptr( const fat_ptr& other ) noexcept = default;

      /// \brief Move-constructs a fat_ptr
      ///
      /// \param other the other pointer to move
      constexpr fat_ptr( fat_ptr&& other ) noexcept = default;

      //----------------------------------------------------------------------

      /// \brief Copy-assigns a fat_ptr
      ///
      /// \param other the other fat_ptr to copy
      /// \return reference to \c (*this)
      fat_ptr& operator=( const fat_ptr& other ) noexcept = default;

      /// \brief Move-assigns a fat_ptr
      ///
      /// \param other the other fat_ptr to move
      /// \return reference to \c (*this)
      fat_ptr& operator=( fat_ptr&& other ) noexcept = default;

      //----------------------------------------------------------------------
      // Modifiers
      //----------------------------------------------------------------------
    public:

      /// \brief Resets this fat_ptr to a nullptr
      constexpr void reset() noexcept;

      /// \brief Resets this fat_ptr to point to the new pointer \p p of size
      ///        \p n
      ///
      /// \param p the pointer
      /// \param n the size of the allocation
      constexpr void reset( T* p, std::size_t n ) noexcept;

      /// \brief Swaps this fat_ptr with the \p other
      ///
      /// \param other the other fat_ptr to swap
      constexpr void swap( fat_ptr& other ) noexcept;

      //----------------------------------------------------------------------
      // Observers
      //----------------------------------------------------------------------
    public:

      /// \brief Gets the underlying pointer
      ///
      /// \return the underlying pointer
      constexpr element_type* get() const noexcept;

      /// \brief Gets the size of the memory pointed to
      ///
      /// \return the size of the memory pointed to
      constexpr std::size_t size() const noexcept;

      /// \brief Accesses the element at the offset from the current pointer
      ///
      /// \param n the index to access
      /// \return reference to the entry
      constexpr element_type& operator[]( std::ptrdiff_t n ) const noexcept;

      /// \brief Convertible to bool
      ///
      /// \return \c true if this pointer is non-null
      constexpr explicit operator bool() const noexcept;

      //----------------------------------------------------------------------
      // Private Members
      //----------------------------------------------------------------------
    private:

      T*          m_ptr;  ///< The underlying pointer
      std::size_t m_size; ///< The size of the memory pointed to
    };

    //------------------------------------------------------------------------
    // Utilities
    //------------------------------------------------------------------------

    /// \brief Swaps the instances of two fat_ptrs
    ///
    /// \param lhs the left pointer to swap
    /// \param rhs the right pointer to swap
    template<typename T>
    void swap( fat_ptr<T>& lhs, fat_ptr<T>& rhs ) noexcept;

    inline namespace casts {

      /// \brief Performs a static_cast on the underlying fat_ptr type
      ///
      /// \param other the other pointer to cast
      /// \return the newly casted pointer
      template<typename To, typename From>
      constexpr fat_ptr<To> static_pointer_cast( const fat_ptr<From>& other ) noexcept;

      /// \brief Performs a dynamic_cast on the underlying fat_ptr type
      ///
      /// \param other the other pointer to cast
      /// \return the newly casted pointer
      template<typename To, typename From>
      constexpr fat_ptr<To> dynamic_pointer_cast( const fat_ptr<From>& other ) noexcept;

      /// \brief Performs a const_cast on the underlying fat_ptr type
      ///
      /// \param other the other pointer to cast
      /// \return the newly casted pointer
      template<typename To, typename From>
      constexpr fat_ptr<To> const_pointer_cast( const fat_ptr<From>& other ) noexcept;

      /// \brief Performs a reinterpret_cast on the underlying fat_ptr type
      ///
      /// \param other the other pointer to cast
      /// \return the newly casted pointer
      template<typename To, typename From>
      constexpr fat_ptr<To> reinterpret_pointer_cast( const fat_ptr<From>& other ) noexcept;

    } // inline namespace
  } // namespace memory
} // namespace bit

#include "fat_ptr.inl"

#endif // BIT_MEMORY_DETAIL_SMART_PTR_FAT_PTR_HPP
