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

#include "concepts/Stateless.hpp"         // is_stateless
#include "concepts/Allocator.hpp"         // is_allocator, allocator_has_allocate,
#include "concepts/ExtendedAllocator.hpp" // allocator_has_extended_try_allocate

#include "allocator_info.hpp"        // allocator_info
#include "errors.hpp"                // get_out_of_memory_handler
#include "macros.hpp"                // BIT_MEMORY_UNUSED
#include "owner.hpp"                 // owner
#include "pointer_utilities.hpp"     // to_raw_pointer
#include "uninitialized_storage.hpp" // uninitialized_construct_at, destroy_at, etc

#include <type_traits> // std::true_type, std::false_type, etc
#include <cstddef>     // std::size_t, std::ptrdiff_t
#include <limits>      // std::numeric_limits
#include <memory>      // std::addressof
#include <typeinfo>    // std::type_info

namespace bit {
  namespace memory {

    ///////////////////////////////////////////////////////////////////////////
    /// \brief The allocator_traits class template provides a standardized
    ///        way to access allocator functionality
    ///
    /// This will infer defaults for any optional values that may not be
    /// present in the allocator
    ///
    /// \tparam Allocator the allocator type. Must satisfy Allocator concept
    ///////////////////////////////////////////////////////////////////////////
    template<typename Allocator>
    class allocator_traits
    {
      static_assert( is_allocator<Allocator>::value,
                     "Allocator must be an Allocator" );

      //-----------------------------------------------------------------------
      // Public Members
      //-----------------------------------------------------------------------
    public:

      using pointer         = allocator_pointer_t<Allocator>;
      using const_pointer   = allocator_const_pointer_t<Allocator>;
      using size_type       = allocator_size_type_t<Allocator>;
      using difference_type = allocator_difference_type_t<Allocator>;

      // qualified 'bit::memory::' to avoid '-fpermissive' errors with gcc
      using is_stateless               = bit::memory::is_stateless<Allocator>;
      using default_alignment          = allocator_default_alignment<Allocator>;
      using max_alignment              = allocator_max_alignment<Allocator>;
      using can_truncate_deallocations = allocator_can_truncate_deallocations<Allocator>;
      using knows_ownership            = allocator_knows_ownership<Allocator>;

      template<typename T>
      using typed_pointer = typename std::pointer_traits<pointer>::template rebind<T>;
      template<typename T>
      using const_typed_pointer = typename std::pointer_traits<const_pointer>::template rebind<const T>;

      // todo
      using propagate_on_container_copy_assignment = std::false_type;
      using propagate_on_container_move_assignment = std::false_type;

      //-----------------------------------------------------------------------
      // Allocation
      //-----------------------------------------------------------------------
    public:

      /// \{
      /// \brief Attempts to allocate memory of at least \p size bytes,
      ///        aligned to \p align boundary
      ///
      /// On failure, this function returns \p nullptr
      ///
      /// \param alloc the allocator to allocate from
      /// \param hint pointer to a nearby memory location to allocate near
      /// \param size the size of the allocation
      /// \param align the alignment of the allocation
      /// \return the pointer to the allocated memory
      static pointer try_allocate( Allocator& alloc,
                                   size_type size,
                                   size_type align ) noexcept;
      static pointer try_allocate( Allocator& alloc,
                                   const_pointer hint,
                                   size_type size,
                                   size_type align ) noexcept;
      /// \}

      /// \{
      /// \brief Attempts to allocate memory of at least \p size bytes,
      ///        aligned to \p align boundary with an offset of \p offset bytes
      ///
      /// On failure, this function returns \p nullptr
      ///
      /// \param alloc the allocator to allocate from
      /// \param hint pointer to a nearby memory location to allocate near
      /// \param size the size of the allocation
      /// \param align the alignment of the allocation
      /// \param offset the offset of the allocation
      /// \return the pointer to the allocated memory
      template<typename U = Allocator,
               typename = std::enable_if<allocator_has_extended_allocate<U>::value>>
      static pointer try_allocate( Allocator& alloc,
                                   size_type size,
                                   size_type align,
                                   size_type offset ) noexcept;
      template<typename U = Allocator,
               typename = std::enable_if<allocator_has_extended_allocate<U>::value>>
      static pointer try_allocate( Allocator& alloc,
                                   const_pointer hint,
                                   size_type size,
                                   size_type align,
                                   size_type offset ) noexcept;
      /// \}

