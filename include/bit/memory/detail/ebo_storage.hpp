/**
 * \file ebo_storage.hpp
 *
 * \brief This header contains an internal-only implementation of an EBO
 *        helper to reduce class-sizes.
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */
#ifndef BIT_MEMORY_DETAIL_EBO_STORAGE_HPP
#define BIT_MEMORY_DETAIL_EBO_STORAGE_HPP

#include <type_traits> // std::integral_constant, std::enable_if, etc...
#include <tuple>       // std::tuple_size
#include <utility>     // std::index_sequence, std::piecewise_construct_t

namespace bit {
  namespace memory {
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
      struct can_ebo : std::integral_constant<bool,std::is_class<T0>::value && !std::is_final<T0>::value && !is_duplicate<T0,Ts...>::value>{};

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
        using base_type = ebo_storage_impl<Idx+1,T1,can_ebo<T1,Ts...>::value, Ts...>;

      protected:

        ebo_storage_impl() = default;

        ebo_storage_impl( const ebo_storage_impl& other ) = default;

        ebo_storage_impl( ebo_storage_impl&& other ) = default;

        ebo_storage_impl& operator=( const ebo_storage_impl& other ) = default;

        ebo_storage_impl& operator=( ebo_storage_impl&& other ) = default;

        template<typename Tuple, typename...Tuples, typename = std::enable_if_t<!std::is_same<std::decay_t<Tuple>,ebo_storage_impl>::value>>
        ebo_storage_impl( Tuple&& tuple, Tuples&&...tuples )
          : ebo_storage_impl( std::forward<Tuple>(tuple),
                              std::make_index_sequence<std::tuple_size<std::decay_t<Tuple>>::value>{},
                              std::forward<Tuples>(tuples)... )
        {

        }

        using base_type::get;

        T0& get( std::integral_constant<std::size_t,Idx> ) &
        {
          return static_cast<T0&>(*this);
        }

        const T0& get( std::integral_constant<std::size_t,Idx> ) const &
        {
          return static_cast<const T0&>(*this);
        }

        T0&& get( std::integral_constant<std::size_t,Idx> ) &&
        {
          return static_cast<T0&&>(*this);
        }

        const T0&& get( std::integral_constant<std::size_t,Idx> ) const &&
        {
          return static_cast<const T0&&>(*this);
        }

      private:

        template<typename Tuple, std::size_t...Idxs, typename...Tuples>
        ebo_storage_impl( Tuple&& tuple, std::index_sequence<Idxs...>, Tuples&&...tuples )
          : T0( std::get<Idxs>( std::forward<Tuple>(tuple) )... ),
            base_type( std::forward<Tuples>(tuples)... )
        {

        }
      };

      //-----------------------------------------------------------------------

      template<std::size_t Idx, typename T0>
      class ebo_storage_impl<Idx,T0,true>
        : private T0
      {

      protected:

        ebo_storage_impl() = default;

        ebo_storage_impl( const ebo_storage_impl& other ) = default;

        ebo_storage_impl( ebo_storage_impl&& other ) = default;

        ebo_storage_impl& operator=( const ebo_storage_impl& other ) = default;

        ebo_storage_impl& operator=( ebo_storage_impl&& other ) = default;

        template<typename Tuple, typename = std::enable_if_t<!std::is_same<std::decay_t<Tuple>,ebo_storage_impl>::value>>
        ebo_storage_impl( Tuple&& tuple )
          : ebo_storage_impl( std::forward<Tuple>(tuple),
                              std::make_index_sequence<std::tuple_size<std::decay_t<Tuple>>::value>{} )
        {

        }

        T0& get( std::integral_constant<std::size_t,Idx> ) &
        {
          return static_cast<T0&>(*this);
        }

        const T0& get( std::integral_constant<std::size_t,Idx> ) const &
        {
          return static_cast<const T0&>(*this);
        }

        T0&& get( std::integral_constant<std::size_t,Idx> ) &&
        {
          return static_cast<T0&&>(*this);
        }

        const T0&& get( std::integral_constant<std::size_t,Idx> ) const &&
        {
          return static_cast<const T0&&>(*this);
        }

      private:

        template<typename Tuple, std::size_t...Idxs>
        ebo_storage_impl( Tuple&& tuple, std::index_sequence<Idxs...> )
          : T0( std::get<Idxs>( std::forward<Tuple>(tuple) )... )
        {

        }
      };

      //-----------------------------------------------------------------------

      template<std::size_t Idx, typename T0, typename T1, typename...Ts>
      class ebo_storage_impl<Idx,T0,false,T1,Ts...>
        : public ebo_storage_impl<Idx+1,T1,can_ebo<T1,Ts...>::value, Ts...>
      {
        using base_type = ebo_storage_impl<Idx+1,T1,can_ebo<T1,Ts...>::value, Ts...>;

      protected:

        ebo_storage_impl() = default;

        ebo_storage_impl( const ebo_storage_impl& other ) = default;

        ebo_storage_impl( ebo_storage_impl&& other ) = default;

        ebo_storage_impl& operator=( const ebo_storage_impl& other ) = default;

        ebo_storage_impl& operator=( ebo_storage_impl&& other ) = default;

        template<typename Tuple, typename...Tuples, typename = std::enable_if_t<!std::is_same<std::decay_t<Tuple>,ebo_storage_impl>::value>>
        ebo_storage_impl( Tuple&& tuple, Tuples&&...tuples )
          : ebo_storage_impl( std::forward<Tuple>(tuple),
                              std::make_index_sequence<std::tuple_size<std::decay_t<Tuple>>::value>{},
                              std::forward<Tuples>(tuples)... )
        {

        }

        using base_type::get;

        T0& get( std::integral_constant<std::size_t,Idx> ) &
        {
          return m_storage;
        }

        const T0& get( std::integral_constant<std::size_t,Idx> ) const &
        {
          return m_storage;
        }

        T0&& get( std::integral_constant<std::size_t,Idx> ) &&
        {
          return static_cast<T0&&>(m_storage);
        }

        const T0&& get( std::integral_constant<std::size_t,Idx> ) const &&
        {
          return static_cast<const T0&&>(m_storage);
        }

      private:

        template<typename Tuple, std::size_t...Idxs, typename...Tuples>
        ebo_storage_impl( Tuple&& tuple, std::index_sequence<Idxs...>, Tuples&&...tuples)
        : base_type( std::forward<Tuples>(tuples)... ),
          m_storage( std::get<Idxs>( std::forward<Tuple>(tuple) )... )
        {

        }

        T0 m_storage;
      };

