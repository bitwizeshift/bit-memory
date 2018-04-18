/*****************************************************************************
 * \file
 * \brief Unit tests for the header 'endian.hpp'
 *****************************************************************************/


#include <bit/memory/utilities/endian.hpp>

#include <catch.hpp>

namespace {


  template<typename T>
  bool reverse_equals( const T& lhs, const T& rhs )
  {
    using byte_t = unsigned char;

    auto lhs_p = reinterpret_cast<const byte_t*>(&lhs);
    auto rhs_p = reinterpret_cast<const byte_t*>(&rhs);

    for( auto i = 0u; i < sizeof(T); ++i ) {
      if( lhs_p[i] != rhs_p[sizeof(T)-i-1] ) {
        return false;
      }
    }
    return true;
  }

} // anonymous namespace

//-----------------------------------------------------------------------------
// endian_swap( T ) : sizeof(T) == 1
//-----------------------------------------------------------------------------

TEST_CASE("endian_swap( char )")
{
  auto c = 'a';

  SECTION("Endian swapping char returns input")
  {
    auto result = bit::memory::endian_swap(c);

    REQUIRE( result == c );
  }
}


//-----------------------------------------------------------------------------

TEST_CASE("endian_swap( std::int8_t )")
{
  SECTION("Negative Number")
  {
    SECTION("Endian swapping byte returns input")
    {
      auto i = std::int8_t(10);

      auto result = bit::memory::endian_swap(i);

      REQUIRE( result == i );
    }
  }

  SECTION("Positive Number")
  {
    SECTION("Endian swapping byte returns input")
    {
      auto i = std::int8_t(-10);

      auto result = bit::memory::endian_swap(i);

      REQUIRE( result == i );
    }
  }
}

TEST_CASE("endian_swap( std::uint8_t )")
{
  auto u = std::uint8_t(10);

  SECTION("Endian swapping byte returns input")
  {
    auto result = bit::memory::endian_swap(u);

    REQUIRE( result == u );
  }
}

//-----------------------------------------------------------------------------
// endian_swap( T ) : sizeof(T) == 2
//-----------------------------------------------------------------------------

TEST_CASE("endian_swap( std::int16_t )")
{
  SECTION("Negative Number")
  {
    SECTION("Reverses input byte stream")
    {
      auto i = std::int16_t(0xdead);

      auto result = bit::memory::endian_swap(i);

      REQUIRE( reverse_equals(i,result) );
    }
  }

  SECTION("Positive Number")
  {
    SECTION("Reverses input byte stream")
    {
      auto i = std::int16_t(-0xdead);

      auto result = bit::memory::endian_swap(i);

      REQUIRE( reverse_equals(i,result) );
    }
  }
}

//-----------------------------------------------------------------------------

TEST_CASE("endian_swap( std::uint16_t )")
{
  auto u = std::uint16_t(0xdead);

  SECTION("Reverses input byte stream")
  {
    auto result = bit::memory::endian_swap(u);

    REQUIRE( reverse_equals(u,result) );
  }
}

//-----------------------------------------------------------------------------
// endian_swap( T ) : sizeof(T) == 4
//-----------------------------------------------------------------------------

TEST_CASE("endian_swap( std::int32_t )")
{
  SECTION("Negative Number")
  {
    SECTION("Reverses input byte stream")
    {
      auto i = std::int32_t(0xdeadbeef);

      auto result = bit::memory::endian_swap(i);

      REQUIRE( reverse_equals(i,result) );
    }
  }

  SECTION("Positive Number")
  {
    SECTION("Reverses input byte stream")
    {
      auto i = std::int32_t(-0xdeadbeef);

      auto result = bit::memory::endian_swap(i);

      REQUIRE( reverse_equals(i,result) );
    }
  }
}

//-----------------------------------------------------------------------------

TEST_CASE("endian_swap( std::uint32_t )")
{
  auto u = std::uint32_t(0xdeadbeef);

  SECTION("Reverses input byte stream")
  {
    auto result = bit::memory::endian_swap(u);

    REQUIRE( reverse_equals(u,result) );
  }
}

//-----------------------------------------------------------------------------
// endian_swap( T ) : sizeof(T) == 8
//-----------------------------------------------------------------------------

