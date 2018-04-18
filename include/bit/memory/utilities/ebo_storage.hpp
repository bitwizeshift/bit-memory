/*****************************************************************************
 * \file
 * \brief This header contains an internal-only implementation of an EBO
 *        helper to reduce class-sizes.
 *
 * \note This is an internal header file, included by other library headers.
 *       Do not attempt to use it directly.
 */
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
#ifndef BIT_MEMORY_UTILITIES_DETAIL_EBO_STORAGE_HPP
#define BIT_MEMORY_UTILITIES_DETAIL_EBO_STORAGE_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <cstddef>     // std::size_t
#include <tuple>       // std::tuple_size
#include <type_traits> // std::integral_constant, std::enable_if, etc...
#include <utility>     // std::index_sequence, std::piecewise_construct_t

namespace bit {
  namespace memory {

    /////////////////////////////////////////////////////////////////////////
    /// \brief A utility class used for leveraging empty-base
    ///        optimization for a generic sequence of types.
    ///
    /// If a type appears twice or more in the type list, only one instance
    /// will be inherited from, and the remainder will be compositely
    /// created.
    ///
    /// Member access can be done through the free-function, \c get, that
    /// accepts an index for the EBO type. In effect, this class is a
    /// compressed_tuple without the additional comparators.
    ///
    /// \tparam Ts the types for this ebo storage
    /////////////////////////////////////////////////////////////////////////
    template<typename...Ts>
    class ebo_storage;

    //-------------------------------------------------------------------------
    // Utilities
    //-------------------------------------------------------------------------

    /// \{
    /// \brief Gets the nth element from the ebo storage
    ///
    /// The reference returned preserves the cv and rvalue-ness of the storage
    ///
    /// It is undefined behaviour to invoke this with an invalid index
    ///
    /// \tparam Idx the index of the element to access
    /// \return reference to the element
    template<std::size_t Idx,typename...Types>
    std::tuple_element_t<Idx,std::tuple<Types...>>&
      get( ebo_storage<Types...>& ebo );
    template<std::size_t Idx,typename...Types>
    std::tuple_element_t<Idx,const std::tuple<Types...>>&
      get( const ebo_storage<Types...>& ebo );
    template<std::size_t Idx,typename...Types>
    std::tuple_element_t<Idx,std::tuple<Types...>>&&
      get( ebo_storage<Types...>&& ebo );
    template<std::size_t Idx,typename...Types>
    std::tuple_element_t<Idx,const std::tuple<Types...>>&&
      get( const ebo_storage<Types...>&& ebo );
    /// \}

    //=========================================================================
    // Implementation
    //=========================================================================

    namespace detail {

      template<typename T, typename...Ts>
      struct is_duplicate;

      template<typename T, typename...Ts>
      struct is_duplicate<T,T,Ts...> : std::true_type{};

      template<typename T, typename T0, typename...Ts>
      struct is_duplicate<T,T0,Ts...> : is_duplicate<T,Ts...>{};

      template<typename T>
      struct is_duplicate<T> : std::false_type{};

      template<typename T0, typename...Ts>
      struct can_ebo
        : std::integral_constant<bool,std::is_class<T0>::value &&
                                      std::is_empty<T0>::value &&
                                     !std::is_final<T0>::value &&
                                     !is_duplicate<T0,Ts...>::value>{};

      //=======================================================================
      // ebo_storage_impl
      //=======================================================================

      template<std::size_t Idx, typename T0, bool CanEbo, typename...Ts>
      class ebo_storage_impl;

      //-----------------------------------------------------------------------

