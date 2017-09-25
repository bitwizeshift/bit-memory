/**
 * \file allocator_traits.hpp
 *
 * \brief This header contains the trait type \c allocator_traits that
 *        determines or infers default values for the Allocator concept
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */

#ifndef BIT_MEMORY_ALLOCATORS_ALLOCATOR_TRAITS_HPP
#define BIT_MEMORY_ALLOCATORS_ALLOCATOR_TRAITS_HPP

#include "../errors.hpp"        // get_out_of_memory_handler
#include "../detail/void_t.hpp" // void_t

#include <type_traits> // std::true_type, std::false_type, etc
#include <cstddef>     // std::size_t, std::ptrdiff_t
#include <limits>      // std::numeric_limits
#include <memory>      // std::addressof

namespace bit {
  namespace memory {
    namespace detail {

      template<typename T, typename = void>
      struct is_allocator : std::false_type{};

      template<typename T>
      struct is_allocator<T,void_t<
        decltype( std::declval<void*&>() = std::declval<T&>().try_allocate( std::declval<std::size_t>(), std::declval<std::size_t>() ) ),
        decltype( std::declval<T&>().deallocate( std::declval<void*>(), std::declval<std::size_t>() ) )>
      > : std::true_type{};

      //----------------------------------------------------------------------
      // Allocator traits
      //----------------------------------------------------------------------

      template<typename T, typename = void>
      struct allocator_has_allocate : std::false_type{};

      template<typename T>
      struct allocator_has_allocate<T,
        void_t<decltype(std::declval<void*&>() = std::declval<T&>().allocate( std::declval<std::size_t>(), std::declval<std::size_t>() ) )>
      > : std::true_type{};

      //----------------------------------------------------------------------

      template<typename T, typename = void>
      struct allocator_has_name : std::false_type{};

      template<typename T>
      struct allocator_has_name<T,
        void_t<decltype( std::declval<const char*&>() = std::declval<T&>().name() )>
      > : std::true_type{};

      //----------------------------------------------------------------------

      template<typename T, typename = void>
      struct allocator_has_max_size : std::false_type{};

      template<typename T>
      struct allocator_has_max_size<T,
        void_t<decltype( std::declval<std::size_t&>() = std::declval<T&>().max_size() )>
      > : std::true_type{};

      //----------------------------------------------------------------------

      template<typename T, typename = void>
      struct allocator_has_used : std::false_type{};

      template<typename T>
      struct allocator_has_used<T,
        void_t<decltype( std::declval<std::size_t&>() = std::declval<T&>().used() )>
      > : std::true_type{};

      //----------------------------------------------------------------------

      template<typename T, typename = void>
      struct allocator_is_always_equal : std::false_type{};

      template<typename T>
      struct allocator_is_always_equal<T,void_t<decltype(T::is_always_equal)>> : T::is_always_equal{};

      //----------------------------------------------------------------------

      template<typename T, typename = void>
      struct allocator_is_stateless : std::false_type{};

      template<typename T>
      struct allocator_is_stateless<T,void_t<decltype(T::is_stateless)>> : T::is_stateless{};

      //----------------------------------------------------------------------

      template<typename T, typename = void>
      struct allocator_default_alignment : std::integral_constant<std::size_t,1>{};

      template<typename T>
      struct allocator_default_alignment<T,void_t<decltype(T::default_alignment)>> : T::default_alignment{};

      //----------------------------------------------------------------------

      template<typename T, typename = void>
      struct allocator_max_alignment : std::integral_constant<std::size_t,alignof(std::max_align_t)>{};

      template<typename T>
      struct allocator_max_alignment<T,void_t<decltype(T::max_alignment)>> : std::integral_constant<std::size_t,alignof(std::max_align_t)>{};

    } // namespace detail

    /// \brief Type-trait to determine whether \p T is an allocator
    ///
    /// The result is \c std::true_type if \p T satisfies the Allocator concept,
    /// which minimally requires the following to be well formed:
    ///
    /// \code
    /// std::declval<T&>().try_allocate( std::declval<std::size_t>(), std::declval<std::size_t>() );
    /// std::declval<T&>().deallocate( std::declval<void*>(), std::declval<std::size_t>() );
    /// \endcode
    ///
    /// The result is accessible as \c ::value
    template<typename T>
    using is_allocator = detail::is_allocator<T>;

    /// \brief Convenience template variable to extract whether \p T is an
    ///        allocator
    template<typename T>
    constexpr bool is_allocator_v = is_allocator<T>::value;

    //////////////////////////////////////////////////////////////////////////
    /// \brief The allocator_traits class template provides a standardized
    ///        way to access allocator functionality
    ///
    /// This will infer defaults for any optional values that may not be
    /// present in the allocator
    ///
    /// \tparam Allocator the allocator type. Must satisfy Allocator concept
    //////////////////////////////////////////////////////////////////////////
    template<typename Allocator>
    class allocator_traits
    {
      //----------------------------------------------------------------------
      // Public Members
      //----------------------------------------------------------------------
    public:

      /// Whether or not
      using is_always_equal   = detail::allocator_is_always_equal<Allocator>;
      using is_stateless      = detail::allocator_is_stateless<Allocator>;
      using default_alignment = detail::allocator_default_alignment<Allocator>;
      using max_alignment     = detail::allocator_max_alignment<Allocator>;

      //----------------------------------------------------------------------
      // Allocation
      //----------------------------------------------------------------------
    public:

      /// \brief Attempts to allocate memory of at least \p size bytes,
      ///        aligned to \p align boundary
      ///
      /// On failure, this function returns \p nullptr
      ///
      /// \param alloc the allocator to allocate from
      /// \param size the size of the allocation
      /// \param align the alignment of the allocation
      /// \return the pointer to the allocated member
      static void* try_allocate( Allocator& alloc,
                                 std::size_t size,
                                 std::size_t align ) noexcept;

      /// \brief Allocates memory of at least \p size bytes, aligned to \p
      ///        align boundary
      ///
      /// On failure, this function may throw or invoke the out_of_memory
      /// handler before returning \p nullptr
      ///
      /// \param alloc the allocator to allocate from
      /// \param size the size of the allocation
      /// \param align the alignment of the allocation
      /// \return the pointer to the allocated member
      static void* allocate( Allocator& alloc,
                             std::size_t size,
                             std::size_t align );

      //----------------------------------------------------------------------
      // Deallocation
      //----------------------------------------------------------------------

      /// \brief Deallocates a pointer previously allocated with \p allocate
      ///        or try_allocate
      ///
      /// \param alloc the allocator to allocate from
      /// \param p the pointer to deallocate
      /// \param size the size of the allocation
      static void deallocate( Allocator& alloc, void* p, std::size_t size );

      //----------------------------------------------------------------------
      // Capacity
      //----------------------------------------------------------------------

      /// \brief Gets the maximum size allocateable from this allocator
      ///
      /// \note the max_size does not guarantee that the size is able to be
      ///       allocated at the time the value is queried; it just means that
      ///       it is the maximum size that the allocator is capable of
      ///       allocating from an empty state.
      ///
      /// \param alloc the allocator to get the max size from
      ///
      /// \return the amount of bytes available for the largest possible
      ///         allocation
      static std::size_t max_size( Allocator& alloc ) noexcept;

      /// \brief Gets the current amount of bytes used by this allocator
      ///
      /// \note the amount used may return \c 0 if an allocator does not keep
      ///       a record of the allocations made
      ///
      /// \param alloc the allocator to to check the amount used
      /// \return the amount of bytes used
      static std::size_t used( Allocator& alloc ) noexcept;

      //----------------------------------------------------------------------
      // Name
      //----------------------------------------------------------------------

      /// \brief Gets the name of the specified allocator
      ///
      /// \note Not all allocators are nameable or have a name specified.
      ///       For these allocators, the string returned is "Unnamed"
      /// \note The lifetime of the pointer returned is unmanaged, and is NOT
      ///       the responsibility of the caller to free.
      ///
      /// \param alloc the allocator to get the name of
      /// \return the name of the allocator
      static const char* name( Allocator& alloc ) noexcept;

      //----------------------------------------------------------------------
      // Private Implementation
      //----------------------------------------------------------------------
    private:

      /// \{
      /// \brief Performs the allocation, either by calling
      ///        \c Allocator::allocate, or by calling into
      ///        \c Allocator::try_allocate and defining a boilerplate
      ///        allocate function
      ///
      /// \param alloc the allocator to allocate
      /// \param size the size of the allocation
      /// \param align the alignment of the allocation
      /// \return the allocated pointer
      static void* do_allocate( std::true_type, Allocator& alloc, std::size_t size, std::size_t align );
      static void* do_allocate( std::false_type, Allocator& alloc, std::size_t size, std::size_t align );
      /// \}

      //----------------------------------------------------------------------

      /// \{
      /// \brief Determines the max size of the allocator, either by
      ///        calling \c Allocator::max_size, or by assuming
      ///        \c std::numeric_limits<std::size_t>::max()
      ///
      /// \param alloc the allocator
      /// \return the max size
      static std::size_t do_max_size( std::true_type, Allocator& alloc );
      static std::size_t do_max_size( std::false_type, Allocator& alloc );
      /// \}

      //----------------------------------------------------------------------

      /// \{
      /// \brief Determines the amount of bytes used by the allocator, either
      ///        by calling \c Allocator::used, or by assuming
      ///        \c 0
      ///
      /// \param alloc the allocator
      /// \return the bytes used
      static std::size_t do_used( std::true_type, Allocator& alloc );
      static std::size_t do_used( std::false_type, Allocator& alloc );
      /// \}

      //----------------------------------------------------------------------

      /// \{
      /// \brief Determines the name of the allocator, either by calling
      ///        \c Allocator::name or by assuming "Unnamed"
      ///
      /// \param alloc the allocator
      /// \return the name of the allocator
      static const char* do_name( std::true_type, Allocator& alloc );
      static const char* do_name( std::false_type, Allocator& alloc );
      /// \}
    };

  } // namespace memory
} // namespace bit

#include "detail/allocator_traits.inl"

#endif /* BIT_MEMORY_ALLOCATORS_ALLOCATOR_TRAITS_HPP */
