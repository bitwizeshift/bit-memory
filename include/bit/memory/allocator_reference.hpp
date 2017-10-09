/**
 * \file allocator_reference.hpp
 *
 * \brief This header defines a manner to wrap allocators in a reference-like
 *        type
 *
 * This allows allocators to be shared and accessed from multiple locations at
 * a single time, without requiring pointer-semantics. \c allocator_traits
 * and \c block_allocator_traits both work with allocator references and will
 * behave as though it is the underlying allocator.
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */
#ifndef BIT_MEMORY_ALLOCATOR_REFERENCE_HPP
#define BIT_MEMORY_ALLOCATOR_REFERENCE_HPP

#include "concepts/Stateless.hpp"      // Stateless
#include "concepts/Allocator.hpp"      // Allocator
#include "concepts/BlockAllocator.hpp" // BlockAllocator

#include "macros.hpp"  // BIT_MEMORY_UNUSED

#include <type_traits> // std::is_final
#include <memory>      // std::addressof

namespace bit {
  namespace memory {
    namespace detail {

      template<typename Allocator,
               bool IsStateless = is_stateless<Allocator>::value && !std::is_final<Allocator>::value>
      class allocator_reference_base;

      template<typename Allocator>
      class allocator_reference_base<Allocator,true>
        : private Allocator
      {
        //---------------------------------------------------------------------
        // Constructor
        //---------------------------------------------------------------------
      public:

        /// \brief Default constructs the allocator reference
        allocator_reference_base() noexcept = default;

        /// \brief constructs an allocator_reference with a reference to the
        ///        specified \p allocator
        ///
        /// \param allocator the allocator to reference
        allocator_reference_base( Allocator& allocator ) noexcept;

        //---------------------------------------------------------------------
        // Element Access
        //---------------------------------------------------------------------
      public:

        operator Allocator&() const noexcept;

        Allocator& get() const noexcept;
      };

      template<typename Allocator>
      class allocator_reference_base<Allocator,false>
      {
        //---------------------------------------------------------------------
        // Constructors
        //---------------------------------------------------------------------
      public:

        /// \brief constructs an allocator_reference with a reference to the
        ///        specified \p allocator
        ///
        /// \param allocator the allocator to reference
        allocator_reference_base( Allocator& allocator ) noexcept;

        //---------------------------------------------------------------------
        // Element Access
        //---------------------------------------------------------------------
      public:

        /// \brief Implicitly converts this allocator_reference to the type of
        ///        the underlying allocator
        operator Allocator&() const noexcept;

        /// \brief Explicit getter to get the underlying reference of the
        ///        allocator
        Allocator& get() const noexcept;

        //---------------------------------------------------------------------
        // Private Members
        //---------------------------------------------------------------------
      private:

        Allocator* m_allocator;
      };
    } // namespace detail

    ///////////////////////////////////////////////////////////////////////////
    /// \brief allocator_reference is a utility that references an underlying
    ///        allocator in a simple way
    ///
    /// \note allocator_reference does not satisfy any of the Allocator traits.
    ///
    /// \tparam Allocator the allocator to reference
    ///////////////////////////////////////////////////////////////////////////
    template<typename Allocator>
    class allocator_reference
      : public detail::allocator_reference_base<Allocator>
    {
      using base_type = detail::allocator_reference_base<Allocator>;

      static_assert( is_allocator<Allocator>::value ||
                     is_block_allocator<Allocator>::value,
                     "Allocator must satisfy either Allocator or BlockAllocator" );

      //-----------------------------------------------------------------------
      // Constructor
      //-----------------------------------------------------------------------
    public:

      // inherit the constructors
      using base_type::base_type;

      /// \brief Move-constructs a allocator_reference from another allocator
      ///
      /// \param other the other allocator_reference to move
      allocator_reference( allocator_reference&& other ) noexcept = default;

      // Deleted copy constructor
      allocator_reference( const allocator_reference& other ) noexcept = default;

      //-----------------------------------------------------------------------

      /// \brief Move-assigns a allocator_reference from another allocator
      ///
      /// \param other the other allocator to move_assign
      /// \return reference to \c (*this)
      allocator_reference& operator=( allocator_reference&& other ) noexcept = default;

      // Deleted copy assignment
      allocator_reference& operator=( const allocator_reference& other ) noexcept = default;

      //-----------------------------------------------------------------------
      // Element Access
      //-----------------------------------------------------------------------
    public:

      using base_type::operator Allocator&;
      using base_type::get;
    };

    //-------------------------------------------------------------------------
    // Comparisons
    //-------------------------------------------------------------------------

    template<typename A>
    bool operator==( allocator_reference<A>& lhs, allocator_reference<A>& rhs ) noexcept;

    template<typename A>
    bool operator!=( allocator_reference<A>& lhs, allocator_reference<A>& rhs ) noexcept;

    template<typename A>
    bool operator<( allocator_reference<A>& lhs, allocator_reference<A>& rhs ) noexcept;

    template<typename A>
    bool operator>( allocator_reference<A>& lhs, allocator_reference<A>& rhs ) noexcept;

    template<typename A>
    bool operator<=( allocator_reference<A>& lhs, allocator_reference<A>& rhs ) noexcept;

    template<typename A>
    bool operator>=( allocator_reference<A>& lhs, allocator_reference<A>& rhs ) noexcept;

    //-------------------------------------------------------------------------
    // Utilities
    //-------------------------------------------------------------------------

    /// \brief Makes an allocator reference from a given \p allocator
    ///
    /// \param allocator the allocator to make a reference to
    /// \return the allocator reference
    template<typename Allocator>
    allocator_reference<Allocator> make_allocator_reference( Allocator& allocator ) noexcept;

    //-------------------------------------------------------------------------
    // Traits
    //-------------------------------------------------------------------------

    /// \brief Trait to determine whether the underlying type is an
    ///        \c allocator_reference
    ///
    /// The result is aliased as \c ::value
    ///
    /// \tparam T the type to check
    template<typename T>
    struct is_allocator_reference : std::false_type{};

    template<typename T>
    struct is_allocator_reference<allocator_reference<T>> : std::true_type{};

    /// \brief Convenience value to extract \c is_allocator_reference<T>::value
    template<typename T>
    constexpr bool is_allocator_reference_v = is_allocator_reference<T>::value;

  } // namespace memory
} // namespace bit

#include "detail/allocator_reference.inl"

#endif /* BIT_MEMORY_ALLOCATOR_REFERENCE_HPP */
