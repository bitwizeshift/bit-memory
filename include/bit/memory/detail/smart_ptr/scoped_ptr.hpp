/**
 * \file scoped_ptr.hpp
 *
 * \brief todo: fill in documentation
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */

#ifndef BIT_MEMORY_DETAIL_SMART_PTR_SCOPED_PTR_HPP
#define BIT_MEMORY_DETAIL_SMART_PTR_SCOPED_PTR_HPP

#include <memory>

namespace bit {
  namespace memory {
    namespace detail {

      template<typename...Ts>
      struct scoped_ptr_voidify{ using type = void; };

      template<typename...Ts>
      using scoped_ptr_void_t = typename scoped_ptr_voidify<Ts...>::type;

      template<typename T, typename Deleter, typename = void>
      struct scoped_ptr_pointer{ using type = T*; };

      template<typename T, typename Deleter>
      struct scoped_ptr_pointer<T,Deleter,scoped_ptr_void_t<typename std::decay_t<Deleter>::type> >
      {
        using type = typename std::decay_t<Deleter>::type;
      };

      template<typename T> struct make_scoped_dispatch;
    }

    //////////////////////////////////////////////////////////////////////////
    /// \brief A pointer that lives as long as the current scope
    ///
    /// Semantically, this is the same as constructing a const unique_ptr.
    //////////////////////////////////////////////////////////////////////////
    template<typename T, typename Deleter = std::default_delete<T>>
    class scoped_ptr
    {
      //----------------------------------------------------------------------
      // Public Member Types
      //----------------------------------------------------------------------
    public:

      using pointer      = typename detail::scoped_ptr_pointer<T,Deleter>::type;
      using element_type = T;
      using deleter_type = Deleter;

      //----------------------------------------------------------------------
      // Constructors / Destructor / Assignment
      //----------------------------------------------------------------------
    public:

      /// \brief Default constructs a scoped_ptr that points to null
      constexpr scoped_ptr() noexcept;

      /// \brief Constructs a scoped_ptr from a nullptr
      constexpr scoped_ptr( std::nullptr_t ) noexcept;

      /// \brief Constructs a scoped_ptr that points to newly allocated memory
      ///        to manage
      ///
      /// \param ptr pointer ot the memory to manage
      constexpr explicit scoped_ptr( pointer ptr ) noexcept;

      /// \brief Constructs a scoped_ptr that points t newly allocated memory
      ///        to manage that will be deleted by \p deleter
      ///
      /// \param ptr pointer to the memory to manage
      /// \param deleter the deleter object
      constexpr scoped_ptr( pointer ptr, const Deleter& deleter ) noexcept;

      /// \copydoc scoped_ptr( pointer, const Deleter& );
      constexpr scoped_ptr( pointer ptr, Deleter&& deleter ) noexcept;

      /// Deleted copy constructor
      scoped_ptr( const scoped_ptr& ) = delete;

      ~scoped_ptr();

      /// \brief Resets a scoped_ptr
      scoped_ptr& operator=( std::nullptr_t );

      /// Deleted copy assignment
      scoped_ptr& operator=( const scoped_ptr& ) = delete;

      /// Deleted move assignment
      scoped_ptr& operator=( scoped_ptr&& ) = delete;

      //----------------------------------------------------------------------
      // Modifiers
      //----------------------------------------------------------------------
    public:

      /// \brief Resets this scoped_ptr to point to \p ptr
      ///
      /// \param ptr the pointer to reset to
      void reset( pointer ptr = pointer() );

      /// \brief Resets this scoped_ptr to \c nullptr
      void reset( std::nullptr_t );

      /// \brief Swaps this scoped_ptr with another scoped_ptr
      ///
      /// \param other the other scoped_ptr
      void swap( scoped_ptr& other ) noexcept;

      /// \brief Releases the underlying pointer from being managed by this
      ///        scoped_ptr
      ///
      /// \return the underlying pointer
      pointer release() noexcept;

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

      /// \brief Dereferences the scoped_ptr
      ///
      /// \return the underlying pointer
      constexpr element_type* operator->() const noexcept;

      /// \brief Dereferences the scoped_ptr
      ///
      /// \return the underlying reference
      constexpr element_type& operator*() const noexcept;

      /// \brief Gets the underlying deleter
      ///
      /// \return the deleter
      Deleter& get_deleter() noexcept;

      /// \copydoc get_deleter()
      const Deleter& get_deleter() const noexcept;

      //----------------------------------------------------------------------
      // Private Member Types
      //----------------------------------------------------------------------
    private:

      struct storage_type : Deleter
      {
        constexpr storage_type( pointer ptr ) : ptr(ptr){}
        constexpr storage_type( pointer ptr, const Deleter& deleter ) : Deleter(deleter), ptr(ptr){};
        constexpr storage_type( pointer ptr, Deleter&& deleter ) : Deleter(deleter), ptr(ptr){};
        pointer ptr;
      };

      //----------------------------------------------------------------------
      // Private Members
      //----------------------------------------------------------------------
    private:

      storage_type m_storage;

      //----------------------------------------------------------------------
      // Private Constructors
      //----------------------------------------------------------------------
    private:

      /// \brief Private move constructor for make_scoped
      ///
      /// \param other the other scoped_ptr to move
      template<typename U>
      scoped_ptr( scoped_ptr<U,Deleter>&& other ) noexcept;

      //----------------------------------------------------------------------
      // Friends
      //----------------------------------------------------------------------
    private:

      template<typename U, typename...Args>
      friend scoped_ptr<U> make_scoped( Args&&... );

      template<typename U>
      friend struct detail::make_scoped_dispatch;
    };

    //------------------------------------------------------------------------
    // T[] Specialization
    //------------------------------------------------------------------------

    template<typename T, typename Deleter>
    class scoped_ptr<T[],Deleter>
    {
      //----------------------------------------------------------------------
      // Public Member Types
      //----------------------------------------------------------------------
    public:

      using pointer      = typename detail::scoped_ptr_pointer<T,Deleter>::type;
      using element_type = T;
      using deleter_type = Deleter;

      //----------------------------------------------------------------------
      // Constructors / Destructor / Assignment
      //----------------------------------------------------------------------
    public:

      /// \brief Default constructs a scoped_ptr that points to null
      constexpr scoped_ptr() noexcept;

      /// \brief Constructs a scoped_ptr from a nullptr
      constexpr scoped_ptr( std::nullptr_t ) noexcept;

      /// \brief Constructs a scoped_ptr that points to newly allocated memory
      ///        to manage
      ///
      /// \param ptr pointer ot the memory to manage
      constexpr explicit scoped_ptr( pointer ptr ) noexcept;

      /// \brief Constructs a scoped_ptr that points t newly allocated memory
      ///        to manage that will be deleted by \p deleter
      ///
      /// \param ptr pointer to the memory to manage
      /// \param deleter the deleter object
      constexpr scoped_ptr( pointer ptr, const Deleter& deleter ) noexcept;

      /// \copydoc scoped_ptr( pointer, const Deleter& );
      constexpr scoped_ptr( pointer ptr, Deleter&& deleter ) noexcept;

      /// Deleted copy constructor
      scoped_ptr( const scoped_ptr& ) = delete;

      ~scoped_ptr();

      /// \brief Resets a scoped_ptr
      scoped_ptr& operator=( std::nullptr_t );

      /// Deleted copy assignment
      scoped_ptr& operator=( const scoped_ptr& ) = delete;

      /// Deleted move assignment
      scoped_ptr& operator=( scoped_ptr&& ) = delete;

      //----------------------------------------------------------------------
      // Modifiers
      //----------------------------------------------------------------------
    public:

      /// \brief Resets this scoped_ptr to point to \p ptr
      ///
      /// \param ptr the pointer to reset to
      void reset( pointer ptr = pointer() );

      /// \brief Resets this scoped_ptr to \c nullptr
      void reset( std::nullptr_t );

      /// \brief Swaps this scoped_ptr with another scoped_ptr
      ///
      /// \param other the other scoped_ptr
      void swap( scoped_ptr& other ) noexcept;

      /// \brief Releases the underlying pointer from being managed by this
      ///        scoped_ptr
      ///
      /// \return the underlying pointer
      pointer release() noexcept;

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

      /// \brief Dereferences the scoped_ptr at the specified index
      ///
      /// \param n the index to access
      /// \return the reference to the element
      constexpr element_type& operator[]( std::ptrdiff_t n ) const noexcept;

      /// \brief Gets the underlying deleter
      ///
      /// \return the deleter
      Deleter& get_deleter() noexcept;

      /// \copydoc get_deleter()
      const Deleter& get_deleter() const noexcept;

      //----------------------------------------------------------------------
      // Private Member Types
      //----------------------------------------------------------------------
    private:

      struct storage_type : Deleter
      {
        constexpr storage_type( pointer ptr ) : ptr(ptr){}
        constexpr storage_type( pointer ptr, const Deleter& deleter ) : Deleter(deleter), ptr(ptr){};
        constexpr storage_type( pointer ptr, Deleter&& deleter ) : Deleter(deleter), ptr(ptr){};
        pointer ptr;
      };