      template<std::size_t Idx, typename T0, typename T1, typename...Ts>
      class ebo_storage_impl<Idx,T0,true,T1,Ts...>
        : private T0,
          public ebo_storage_impl<Idx+1,T1,can_ebo<T1,Ts...>::value, Ts...>
      {
        using base_type = detail::ebo_storage_impl<Idx+1,T1,can_ebo<T1,Ts...>::value, Ts...>;

        //---------------------------------------------------------------------
        // Constructors
        //---------------------------------------------------------------------
      protected:

        ebo_storage_impl() = default;
        template<typename Tuple, typename...Tuples, typename = std::enable_if_t<!std::is_same<std::decay_t<Tuple>,ebo_storage_impl>::value>>
        ebo_storage_impl( Tuple&& tuple, Tuples&&...tuples );
        ebo_storage_impl( const ebo_storage_impl& other ) = default;
        ebo_storage_impl( ebo_storage_impl&& other ) = default;

        //---------------------------------------------------------------------

        ebo_storage_impl& operator=( const ebo_storage_impl& other ) = default;
        ebo_storage_impl& operator=( ebo_storage_impl&& other ) = default;

        //---------------------------------------------------------------------
        // Observers
        //---------------------------------------------------------------------
      protected:

        using base_type::get;

        T0& get( std::integral_constant<std::size_t,Idx> ) &;
        const T0& get( std::integral_constant<std::size_t,Idx> ) const &;
        T0&& get( std::integral_constant<std::size_t,Idx> ) &&;
        const T0&& get( std::integral_constant<std::size_t,Idx> ) const &&;

        //---------------------------------------------------------------------
        // Private Constructors
        //---------------------------------------------------------------------
      private:

        template<typename Tuple, std::size_t...Idxs, typename...Tuples>
        ebo_storage_impl( Tuple&& tuple, std::index_sequence<Idxs...>, Tuples&&...tuples );
      };

      //-----------------------------------------------------------------------

      template<std::size_t Idx, typename T0>
      class ebo_storage_impl<Idx,T0,true>
        : private T0
      {

      protected:

        ebo_storage_impl() = default;
        template<typename Tuple, typename = std::enable_if_t<!std::is_same<std::decay_t<Tuple>,ebo_storage_impl>::value>>
        ebo_storage_impl( Tuple&& tuple );
        ebo_storage_impl( const ebo_storage_impl& other ) = default;
        ebo_storage_impl( ebo_storage_impl&& other ) = default;

        ebo_storage_impl& operator=( const ebo_storage_impl& other ) = default;
        ebo_storage_impl& operator=( ebo_storage_impl&& other ) = default;

        //---------------------------------------------------------------------
        // Protected Members
        //---------------------------------------------------------------------
      protected:

        T0& get( std::integral_constant<std::size_t,Idx> ) &;
        const T0& get( std::integral_constant<std::size_t,Idx> ) const &;
        T0&& get( std::integral_constant<std::size_t,Idx> ) &&;
        const T0&& get( std::integral_constant<std::size_t,Idx> ) const &&;

        //---------------------------------------------------------------------
        // Private Constructors
        //---------------------------------------------------------------------
      private:

        template<typename Tuple, std::size_t...Idxs>
        ebo_storage_impl( Tuple&& tuple, std::index_sequence<Idxs...> );
      };

      //-----------------------------------------------------------------------

      template<std::size_t Idx, typename T0, typename T1, typename...Ts>
      class ebo_storage_impl<Idx,T0,false,T1,Ts...>
        : public ebo_storage_impl<Idx+1,T1,can_ebo<T1,Ts...>::value, Ts...>
      {
        using base_type = detail::ebo_storage_impl<Idx+1,T1,can_ebo<T1,Ts...>::value, Ts...>;

        //---------------------------------------------------------------------
        // Protected Constructors / Assignment
        //---------------------------------------------------------------------
      protected:

        ebo_storage_impl() = default;
        template<typename Tuple, typename...Tuples, typename = std::enable_if_t<!std::is_same<std::decay_t<Tuple>,ebo_storage_impl>::value>>
        ebo_storage_impl( Tuple&& tuple, Tuples&&...tuples );
        ebo_storage_impl( const ebo_storage_impl& other ) = default;
        ebo_storage_impl( ebo_storage_impl&& other ) = default;

        //---------------------------------------------------------------------

        ebo_storage_impl& operator=( const ebo_storage_impl& other ) = default;
        ebo_storage_impl& operator=( ebo_storage_impl&& other ) = default;

        //---------------------------------------------------------------------
        // Protected Observers
        //---------------------------------------------------------------------
      protected:

        using base_type::get;

        T0& get( std::integral_constant<std::size_t,Idx> ) &;
        const T0& get( std::integral_constant<std::size_t,Idx> ) const &;
        T0&& get( std::integral_constant<std::size_t,Idx> ) &&;
        const T0&& get( std::integral_constant<std::size_t,Idx> ) const &&;

        //---------------------------------------------------------------------
        // Private Constructors
        //---------------------------------------------------------------------
      private:

        template<typename Tuple, std::size_t...Idxs, typename...Tuples>
        ebo_storage_impl( Tuple&& tuple, std::index_sequence<Idxs...>, Tuples&&...tuples);

        //---------------------------------------------------------------------
        // Private Members
        //---------------------------------------------------------------------
      private:

        T0 m_storage;
      };