      //-----------------------------------------------------------------------

      /// \{
      /// \brief Allocates memory of at least \p size bytes, aligned to \p
      ///        align boundary
      ///
      /// On failure, this function may throw or invoke the out_of_memory
      /// handler before returning \p nullptr
      ///
      /// \param alloc the allocator to allocate from
      /// \param hint pointer to a nearby memory location to allocate near
      /// \param size the size of the allocation
      /// \param align the alignment of the allocation
      /// \return the pointer to the allocated member
      static pointer allocate( Allocator& alloc,
                               size_type size,
                               size_type align );
      static pointer allocate( Allocator& alloc,
                               const_pointer hint,
                               size_type size,
                               size_type align );
      /// \}

      /// \{
      /// \brief Allocates memory of at least \p size bytes, aligned to \p
      ///        align boundary
      ///
      /// On failure, this function may throw or invoke the out_of_memory
      /// handler before returning \p nullptr
      ///
      /// \param alloc the allocator to allocate from
      /// \param hint pointer to a nearby memory location to allocate near
      /// \param size the size of the allocation
      /// \param align the alignment of the allocation
      /// \param offset the offset of the allocation
      /// \return the pointer to the allocated member
      template<typename U = Allocator,
               typename = std::enable_if<allocator_has_extended_allocate<U>::value>>
      static pointer allocate( Allocator& alloc,
                               size_type size,
                               size_type align,
                               size_type offset );
      template<typename U = Allocator,
               typename = std::enable_if<allocator_has_extended_allocate<U>::value>>
      static pointer allocate( Allocator& alloc,
                               const_pointer hint,
                               size_type size,
                               size_type align,
                               size_type offset );
      /// \}

      //-----------------------------------------------------------------------

      /// \{
      /// \brief Expands the memory addres located at \p p to contain \p
      ///        new_size bytes
      ///
      /// This function returns a boolean indicating whether the memory
      /// region was able to be expanded to contain the new size. If the region
      /// could not contain the new size, nothing is changed, and \c false is
      /// returned.
      ///
      /// \note The default implementation if this is not defined in
      ///       \c Allocator is to simply do nothing and return \c false
      ///
      /// \param alloc the allocator to use for expansion
      /// \param p the pointer to expand
      /// \param new_size the new size to expand to
      /// \return \c true if the memory region was expanded, \c false otherwise
      static bool expand( Allocator& alloc,
                          pointer p,
                          size_type new_size );
      /// \}

      //-----------------------------------------------------------------------
      // Deallocation
      //-----------------------------------------------------------------------
    public:

      /// \brief Deallocates a pointer previously allocated with \p allocate
      ///        or try_allocate
      ///
      /// \param alloc the allocator to allocate from
      /// \param p the pointer to deallocate
      /// \param size the size of the allocation
      static void deallocate( Allocator& alloc, pointer p, size_type size );

      /// \brief Deallocates all memory from the given allocator
      ///
      /// This is only enabled if the underlying allocator supports it
      ///
      /// \param alloc the allocator to deallocate everything from
      template<typename U = Allocator, typename = std::enable_if<can_truncate_deallocations::value>>
      static void deallocate_all( Allocator& alloc );

      //-----------------------------------------------------------------------
      // Observers
      //-----------------------------------------------------------------------
    public:

      /// \brief Determines the recommended allocation size of \p requested
      ///        bytes from the given allocator
      ///
      /// \note This invokes \c alloc.recommended_allocation_size(requested) if
      ///       it is defined for the specified Allocator -- otherwise it
      ///       defaults to rounding 'requested' up to the next aligned value
      ///
      /// \param alloc the allocator
      /// \param requested the amount of requested bytes
      /// \return the recommended amount to allocate
      size_type recommended_allocation_size( const Allocator& alloc,
                                             size_type requested );

      //-----------------------------------------------------------------------

      /// \brief Checks if the given allocator is known to own the specified
      ///        pointer \p p
      ///
      /// \note This directly invokes \c alloc.owns(p) . It is undefined
      ///       behaviour to invoke this if \c Allocator::owns is not
      ///       defined; branches should instead be taken by using
      ///       \ref knows_ownership
      ///
      /// \param alloc the allocator
      /// \param p the pointer
      static bool owns( const Allocator& alloc, const_pointer p ) noexcept;