      //----------------------------------------------------------------------
      // Private Members
      //----------------------------------------------------------------------
    private:

      storage_type m_storage;

      //----------------------------------------------------------------------
      // Private Constructors
      //----------------------------------------------------------------------
    private:

      /// \brief Private move constructor for make_scoped
      ///
      /// \remark this private constructor exists to allow for make_scoped to
      ///         work
      ///
      /// \param other the other scoped_ptr to move
      scoped_ptr( scoped_ptr&& other ) noexcept;

      //----------------------------------------------------------------------
      // Friends
      //----------------------------------------------------------------------
    private:

      template<typename U,typename...Args>
      friend scoped_ptr<U> make_scoped( Args&&... );

      template<typename U>
      friend struct detail::make_scoped_dispatch;
    };

    //------------------------------------------------------------------------
    // Utilities
    //------------------------------------------------------------------------

    /// \brief Utility to make a scope_ptr, forwarding \p args to \p T's
    ///        constructor
    ///
    /// \param args the arguments
    /// \return the scoped_ptr
    template<typename T, typename...Args>
    scoped_ptr<T> make_scoped( Args&&...args );

    template<typename T, typename Deleter>
    void swap( scoped_ptr<T,Deleter>& lhs, scoped_ptr<T,Deleter>& rhs ) noexcept;

//    template<typename T, typename Allocator, typename...Args>
//    allocated_scoped_ptr<T> allocate_scoped( Allocator&& allocator, Args&&...args );

//    template<typename T, typename Allocator>
//    allocated_scoped_ptr<T[]> allocate_scoped( Allocator&& allocator, std::size_t size );

    //------------------------------------------------------------------------
    // Comparisons
    //------------------------------------------------------------------------

    template<typename T, typename U>
    constexpr bool operator==( const scoped_ptr<T>& lhs,
                               const scoped_ptr<U>& rhs ) noexcept;
    template<typename T, typename U>
    constexpr bool operator!=( const scoped_ptr<T>& lhs,
                               const scoped_ptr<U>& rhs ) noexcept;
    template<typename T, typename U>
    constexpr bool operator<( const scoped_ptr<T>& lhs,
                              const scoped_ptr<U>& rhs ) noexcept;
    template<typename T, typename U>
    constexpr bool operator>( const scoped_ptr<T>& lhs,
                              const scoped_ptr<U>& rhs ) noexcept;
    template<typename T, typename U>
    constexpr bool operator<=( const scoped_ptr<T>& lhs,
                               const scoped_ptr<U>& rhs ) noexcept;
    template<typename T, typename U>
    constexpr bool operator>=( const scoped_ptr<T>& lhs,
                               const scoped_ptr<U>& rhs ) noexcept;

    //------------------------------------------------------------------------

    template<typename T>
    constexpr bool operator==( std::nullptr_t,
                               const scoped_ptr<T>& rhs ) noexcept;
    template<typename T>
    constexpr bool operator==( const scoped_ptr<T>& lhs,
                               std::nullptr_t ) noexcept;

    template<typename T>
    constexpr bool operator!=( std::nullptr_t,
                               const scoped_ptr<T>& rhs ) noexcept;
    template<typename T>
    constexpr bool operator!=( const scoped_ptr<T>& lhs,
                               std::nullptr_t ) noexcept;

    template<typename T>
    constexpr bool operator<( std::nullptr_t,
                              const scoped_ptr<T>& rhs ) noexcept;
    template<typename T>
    constexpr bool operator<( const scoped_ptr<T>& lhs,
                              std::nullptr_t ) noexcept;

    template<typename T>
    constexpr bool operator>( std::nullptr_t,
                              const scoped_ptr<T>& rhs ) noexcept;
    template<typename T>
    constexpr bool operator>( const scoped_ptr<T>& lhs,
                              std::nullptr_t ) noexcept;

    template<typename T>
    constexpr bool operator<=( std::nullptr_t,
                               const scoped_ptr<T>& rhs ) noexcept;
    template<typename T>
    constexpr bool operator<=( const scoped_ptr<T>& lhs,
                               std::nullptr_t ) noexcept;

    template<typename T>
    constexpr bool operator>=( std::nullptr_t,
                               const scoped_ptr<T>& rhs ) noexcept;
    template<typename T>
    constexpr bool operator>=( const scoped_ptr<T>& lhs,
                               std::nullptr_t ) noexcept;

  } // namespace memory
} // namespace bit

#include "scoped_ptr.inl"

#endif /* BIT_MEMORY_DETAIL_SMART_PTR_SCOPED_PTR_HPP */