TEST_CASE("endian_swap( std::int64_t )")
{
  SECTION("Negative Number")
  {
    SECTION("Reverses input byte stream")
    {
      auto i = std::int64_t(0x123456789abcdefll);

      auto result = bit::memory::endian_swap(i);

      REQUIRE( reverse_equals(i,result) );
    }
  }

  SECTION("Positive Number")
  {
    SECTION("Reverses input byte stream")
    {
      auto i = std::int64_t(-0x123456789abcdefll);

      auto result = bit::memory::endian_swap(i);

      REQUIRE( reverse_equals(i,result) );
    }
  }
}

//-----------------------------------------------------------------------------

TEST_CASE("endian_swap( std::uint64_t )")
{
  auto u = std::uint64_t(0x123456789abcdefll);

  SECTION("Reverses input byte stream")
  {
    auto result = bit::memory::endian_swap(u);

    REQUIRE( reverse_equals(u,result) );
  }
}

//-----------------------------------------------------------------------------
// endian_swap( T ) : std::is_floating_point<T>::value == true
//-----------------------------------------------------------------------------

TEST_CASE("endian_swap( float )")
{
  SECTION("Reverses input byte stream")
  {
    auto f = 0.1f;
    auto result = bit::memory::endian_swap(f);

    REQUIRE( reverse_equals(f,result) );
  }
}

TEST_CASE("endian_swap( double )")
{
  SECTION("Reverses input byte stream")
  {
    auto d = 0.1;
    auto result = bit::memory::endian_swap(d);

    REQUIRE( reverse_equals(d,result) );
  }
}

//-----------------------------------------------------------------------------
// endian_cast
//-----------------------------------------------------------------------------

// All swaps to the same endianness return the input
TEST_CASE("endian_cast<endian::native>( T )")
{
  using namespace bit::memory::casts;
  using bit::memory::endian;

  SECTION("endian_cast<endian::native>( char )")
  {
    SECTION("Returns input")
    {
      auto c = 'a';

      auto result = endian_cast<endian::native>(c);

      REQUIRE( c == result );
    }
  }

  SECTION("endian_cast<endian::native>( std::int8_t )")
  {
    SECTION("Returns input")
    {
      auto i = std::int8_t(-42);

      auto result = endian_cast<endian::native>(i);

      REQUIRE( i == result );
    }
  }

  SECTION("endian_cast<endian::native>( std::uint8_t )")
  {
    SECTION("Returns input")
    {
      auto u = std::uint8_t(42);

      auto result = endian_cast<endian::native>(u);

      REQUIRE( u == result );
    }
  }

  //--------------------------------------------------------------------------

  SECTION("endian_cast<endian::native>( std::int16_t )")
  {
    SECTION("Returns input")
    {
      auto i = std::uint16_t(0xdeadu);

      auto result = endian_cast<endian::native>(i);

      REQUIRE( i == result );
    }
  }

  SECTION("endian_cast<endian::native>( std::uint16_t )")
  {
    SECTION("Returns input")
    {
      auto u = std::uint16_t(0xdeadu);

      auto result = endian_cast<endian::native>(u);

      REQUIRE( u == result );
    }
  }

  //--------------------------------------------------------------------------

  SECTION("endian_cast<endian::native>( std::int32_t )")
  {
    SECTION("Returns input")
    {
      auto i = std::int32_t(0xdeadbeef);

      auto result = endian_cast<endian::native>(i);

      REQUIRE( i == result );
    }
  }

  SECTION("endian_cast<endian::native>( std::uint32_t )")
  {
    SECTION("Returns input")
    {
      auto u = std::uint32_t(0xdeadbeefu);

      auto result = endian_cast<endian::native>(u);

      REQUIRE( u == result );
    }
  }

  //--------------------------------------------------------------------------

  SECTION("endian_cast<endian::native>( std::int64_t )")
  {
    SECTION("Returns input")
    {
      auto i = std::int16_t(-0x123456789abcdefll);

      auto result = endian_cast<endian::native>(i);

      REQUIRE( i == result );
    }
  }

  SECTION("endian_cast<endian::native>( std::uint64_t )")
  {
    SECTION("Returns input")
    {
      auto u = std::uint16_t(0x123456789abcdefull);

      auto result = endian_cast<endian::native>(u);

      REQUIRE( u == result );
    }
  }

  //--------------------------------------------------------------------------

  SECTION("endian_cast<endian::native>( float )")
  {
    SECTION("Returns input")
    {
      auto f = 0.1f;

      auto result = endian_cast<endian::native>(f);

      REQUIRE( f == result );
    }
  }

  SECTION("endian_cast<endian::native>( double )")
  {
    SECTION("Returns input")
    {
      auto d = 0.1;

      auto result = endian_cast<endian::native>(d);

      REQUIRE( d == result );
    }
  }
}

