/*****************************************************************************
 * \file
 * \brief This header contains the trait type \c allocator_traits that
 *        determines or infers default values for the Allocator concept
 *****************************************************************************/

/*
  The MIT License (MIT)

  Copyright (c) 2018 Matthew Rodusek

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/
#ifndef BIT_MEMORY_TRAITS_ALLOCATOR_TRAITS_HPP
#define BIT_MEMORY_TRAITS_ALLOCATOR_TRAITS_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "../utilities/allocator_info.hpp"        // allocator_info
#include "../utilities/errors.hpp"                // get_out_of_memory_handler
#include "../utilities/macros.hpp"                // BIT_MEMORY_UNUSED
#include "../utilities/owner.hpp"                 // owner
#include "../utilities/pointer_utilities.hpp"     // to_raw_pointer
#include "../utilities/uninitialized_storage.hpp" // destroy_at, etc

#include "../concepts/Allocator.hpp" // is_allocator, allocator_has_allocate,

#include <type_traits> // std::true_type, std::false_type, etc
#include <cstddef>     // std::size_t, std::ptrdiff_t
#include <limits>      // std::numeric_limits
#include <memory>      // std::addressof. std::pointer_traits
#include <typeinfo>    // std::type_info
#include <utility>     // std::move, std::forward

namespace bit {
  namespace memory {
    namespace detail {

      template<typename Allocator, typename T, typename=void>
      struct allocator_pointer_rebind
      {
        using type = typename std::pointer_traits<allocator_pointer_t<Allocator>>::template rebind<T>;
      };

      template<typename Allocator, typename T>
      struct allocator_pointer_rebind<Allocator,T,void_t<typename Allocator::template pointer_rebind<T>>>
      {
        using type = typename Allocator::template pointer_rebind<T>;
      };

    } // namespace detail

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

      template<typename T>
      struct negation : std::integral_constant<bool,!T::value>{};

      //-----------------------------------------------------------------------
      // Public Members
      //-----------------------------------------------------------------------
    public:

      // aliases
      using pointer         = allocator_pointer_t<Allocator>;
      using const_pointer   = allocator_const_pointer_t<Allocator>;
      using size_type       = allocator_size_type_t<Allocator>;
      using difference_type = allocator_difference_type_t<Allocator>;

      /// \brief Rebinds a pointer to the new pointer type
      ///
      /// \tparam T the type to rebind the pointer to
      template<typename T>
      using pointer_rebind = typename detail::allocator_pointer_rebind<Allocator,T>::type;

      // properties
      using default_alignment          = allocator_default_alignment<Allocator>;
      using max_alignment              = allocator_max_alignment<Allocator>;
      using can_truncate_deallocations = allocator_can_truncate_deallocations<Allocator>;
      using knows_ownership            = allocator_knows_ownership<Allocator>;
      using uses_pretty_pointers       = negation<std::is_same<void*,pointer_rebind<void>>>;

    private:

      template<typename Pointer>
      static constexpr bool is_typed_pointer = std::is_same<typename std::pointer_traits<Pointer>::template rebind<void>,
                                                            pointer>::value;

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
      // Constructions
      //-----------------------------------------------------------------------
    public:

      // Unfortunately, MSVC is unable to compile 'pointer_rebind<T>' as a
      // return type, with the definition written out-of-line -- so the following
      // is expanded in place.
      //
      // gcc/clang accept 'pointer_rebind<T>' with its expansion:
      // typename bit::memory::allocator_traits<Allocator>::template pointer_rebind<T>
      // MSVC does not, but curiously will accept the ever-verbose expansion
      // of what 'pointer_rebind<T>' aliases.

      /// \brief Constructs an instance of \p T at address \p p with \p args
      ///
      /// \tparam T the type to construct
      /// \param alloc the allocator to use for construction
      /// \param p the pointer to the memory to construct at
      /// \param args the arguments to forward to \p T's constructor
      /// \return pointer to teh constructed entry
      template<typename T, typename...Args>
      static void construct( Allocator& alloc, void* p, Args&&...args );

      /// \brief Allocates and constructs a type \p T with the arguments
      ///        \p args
      ///
      /// \note This will call the out-of-memory handler if an allocation fails
      ///
      /// \tparam T the type to construct
      /// \param alloc the allocator to use for construction
      /// \param args the arguments to forward to T's constructor
      /// \return pointer to the constructed type
      template<typename T, typename...Args>
      static typename detail::allocator_pointer_rebind<Allocator,T>::type
        make( Allocator& alloc, Args&&...args );

      //-----------------------------------------------------------------------

      /// \{
      /// \brief Allocates and construct an array of type \p T
      ///
      /// \note This will call the out-of-memory handler if an allocation fails
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

      /// \brief Destroys the instance at \p p by invoking the destructor
      ///
      /// \param alloc the allocator to use for destruction
      /// \param p the pointer to destroy
      template<typename T>
      static void destroy( Allocator& alloc, T* p );

      /// \brief Disposes of the given type \p T
      ///
      /// This calls T's destructor, and then deallocates it with \p alloc
      ///
      /// \note It is implementation-defined behavior to call 'dispose' with
      ///       a type that is different than the original type used in the
      ///       'make' call.
      ///       For the default implementation of 'dispose', it is undefined
      ///       behavior to call dispose with a type that isn't the same one
      ///       provided to 'make'. An allocator's implementation of
      ///       make/dispose, however, may fully support this. For example, with
      ///       a malloc_allocator or new_allocator, make and dispose don't
      ///       require a computation for determining the original allocation
      ///       size in order to dispose of the memory.
      ///
      /// \note It is undefined behavior if T's destructor throws during a
      ///       call to dispose
      ///
      /// \param alloc the allocater to use for deallocation
      /// \param p to the type to destroy
      template<typename Pointer,
               typename=std::enable_if_t<is_typed_pointer<Pointer>>>
      static void dispose( Allocator& alloc, const Pointer& p );

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
      /// \param alloc the allocater to use for deallocation
      /// \param pointer to the type to destroy
      template<typename Pointer,
               typename=std::enable_if_t<is_typed_pointer<Pointer>>>
      static void dispose_array( Allocator& alloc, const Pointer& p, size_type n );

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
      ///       behavior to invoke this if \c Allocator::owns is not
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
      // Conversion
      //-----------------------------------------------------------------------
    public:

      /// \brief Converts a reference to an entry into a pointer used by this
      ///        allocator
      ///
      /// This will attempt to call \c Allocator::pointer_to, if it exists, or
      /// \c std::pointer_traits::pointer_to if it doesn't
      ///
      /// \param alloc the allocator to use
      /// \param x the value
      /// \return pointer to the entry
      template<typename T>
      static typename std::pointer_traits<pointer>::template rebind<T>
        pointer_to( Allocator& alloc, T& x ) noexcept;

      /// \brief Converts a pointer from this allocator into a reference
      ///
      /// This will attempt to call \c Allocator::to_address, if it exists,
      /// \c *std::pointer_traits::to_address if it doesn't, and fall back to
      /// recursively dereferencing with \c operator->() until it results in
      /// a pointer otherwise.
      ///
      /// \param alloc the allocator to use
      /// \param p the pointer
      /// \return reference to the entry
      template<typename Pointer,
               typename=std::enable_if_t<is_typed_pointer<Pointer>>>
      static typename std::pointer_traits<Pointer>::element_type*
        to_raw_pointer( Allocator& alloc,
                        const Pointer& p ) noexcept;

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
    };
  } // namespace memory
} // namespace bit

#include "detail/allocator_traits.inl"

#endif /* BIT_MEMORY_TRAITS_ALLOCATOR_TRAITS_HPP */