      /// \brief Gets the name of the specified allocator
      ///
      /// \note Not all allocators are nameable or have a name specified.
      ///       For these allocators, the string returned is
      ///       \c typeid(Allocator).name()
      ///
      /// \note The lifetime of the pointer returned is unmanaged, and is NOT
      ///       the responsibility of the caller to free.
      ///
      /// \param alloc the allocator to get the name of
      /// \return the name of the allocator
      static allocator_info info( const Allocator& alloc ) noexcept;

      //-----------------------------------------------------------------------
      // Constructions
      //-----------------------------------------------------------------------
    public:

      // Unfortunately, MSVC is unable to compile 'typed_allocator<T>' as a
      // return type, with the definition written out-of-line -- so the following
      // is expanded in place.
      //
      // gcc/clang accept 'typed_pointer<T>' with its expansion:
      // typename bit::memory::allocator_traits<Allocator>::template typed_pointer<T>
      // MSVC does not, but curiously will except the ever-verbose expansion
      // of what 'typed_pointer<T>' aliases.

      /// \brief Allocates and constructs a type \p T with the arguments
      ///        \p args
      ///
      /// \note This will call the out-of-memory handler if an allocation fails
      ///
      /// \note This function cannot be overrided in any allocator
      ///       implementation; the behaviour is fixed in the allocator_traits
      ///       type
      ///
      /// \tparam T the type to construct
      /// \param alloc the allocator to use for construction
      /// \param args the arguments to forward to T's constructor
      /// \return pointer to the constructed type
      template<typename T, typename...Args>
      static typename std::pointer_traits<pointer>::template rebind<T>
        make( Allocator& alloc, Args&&...args );

      //-----------------------------------------------------------------------

      /// \{
      /// \brief Allocates and construct an array of type \p T
      ///
      /// \note This will call the out-of-memory handler if an allocation fails
      ///
      /// \note This function cannot be overrided in any allocator
      ///       implementation; the behaviour is fixed in the allocator_traits
      ///       type
      ///
      /// \tparam T the type to construct
      /// \param alloc the allocator to use for construction
      /// \param n the size of the array
      /// \param copy an instance to copy to each array entry
      /// \return pointer to the constructed array
      template<typename T>
      static typename std::pointer_traits<pointer>::template rebind<T>
        make_array( Allocator& alloc, size_type n );
      template<typename T>
      static typename std::pointer_traits<pointer>::template rebind<T>
        make_array( Allocator& alloc, size_type n, const T& copy );
      /// \}

      //-----------------------------------------------------------------------
      // Destruction
      //-----------------------------------------------------------------------
    public:

      /// \brief Disposes of the given type \p T
      ///
      /// This calls T's destructor, and then deallocates it with \p alloc
      ///
      /// \note It is undefined behavior to call 'dispose' with type different
      ///       than the original type used in the 'make' call. This is a
      ///       result of requiring the size of the allocation, which is
      ///       inferred from the size of type \c T
      ///
      /// \note It is undefined behavior if T's destructor throws during a
      ///       call to dispose
      ///
      /// \note This function cannot be overrided in any allocator
      ///       implementation; the behaviour is fixed in the allocator_traits
      ///       type
      ///
      /// \param alloc the allocater to use for deallocation
      /// \param pointer to the type to destroy
      template<typename T>
      static void dispose( Allocator& alloc,
                           typed_pointer<T> p );

      /// \brief Disposes of the given array of T
      ///
      /// This calls the destructor of each T, and then deallocates it
      /// with \p alloc
      ///
      /// \note It is undefined behaviour to call 'dispose_array' with a size
      ///       that is different than the original size allocated with a
      ///       'make_array' call.
      ///
      /// \note It is undefined behavior to call 'dispose_array' with a type
      ///       different than the original type used in the 'make' call. This
      ///       is a result of requiring the size of the allocation, which is
      ///       inferred from the size of type \c T. Additionally, casting an
      ///       array to a different type is usually UB anyway.
      ///
      /// \note It is undefined behavior if T's destructor throws during a
      ///       call to dispose
      ///
      /// \note This function cannot be overrided in any allocator
      ///       implementation; the behaviour is fixed in the allocator_traits
      ///       type
      ///
      /// \param alloc the allocater to use for deallocation
      /// \param pointer to the type to destroy
      template<typename T>
      static void dispose_array( Allocator& alloc,
                                 typed_pointer<T> p,
                                 size_type n );

      //-----------------------------------------------------------------------
      // Capacity
      //-----------------------------------------------------------------------
    public:

      /// \brief Determines if an allocator is unbounded
      ///
      /// The default behavior is to check for if \c max_size() returns
      /// the largest possible value for \c size_type.
      ///
      /// \param alloc the allocato to check
      /// \return \c true if \p alloc is unbounded
      static bool is_unbounded( const Allocator& alloc ) noexcept;

      //-----------------------------------------------------------------------

      /// \brief Gets the maximum size allocateable from this allocator
      ///
      /// \note the max_size does not guarantee that the size is able to be
      ///       allocated at the time the value is queried; it just means that
      ///       it is the maximum size that the allocator is capable of
      ///       allocating from an empty state.
      ///
      /// \param alloc the allocator to get the max size from
      /// \return the amount of bytes available for the largest possible
      ///         allocation
      static size_type max_size( const Allocator& alloc ) noexcept;

      /// \brief Gets the minimum size allocateable from this allocator
      ///
      /// \note The default is 1, if this function is not defined
      ///
      /// \param alloc the allocator to get the min size from
      /// \return the minimum amount of bytes able to allocated
      static size_type min_size( const Allocator& alloc ) noexcept;

      //-----------------------------------------------------------------------
      // Private Allocation
      //-----------------------------------------------------------------------
    private:

      /// \{
      /// \brief Performs the allocation with a hint, either by calling
      ///        the underlying hint allocator if it exists -- or ignoring
      ///        the hint.
      ///
      /// \param alloc the allocator to allocate
      /// \param hint the hint to provide
      /// \param size the size of the allocation
      /// \param align the alignment of the allocation
      /// \return the allocated pointer
      static pointer do_try_allocate_hint( std::true_type,
                                           Allocator& alloc,
                                           const_pointer hint,
                                           size_type size,
                                           size_type align );
      static pointer do_try_allocate_hint( std::false_type,
                                           Allocator& alloc,
                                           const_pointer hint,
                                           size_type size,
                                           size_type align );
      /// \}

      //-----------------------------------------------------------------------

      /// \{
      /// \brief Performs the allocation with a hint, either by calling
      ///        the underlying hint allocator if it exists -- or ignoring
      ///        the hint.
      ///
      /// \param alloc the allocator to allocate
      /// \param hint the hint to provide
      /// \param size the size of the allocation
      /// \param align the alignment of the allocation
      /// \param offset the offset of the alignment
      /// \return the allocated pointer
      static pointer do_extended_try_allocate_hint( std::true_type,
                                                    Allocator& alloc,
                                                    const_pointer p,
                                                    size_type size,
                                                    size_type align,
                                                    size_type offset );
      static pointer do_extended_try_allocate_hint( std::false_type,
                                                    Allocator& alloc,
                                                    const_pointer p,
                                                    size_type size,
                                                    size_type align,
                                                    size_type offset );
      /// \}

      //-----------------------------------------------------------------------

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
      static pointer do_allocate( std::true_type,
                                  Allocator& alloc,
                                  size_type size,
                                  size_type align );
      static pointer do_allocate( std::false_type,
                                  Allocator& alloc,
                                  size_type size,
                                  size_type align );
      /// \}

      //-----------------------------------------------------------------------

      /// \{
      /// \brief Performs the allocation, either by calling
      ///        \c Allocator::allocate, or by calling into
      ///        \c Allocator::try_allocate and defining a boilerplate
      ///        allocate function
      ///
      /// \param alloc the allocator to allocate
      /// \param size the size of the allocation
      /// \param align the alignment of the allocation
      /// \param offset the offset of the alignment
      /// \return the allocated pointer
      static pointer do_extended_allocate( std::true_type,
                                           Allocator& alloc,
                                           size_type size,
                                           size_type align,
                                           size_type offset );
      static pointer do_extended_allocate( std::false_type,
                                           Allocator& alloc,
                                           size_type size,
                                           size_type align,
                                           size_type offset );
      /// \}

      //-----------------------------------------------------------------------

      /// \{
      /// \brief Performs the allocation with a hint, either by calling
      ///        the underlying hint allocator if it exists -- or ignoring
      ///        the hint.
      ///
      /// \param alloc the allocator to allocate
      /// \param hint the hint to provide
      /// \param size the size of the allocation
      /// \param align the alignment of the allocation
      /// \return the allocated pointer
      static pointer do_allocate_hint( std::true_type,
                                       Allocator& alloc,
                                       const_pointer p,
                                       size_type size,
                                       size_type align );
      static pointer do_allocate_hint( std::false_type,
                                       Allocator& alloc,
                                       const_pointer p,
                                       size_type size,
                                       size_type align );
      /// \}