namespace {
  template<bit::memory::endian E>
  struct other_endianness{ static constexpr auto value = bit::memory::endian::big; };

  template<>
  struct other_endianness<bit::memory::endian::big>{ static constexpr auto value = bit::memory::endian::little; };

} // anonymous namespace

//-----------------------------------------------------------------------------

TEST_CASE("endian_cast<other_endian>( T )")
{
  using namespace bit::memory::casts;
  static constexpr auto other_endian = other_endianness<bit::memory::endian::native>::value;

  SECTION("endian_cast<other_endian>( char )")
  {
    SECTION("Returns input")
    {
      auto c = 'a';

      auto result = endian_cast<other_endian>(c);

      REQUIRE( c == result );
    }
  }

  SECTION("endian_cast<other_endian>( std::int8_t )")
  {
    SECTION("Returns input")
    {
      auto i = std::int8_t(-42);

      auto result = endian_cast<other_endian>(i);

      REQUIRE( i == result );
    }
  }

  SECTION("endian_cast<other_endian>( std::uint8_t )")
  {
    SECTION("Returns input")
    {
      auto u = std::uint8_t(42);

      auto result = endian_cast<other_endian>(u);

      REQUIRE( u == result );
    }
  }

  //--------------------------------------------------------------------------

  SECTION("endian_cast<other_endian>( std::int16_t )")
  {
    SECTION("Reverses input byte stream")
    {
      auto i = std::uint16_t(0xdeadu);

      auto result = endian_cast<other_endian>(i);

      REQUIRE( reverse_equals(i,result) );
    }
  }

  SECTION("endian_cast<other_endian>( std::uint16_t )")
  {
    SECTION("Reverses input byte stream")
    {
      auto u = std::uint16_t(0xdeadu);

      auto result = endian_cast<other_endian>(u);

      REQUIRE( reverse_equals(u,result) );
    }
  }

  //--------------------------------------------------------------------------

  SECTION("endian_cast<other_endian>( std::int32_t )")
  {
    SECTION("Reverses input byte stream")
    {
      auto i = std::int32_t(0xdeadbeef);

      auto result = endian_cast<other_endian>(i);

      REQUIRE( reverse_equals(i,result) );
    }
  }

  SECTION("endian_cast<other_endian>( std::uint32_t )")
  {
    SECTION("Reverses input byte stream")
    {
      auto u = std::uint32_t(0xdeadbeefu);

      auto result = endian_cast<other_endian>(u);

      REQUIRE( reverse_equals(u,result) );
    }
  }

  //--------------------------------------------------------------------------

  SECTION("endian_cast<other_endian>( std::int64_t )")
  {
    SECTION("Reverses input byte stream")
    {
      auto i = std::int16_t(-0x123456789abcdefll);

      auto result = endian_cast<other_endian>(i);

      REQUIRE( reverse_equals(i,result) );
    }
  }

  SECTION("endian_cast<other_endian>( std::uint64_t )")
  {
    SECTION("Reverses input byte stream")
    {
      auto u = std::uint16_t(0x123456789abcdefull);

      auto result = endian_cast<other_endian>(u);

      REQUIRE( reverse_equals(u,result) );
    }
  }

  //--------------------------------------------------------------------------

  SECTION("endian_cast<other_endian>( float )")
  {
    SECTION("Reverses input byte stream")
    {
      auto f = 0.1f;

      auto result = endian_cast<other_endian>(f);

      REQUIRE( reverse_equals(f,result) );
    }
  }

  SECTION("endian_cast<other_endian>( double )")
  {
    SECTION("Reverses input byte stream")
    {
      auto d = 0.1;

      auto result = endian_cast<other_endian>(d);

      REQUIRE( reverse_equals(d,result) );
    }
  }
}
