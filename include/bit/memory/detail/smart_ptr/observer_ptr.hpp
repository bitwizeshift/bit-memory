/**
 * \file observer_ptr.hpp
 *
 * \brief todo: fill in documentation
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */
#ifndef BIT_MEMORY_DETAIL_SMART_PTR_OBSERVER_PTR_HPP
#define BIT_MEMORY_DETAIL_SMART_PTR_OBSERVER_PTR_HPP

namespace bit {
  namespace memory {
    namespace detail {

      template<typename T>
      struct is_ptr_observable
      {
        template<typename U>
        static auto test( U* t )
          -> decltype(std::declval<const void*>() = t->get(), std::true_type{});

        static auto test(...) -> std::false_type;

        static constexpr bool value = decltype(test( std::declval<T*>() ))::value;
      };

      template<typename T>
      using make_observer_element_type_t
        = std::remove_pointer_t<std::decay_t<decltype(std::declval<T>().get())>>;

    } // namespace detail

    //////////////////////////////////////////////////////////////////////////
    /// \brief A lightweight wrapper around a pointer type
    ///
    /// \tparam the underlying pointer type
    //////////////////////////////////////////////////////////////////////////
    template<typename T>
    class observer_ptr
    {
      //----------------------------------------------------------------------
      // Public Member Types
      //----------------------------------------------------------------------
    public:

      using pointer      = T*; ///<
      using element_type = T;  ///<

      //----------------------------------------------------------------------
      // Constructors / Assignment
      //----------------------------------------------------------------------
    public:

      /// \brief Default-constructs an observer_ptr to \c nullptr
      constexpr observer_ptr() noexcept;

      /// \brief Constructs an observer_ptr pointing to \c nullptr
      constexpr observer_ptr( std::nullptr_t ) noexcept;

      /// \brief Constructs an observer_ptr that points to \p ptr
      ///
      /// \param ptr the pointer value
      constexpr explicit observer_ptr( pointer ptr ) noexcept;

      /// \brief Copy-converts an observer_ptr from another one that points
      ///        to U
      ///
      /// \param other the other pointer to copy convert
      template<typename U, std::enable_if<std::is_convertible<U*,T*>::value>* = nullptr>
      constexpr observer_ptr( const observer_ptr<U>& other ) noexcept;

      /// \brief Move-converts an observer_ptr from another one that points
      ///        to U
      ///
      /// \param other the other pointer to move convert
      template<typename U, std::enable_if<std::is_convertible<U*,T*>::value>* = nullptr>
      observer_ptr( observer_ptr<U>&& other ) noexcept;

      /// \brief Copy-constructs an observer_ptr from another one
      ///
      /// \param other the other observer_ptr to copy
      constexpr observer_ptr( const observer_ptr& other ) noexcept = default;

      /// \brief Move-constructs an observer_ptr from another one
      ///
      /// \param other the other observer_ptr to move
      constexpr observer_ptr( observer_ptr&& other ) noexcept = default;

      //----------------------------------------------------------------------

      /// \brief Assigns this observer_ptr to \c nullptr
      ///
      /// \return reference to \c (*this)
      observer_ptr& operator=( std::nullptr_t ) noexcept;

      /// \brief Copy-assigns an observer_ptr from another one
      ///
      /// \param other the other observer_ptr to copy
      /// \return reference to \c (*this)
      observer_ptr& operator=( const observer_ptr& other ) noexcept = default;

      /// \brief Move-assigns an observer_ptr from another one
      ///
      /// \param other the other observer_ptr to move
      /// \return reference to \c (*this)
      observer_ptr& operator=( observer_ptr&& other ) noexcept = default;

      //----------------------------------------------------------------------
      // Modifiers
      //----------------------------------------------------------------------
    public:

      /// \brief Resets this observer_ptr to point to \p ptr
      ///
      /// \param ptr the pointer to reset to
      void reset( pointer ptr = pointer() ) noexcept;

      /// \brief Resets this observer_ptr to \c nullptr
      void reset( std::nullptr_t ) noexcept;

      /// \brief Swaps this observer_ptr with another observer_ptr
      ///
      /// \param other the other observer_ptr
      void swap( observer_ptr& other ) noexcept;

      //----------------------------------------------------------------------
      // Observers
      //----------------------------------------------------------------------
    public:

      /// \brief Gets the underlying pointer
      ///
      /// \return the underlying pointer
      constexpr pointer get() const noexcept;

      /// \brief Explicitly convertible to bool. This is \c true when the
      ///        underlying pointer is non-null
      constexpr explicit operator bool() const noexcept;

      /// \brief Dereferences the observer_ptr
      ///
      /// \return the underlying pointer
      constexpr pointer operator->() const noexcept;

      /// \brief Dereferences the observer_ptr
      ///
      /// \return the underlying reference
      constexpr std::add_lvalue_reference_t<T> operator*() const noexcept;

      //----------------------------------------------------------------------
      // Conversions
      //----------------------------------------------------------------------
    public:

      /// \brief Explicitly convertible to pointer type
      constexpr explicit operator element_type*() const noexcept;

      //----------------------------------------------------------------------
      // Private Members
      //----------------------------------------------------------------------
    private:

      T* m_ptr; ///< The underlying pointer
    };

    //------------------------------------------------------------------------
    // Free Functions
    //------------------------------------------------------------------------

    /// \brief Swaps the observer_ptr \p lhs with \p rhs
    ///
    /// \param lhs the left one to swap
    /// \param rhs the right one to swap
    template<typename T>
    void swap( observer_ptr<T>& lhs, observer_ptr<T>& rhs ) noexcept;

    /// \brief Makes an observer_ptr from a raw pointer
    ///
    /// \param ptr the pointer
    /// \return an observer_ptr
    template<typename T>
    constexpr observer_ptr<T> make_observer( T* ptr ) noexcept;

    /// \brief Makes an observer_ptr from a raw pointer, and coerces it to
    ///        the specified type
    ///
    /// \tparam T the type to convert to
    /// \param ptr the pointer
    /// \return an observer_ptr
    template<typename T, typename U, std::enable_if_t<!std::is_same<T,U>::value && std::is_convertible<U*,T*>::value>* = nullptr>
    constexpr observer_ptr<T> make_observer( U* ptr ) noexcept;

    /// \brief Makes an observer_ptr from a smart pointer
    ///
    /// \param ptr the pointer
    /// \return an observer_ptr
    template<typename Pointer>
    constexpr auto make_observer( const Pointer& ptr ) noexcept
      -> decltype( make_observer( ptr.get() ) );

    /// \brief Makes an observer_ptr from a smart pointer
    ///
    /// \tparam T explicit type of the pointer
    /// \param ptr the pointer
    /// \return an observer_ptr
    template<typename T, typename Pointer, std::enable_if<detail::is_ptr_observable<Pointer>::value>* = nullptr>
    constexpr observer_ptr<T> make_observer( const Pointer& ptr ) noexcept;

    //------------------------------------------------------------------------
    // Comparisons
    //------------------------------------------------------------------------

    template<typename T, typename U>
    constexpr bool operator==( const observer_ptr<T>& lhs,
                               const observer_ptr<U>& rhs ) noexcept;
    template<typename T, typename U>
    constexpr bool operator!=( const observer_ptr<T>& lhs,
                               const observer_ptr<U>& rhs ) noexcept;
    template<typename T, typename U>
    constexpr bool operator<( const observer_ptr<T>& lhs,
                              const observer_ptr<U>& rhs ) noexcept;
    template<typename T, typename U>
    constexpr bool operator>( const observer_ptr<T>& lhs,
                              const observer_ptr<U>& rhs ) noexcept;
    template<typename T, typename U>
    constexpr bool operator<=( const observer_ptr<T>& lhs,
                               const observer_ptr<U>& rhs ) noexcept;
    template<typename T, typename U>
    constexpr bool operator>=( const observer_ptr<T>& lhs,
                               const observer_ptr<U>& rhs ) noexcept;

    //------------------------------------------------------------------------

    template<typename T>
    constexpr bool operator==( std::nullptr_t,
                               const observer_ptr<T>& rhs ) noexcept;
    template<typename T>
    constexpr bool operator==( const observer_ptr<T>& lhs,
                               std::nullptr_t ) noexcept;

    template<typename T>
    constexpr bool operator!=( std::nullptr_t,
                               const observer_ptr<T>& rhs ) noexcept;
    template<typename T>
    constexpr bool operator!=( const observer_ptr<T>& lhs,
                               std::nullptr_t ) noexcept;

    template<typename T>
    constexpr bool operator<( std::nullptr_t,
                              const observer_ptr<T>& rhs ) noexcept;
    template<typename T>
    constexpr bool operator<( const observer_ptr<T>& lhs,
                              std::nullptr_t ) noexcept;

    template<typename T>
    constexpr bool operator>( std::nullptr_t,
                              const observer_ptr<T>& rhs ) noexcept;
    template<typename T>
    constexpr bool operator>( const observer_ptr<T>& lhs,
                              std::nullptr_t ) noexcept;

    template<typename T>
    constexpr bool operator<=( std::nullptr_t,
                               const observer_ptr<T>& rhs ) noexcept;
    template<typename T>
    constexpr bool operator<=( const observer_ptr<T>& lhs,
                               std::nullptr_t ) noexcept;

    template<typename T>
    constexpr bool operator>=( std::nullptr_t,
                               const observer_ptr<T>& rhs ) noexcept;
    template<typename T>
    constexpr bool operator>=( const observer_ptr<T>& lhs,
                               std::nullptr_t ) noexcept;

  } // namespace memory
} // namespace bit

#include "observer_ptr.inl"

#endif /* BIT_MEMORY_DETAIL_SMART_PTR_OBSERVER_PTR_HPP */