      //-----------------------------------------------------------------------

      template<std::size_t Idx, typename T0>
      class ebo_storage_impl<Idx,T0,false>
      {
      protected:

        ebo_storage_impl() = default;

        ebo_storage_impl( const ebo_storage_impl& other ) = default;

        ebo_storage_impl( ebo_storage_impl&& other ) = default;

        ebo_storage_impl& operator=( const ebo_storage_impl& other ) = default;

        ebo_storage_impl& operator=( ebo_storage_impl&& other ) = default;

        template<typename Tuple, typename = std::enable_if_t<!std::is_same<std::decay_t<Tuple>,ebo_storage_impl>::value>>
        ebo_storage_impl( Tuple&& tuple )
          : ebo_storage_impl( std::forward<Tuple>(tuple),
                              std::make_index_sequence<std::tuple_size<std::decay_t<Tuple>>::value>{} )
        {

        }

        T0& get( std::integral_constant<std::size_t,Idx> ) &
        {
          return m_storage;
        }

        const T0& get( std::integral_constant<std::size_t,Idx> ) const &
        {
          return m_storage;
        }

        T0&& get( std::integral_constant<std::size_t,Idx> ) &&
        {
          return static_cast<T0&&>(m_storage);
        }

        const T0&& get( std::integral_constant<std::size_t,Idx> ) const &&
        {
          return static_cast<const T0&&>(m_storage);
        }

      private:

        template<typename Tuple, std::size_t...Idxs>
        ebo_storage_impl( Tuple&& tuple, std::index_sequence<Idxs...> )
          : m_storage( std::get<Idxs>( std::forward<Tuple>(tuple) )... )
        {

        }

        T0 m_storage;
      };

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

      // base case: no members, no functionality

      template<>
      class ebo_storage<>
      {
        //---------------------------------------------------------------------
        // Constructors
        //---------------------------------------------------------------------
      public:

        ebo_storage() = default;

        ebo_storage( const ebo_storage& other ) = default;

        ebo_storage( ebo_storage&& other ) = default;

        ebo_storage& operator=( const ebo_storage& other ) = default;

        ebo_storage& operator=( ebo_storage&& other ) = default;
      };

      // standard case, 1-n types

      template<typename T0, typename...Ts>
      class ebo_storage<T0,Ts...>
        : public ebo_storage_impl<0,T0,can_ebo<T0,Ts...>::value,Ts...>
      {

        using base_type = ebo_storage_impl<0,T0,can_ebo<T0,Ts...>::value,Ts...>;

        //---------------------------------------------------------------------
        // Constructors
        //---------------------------------------------------------------------
      public:

        ebo_storage() = default;

        ebo_storage( const ebo_storage& other ) = default;

        ebo_storage( ebo_storage&& other ) = default;

        ebo_storage& operator=( const ebo_storage& other ) = default;

        ebo_storage& operator=( ebo_storage&& other ) = default;

        template<typename...Tuples, typename = std::enable_if_t<sizeof...(Ts)+1==sizeof...(Tuples)>>
        ebo_storage( Tuples&&...tuples )
          : base_type( std::forward<Tuples>(tuples)... )
        {

        }

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

      //-----------------------------------------------------------------------
      // Utilities
      //-----------------------------------------------------------------------

      template<std::size_t Idx,typename...Types>
      std::tuple_element_t<Idx,std::tuple<Types...>>&
        get( ebo_storage<Types...>& ebo )
      {
        return ebo.get( std::integral_constant<std::size_t,Idx>{} );
      }

      template<std::size_t Idx,typename...Types>
      std::tuple_element_t<Idx,const std::tuple<Types...>>&
        get( const ebo_storage<Types...>& ebo )
      {
        return ebo.get( std::integral_constant<std::size_t,Idx>{} );
      }

      template<std::size_t Idx,typename...Types>
      std::tuple_element_t<Idx,std::tuple<Types...>>&&
        get( ebo_storage<Types...>&& ebo )
      {
        return ebo.get( std::integral_constant<std::size_t,Idx>{} );
      }

      template<std::size_t Idx,typename...Types>
      std::tuple_element_t<Idx,const std::tuple<Types...>>&&
        get( const ebo_storage<Types...>&& ebo )
      {
        return ebo.get( std::integral_constant<std::size_t,Idx>{} );
      }

    } // namespace detail
  } // namespace memory
} // namespace bit

#endif /* BIT_MEMORY_DETAIL_EBO_STORAGE_HPP */