      //-----------------------------------------------------------------------

      /// \{
      /// \brief Performs the allocation with a hint, either by calling
      ///        the underlying hint allocator if it exists -- or ignoring
      ///        the hint.
      ///
      /// \param alloc the allocator to allocate
      /// \param hint the hint to provide
      /// \param size the size of the allocation
      /// \param align the alignment of the allocation
      /// \param offset the offset of the alignment
      /// \return the allocated pointer
      static pointer do_extended_allocate_hint( std::true_type,
                                                Allocator& alloc,
                                                const_pointer p,
                                                size_type size,
                                                size_type align,
                                                size_type offset );
      static pointer do_extended_allocate_hint( std::false_type,
                                                Allocator& alloc,
                                                const_pointer p,
                                                size_type size,
                                                size_type align,
                                                size_type offset );
      /// \}

      //-----------------------------------------------------------------------

      /// \{
      /// \brief Performs the expansion of a given pointer \p p, returning the
      ///        success
      ///
      /// The default always returns \c false
      ///
      /// \param alloc the allocator to perform the expansion
      /// \param p the pointer to expand
      /// \param new_size the new size to request
      /// \return \c true if it was expanded
      static bool do_expand( std::true_type,
                             Allocator& alloc,
                             pointer p,
                             size_type new_size );
      static bool do_expand( std::false_type,
                             Allocator& alloc,
                             pointer p,
                             size_type new_size );
      /// \}

      //-----------------------------------------------------------------------
      // Private Observers
      //-----------------------------------------------------------------------
    private:

      /// \{
      /// \brief Returns the recommended allocation size given the \p requested
      ///        size
      ///
      /// \param alloc the allocator to make the recommendation
      /// \param requested the amount of bytes originally being requested
      /// \return the recommended size
      size_type do_recommended_allocation_size( std::true_type,
                                                const Allocator& alloc,
                                                size_type requested );
      size_type do_recommended_allocation_size( std::false_type,
                                                const Allocator& alloc,
                                                size_type requested );
      /// \}

      /// \{
      /// \brief Determines the info for the allocator, either by calling
      ///        \c Allocator::info or by assuming "Unnamed" for the name
      ///
      /// \param alloc the allocator
      /// \return the name of the allocator
      static allocator_info do_info( std::true_type, const Allocator& alloc );
      static allocator_info do_info( std::false_type, const Allocator& alloc );
      /// \}

      //-----------------------------------------------------------------------
      // Private Constructions
      //-----------------------------------------------------------------------
    private:

      /// \{
      /// \brief Tries to call the underlying 'make' function for the allocator,
      ///        otherwise calls a custom implementation.
      ///
      /// \throws any exception thrown by 'T's constructor
      /// \param alloc the allocator to perform the allocation
      /// \param args the arguments to forward to 'T's constructor
      /// \return a pointer to the allocated/constructed array
      template<typename T, typename...Args>
      static typename std::pointer_traits<pointer>::template rebind<T>
        do_make( std::true_type,
                 Allocator& alloc,
                 Args&&...args );
      template<typename T, typename...Args>
      static typename std::pointer_traits<pointer>::template rebind<T>
        do_make( std::false_type,
                 Allocator& alloc,
                 Args&&...args );
      /// \}

      /// \{
      /// \brief Makes an instance by allocating memory and constructing
      ///        the type by forwarding 'args'.
      ///
      /// On allocation failure, this function will invoke the out-of-memory
      /// handler
      ///
      /// \throws any exception thrown by 'T's constructor
      /// \param alloc the allocator to perform the allocation
      /// \param args the arguments to forward to 'T's constructor
      /// \return a pointer to the allocated/constructed array
      template<typename T, typename...Args>
      static typename std::pointer_traits<pointer>::template rebind<T>
        do_make_nothrow( std::true_type,
                         Allocator& alloc,
                         Args&&...args )
        noexcept;
      template<typename T, typename...Args>
      static typename std::pointer_traits<pointer>::template rebind<T>
        do_make_nothrow( std::false_type,
                         Allocator& alloc,
                         Args&&...args );
      /// \}

      //-----------------------------------------------------------------------