      //-----------------------------------------------------------------------

      template<std::size_t Idx, typename T0>
      class ebo_storage_impl<Idx,T0,false>
      {
        //---------------------------------------------------------------------
        // Protected Constructors / Assignment
        //---------------------------------------------------------------------
      protected:

        ebo_storage_impl() = default;
        template<typename Tuple, typename = std::enable_if_t<!std::is_same<std::decay_t<Tuple>,ebo_storage_impl>::value>>
        ebo_storage_impl( Tuple&& tuple );
        ebo_storage_impl( const ebo_storage_impl& other ) = default;
        ebo_storage_impl( ebo_storage_impl&& other ) = default;

        //---------------------------------------------------------------------

        ebo_storage_impl& operator=( const ebo_storage_impl& other ) = default;
        ebo_storage_impl& operator=( ebo_storage_impl&& other ) = default;

        //---------------------------------------------------------------------
        // Protected Observers
        //---------------------------------------------------------------------
      protected:

        T0& get( std::integral_constant<std::size_t,Idx> ) &;
        const T0& get( std::integral_constant<std::size_t,Idx> ) const &;
        T0&& get( std::integral_constant<std::size_t,Idx> ) &&;
        const T0&& get( std::integral_constant<std::size_t,Idx> ) const &&;

        //---------------------------------------------------------------------
        // Private Constructors
        //---------------------------------------------------------------------
      private:

        template<typename Tuple, std::size_t...Idxs>
        ebo_storage_impl( Tuple&& tuple, std::index_sequence<Idxs...> );

        //---------------------------------------------------------------------
        // Private Members
        //---------------------------------------------------------------------
      private:

        T0 m_storage;
      };
    } // namespace detail

    //=========================================================================
    // base case: no members, no functionality
    //=========================================================================

    template<>
    class ebo_storage<>
    {
      //-----------------------------------------------------------------------
      // Constructors
      //-----------------------------------------------------------------------
    public:

      ebo_storage() = default;

      ebo_storage( const ebo_storage& other ) = default;

      ebo_storage( ebo_storage&& other ) = default;

      ebo_storage& operator=( const ebo_storage& other ) = default;

      ebo_storage& operator=( ebo_storage&& other ) = default;
    };

    //=========================================================================
    // standard case, 1+ types
    //=========================================================================

    template<typename T0, typename...Ts>
    class ebo_storage<T0,Ts...>
    : public detail::ebo_storage_impl<0,T0,detail::can_ebo<T0,Ts...>::value,Ts...>
    {

      using base_type = detail::ebo_storage_impl<0,T0,detail::can_ebo<T0,Ts...>::value,Ts...>;

      //-----------------------------------------------------------------------
      // Constructors
      //-----------------------------------------------------------------------
    public:

      ebo_storage() = default;

      template<typename...Tuples, typename = std::enable_if_t<sizeof...(Ts)+1==sizeof...(Tuples)>>
      explicit ebo_storage( Tuples&&...tuples );

      ebo_storage( const ebo_storage& other ) = default;

      ebo_storage( ebo_storage&& other ) = default;

      //-----------------------------------------------------------------------

      ebo_storage& operator=( const ebo_storage& other ) = default;

      ebo_storage& operator=( ebo_storage&& other ) = default;

      //---------------------------------------------------------------------
      // Friends
      //---------------------------------------------------------------------
    private:

      template<std::size_t Idx, typename...Types>
      friend std::tuple_element_t<Idx,std::tuple<Types...>>&
       get( ebo_storage<Types...>& );

      template<std::size_t Idx, typename...Types>
      friend std::tuple_element_t<Idx,const std::tuple<Types...>>&
       get( const ebo_storage<Types...>& );

      template<std::size_t Idx, typename...Types>
      friend std::tuple_element_t<Idx,const std::tuple<Types...>>&&
       get( ebo_storage<Types...>&& );

      template<std::size_t Idx, typename...Types>
      friend std::tuple_element_t<Idx,const std::tuple<Types...>>&&
       get( const ebo_storage<Types...>&& );
    };

  } // namespace memory
} // namespace bit

#include "detail/ebo_storage.inl"

#endif /* BIT_MEMORY_UTILITIES_DETAIL_EBO_STORAGE_HPP */