      /// \brief Calls 'make_array' from the underlying allocator if it
      ///        exists, otherwise calls a custom implementation
      ///
      /// \throws any exception thrown by 'T's constructor
      /// \param alloc the allocator to perform the allocation
      /// \param n the number of entries in the array
      /// \param args the arguments to forward to 'T's constructor
      /// \return a pointer to the allocated/constructed array
      template<typename T, typename...Args>
      static typename std::pointer_traits<pointer>::template rebind<T>
        do_make_array( std::true_type,
                       Allocator& alloc,
                       std::size_t n,
                       Args&&...args );
      template<typename T, typename...Args>
      static typename std::pointer_traits<pointer>::template rebind<T>
        do_make_array( std::false_type,
                       Allocator& alloc,
                       std::size_t n,
                       Args&&...args );

      /// \{
      /// \brief Makes an array by allocating memory and constructing
      ///        an array by forwarding 'args' to each entry.
      ///
      /// On allocation failure, this function will invoke the out-of-memory
      /// handler
      ///
      /// \throws any exception thrown by 'T's constructor
      /// \param alloc the allocator to perform the allocation
      /// \param n the number of entries in the array
      /// \param args the arguments to forward to 'T's constructor
      /// \return a pointer to the allocated/constructed array
      template<typename T, typename...Args>
      static typename std::pointer_traits<pointer>::template rebind<T>
        do_make_array_nothrow( std::true_type,
                               Allocator& alloc,
                               std::size_t n,
                               Args&&...args )
        noexcept;
      template<typename T, typename...Args>
      static typename std::pointer_traits<pointer>::template rebind<T>
        do_make_array_nothrow( std::false_type,
                               Allocator& alloc,
                               std::size_t n,
                               Args&&...args );
      /// \}

      //-----------------------------------------------------------------------
      // Private Destruction
      //-----------------------------------------------------------------------
    private:

      /// \{
      /// \brief Calls the allocator's underlying 'dispose' function if it
      ///        exists, otherwise calls a custom implementation
      ///
      /// \param alloc the allocator to use for deallocating
      /// \param p the pointer to dispose
      template<typename T>
      static void do_dispose( std::true_type,
                              Allocator& alloc,
                              typed_pointer<T> p );
      template<typename T>
      static void do_dispose( std::false_type,
                              Allocator& alloc,
                              typed_pointer<T> p );
      /// \}

      /// \{
      /// \brief Calls the allocator's underlying 'dispose' function if it
      ///        exists, otherwise calls a custom implementation
      ///
      /// \param alloc the allocator to use for deallocating
      /// \param p the pointer to dispose
      template<typename T>
      static void do_dispose_array( std::true_type,
                                    Allocator& alloc,
                                    typed_pointer<T> p,
                                    size_type n );
      template<typename T>
      static void do_dispose_array( std::false_type,
                                    Allocator& alloc,
                                    typed_pointer<T> p,
                                    size_type n );
      /// \}

      //-----------------------------------------------------------------------
      // Private Capacity
      //-----------------------------------------------------------------------
    private:

      /// \{
      /// \brief Determines whether the allocator is considered unbounded
      ///
      /// By default, this is determined by checking the max value of the given
      /// 'size_type' using 'std::numeric_limits' with 'max_size'
      ///
      /// \param alloc the allocator
      /// \return the max size
      static bool do_is_unbounded( std::true_type, const Allocator& alloc );
      static bool do_is_unbounded( std::false_type, const Allocator& alloc );
      /// \}


      /// \{
      /// \brief Determines the max size of the allocator, either by
      ///        calling \c Allocator::max_size, or by assuming
      ///        \c std::numeric_limits<size_type>::max()
      ///
      /// \param alloc the allocator
      /// \return the max size
      static size_type do_max_size( std::true_type, const Allocator& alloc );
      static size_type do_max_size( std::false_type, const Allocator& alloc );
      /// \}

      //-----------------------------------------------------------------------

      /// \{
      /// \brief Determines the min size of an allocation, either by
      ///        calling \c Allocator::min_size(), or by assuming
      ///        \c 1
      ///
      /// \param alloc the allocator
      /// \return the max size
      static size_type do_min_size( std::true_type, const Allocator& alloc );
      static size_type do_min_size( std::false_type, const Allocator& alloc );
      /// \}
    };
  } // namespace memory
} // namespace bit

#include "detail/allocator_traits.inl"

#endif /* BIT_MEMORY_ALLOCATORS_ALLOCATOR_TRAITS_HPP */
