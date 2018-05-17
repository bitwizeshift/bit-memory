/*****************************************************************************
 * \file
 * \brief Unit tests for the malloc_block_allocator
 *****************************************************************************/


#include <bit/memory/block_allocators/malloc_block_allocator.hpp>
#include <bit/memory/concepts/Stateless.hpp>
#include <bit/memory/concepts/BlockAllocator.hpp>

#include <bit/memory/policies/growth_multipliers/power_two_growth.hpp>

#include <catch.hpp>

#include <cstring> // std::memset
#include <array>

//=============================================================================
// Static Requirements
//=============================================================================

using static_type              = bit::memory::malloc_block_allocator<64>;
using named_static_type        = bit::memory::named_malloc_block_allocator<64>;
using cached_static_type       = bit::memory::cached_malloc_block_allocator<64>;
using named_cached_static_type = bit::memory::named_cached_malloc_block_allocator<64>;

//=============================================================================

static_assert( bit::memory::is_block_allocator<static_type>::value,
               "static malloc block allocator must be a block allocator" );

static_assert( bit::memory::is_block_allocator<named_static_type>::value,
               "named static malloc block allocator must be a block allocator" );

static_assert( bit::memory::is_block_allocator<cached_static_type>::value,
               "cached static malloc allocator must be a block allocator" );

static_assert( bit::memory::is_block_allocator<named_cached_static_type>::value,
               "named cached static malloc block allocator must be a block allocator" );

//=============================================================================

#ifndef _MSC_VER // MSVC fails to determine that this code is actually stateless

static_assert( bit::memory::is_stateless<static_type>::value,
               "static malloc block allocator must be stateless" );

#endif

static_assert( !bit::memory::is_stateless<named_static_type>::value,
               "named static malloc block allocator cannot be stateless" );

static_assert( !bit::memory::is_stateless<cached_static_type>::value,
               "cached static malloc allocator cannot be stateless" );

static_assert( !bit::memory::is_stateless<named_cached_static_type>::value,
               "named cached static malloc block allocator cannot be stateless" );

//=============================================================================
// Unit Tests
//=============================================================================

//-----------------------------------------------------------------------------
// malloc_block_allocator<1024>
//-----------------------------------------------------------------------------

TEST_CASE("malloc_block_allocator<1024>" "[resource management]")
{
  static constexpr auto block_size = 1024;
  auto block_allocator = bit::memory::malloc_block_allocator<block_size>();

  //---------------------------------------------------------------------------

  SECTION("allocate_block with blocks available")
  {
    SECTION("Lists next_block_size as 'block_size'")
    {
      const auto size = block_allocator.next_block_size();

      REQUIRE( size == block_size );
    }

    SECTION("Allocates non-null block")
    {
      auto block = block_allocator.allocate_block();

      auto success = block != bit::memory::nullblock;
      REQUIRE( success );

      block_allocator.deallocate_block( block );
    }
  }

  //---------------------------------------------------------------------------

  SECTION("allocate_block creates read/writeable range of memory")
  {
    const auto block = block_allocator.allocate_block();
    auto start = static_cast<unsigned char*>(block.data());
    auto end   = static_cast<unsigned char*>(block.data()) + block.size();

    // test write
    std::memset( block.data(), 0x01, block.size() );

    // test read
    auto sum = 0u;
    for( ; start != end; ++start ) sum += *start;

    REQUIRE( sum == block.size() );

    block_allocator.deallocate_block( block );
  }
}

//-----------------------------------------------------------------------------
// cached_malloc_block_allocator<1024>
//-----------------------------------------------------------------------------

TEST_CASE("cached_malloc_block_allocator<1024>" "[resource management]")
{
  static constexpr auto block_size = 1024;
  auto block_allocator = bit::memory::cached_malloc_block_allocator<block_size>{};

  //---------------------------------------------------------------------------

  // block allocators will attempt to reuse deallocated blocks first, before
  // allocating a new block
  SECTION("allocate_block reuses previously deallocated block")
  {
    auto p1 = static_cast<void*>(nullptr);
    auto s1 = static_cast<std::size_t>(0);
    {
      auto block = block_allocator.allocate_block();
      p1 = block.data();
      s1 = block.size();
      block_allocator.deallocate_block( block );
    }

    SECTION("Lists next_block_size as 's1'")
    {
      const auto size = block_allocator.next_block_size();

      REQUIRE( size == s1 );
    }

    SECTION("Allocates a block")
    {
      const auto block = block_allocator.allocate_block();

      auto success = block != bit::memory::nullblock;
      REQUIRE( success );

      block_allocator.deallocate_block( block );
    }

    SECTION("Reuses previously allocated block")
    {
      const auto block = block_allocator.allocate_block();
      const auto p2 = block.data();
      const auto s2 = block.size();

      SECTION("Allocates same memory region")
      {
        REQUIRE( p1 == p2 );
      }

      SECTION("Allocates same block size")
      {
        REQUIRE( s1 == s2 );
      }

      block_allocator.deallocate_block( block );
    }
  }

  //---------------------------------------------------------------------------

  SECTION("allocate_block creates read/writeable range of memory")
  {
    const auto block = block_allocator.allocate_block();
    auto start = static_cast<unsigned char*>(block.data());
    auto end   = static_cast<unsigned char*>(block.data()) + block.size();

    // test write
    std::memset( block.data(), 0x01, block.size() );

    // test read
    auto sum = 0u;
    for( ; start != end; ++start ) sum += *start;

    REQUIRE( sum == block.size() );

    block_allocator.deallocate_block( block );
  }
}

//-----------------------------------------------------------------------------
// malloc_block_allocator<1024,power_of_two_growth>
//-----------------------------------------------------------------------------

TEST_CASE("malloc_block_allocator<block_size,power_of_two_growth>" "[resource management]")
{
  using growth_multiplier = bit::memory::power_two_growth;
  static constexpr auto block_size  = 1024;
  static constexpr auto growths     = 3u;
  auto multiplier = growth_multiplier(growths);
  auto block_allocator = bit::memory::malloc_block_allocator<block_size,growth_multiplier>{multiplier};

  //---------------------------------------------------------------------------

  SECTION("allocate_block with blocks available")
  {
    SECTION("Lists next_block_size as 'block_size'")
    {
      const auto size = block_allocator.next_block_size();

      REQUIRE( size == block_size );
    }

    SECTION("Allocates non-null block")
    {
      auto block = block_allocator.allocate_block();

      auto success = block != bit::memory::nullblock;
      REQUIRE( success );

      block_allocator.deallocate_block( block );
    }
  }

  //---------------------------------------------------------------------------

  SECTION("allocate_block grows with each allocation")
  {
    auto block = block_allocator.allocate_block();

    SECTION("Lists next_block_size as 2*'block_size'")
    {
      const auto size = block_allocator.next_block_size();

      REQUIRE( size == (2 * block_size) );
    }

    SECTION("Allocates non-null block")
    {
      auto block = block_allocator.allocate_block();

      auto success = block != bit::memory::nullblock;
      REQUIRE( success );

      block_allocator.deallocate_block( block );
    }

    block_allocator.deallocate_block(block);
  }

  //---------------------------------------------------------------------------

  SECTION("allocate_block caps size at growths ^ rate * block_size")
  {
    auto allocated_blocks = std::array<bit::memory::memory_block,growths>{};

    for( auto& block : allocated_blocks ) {
      block = block_allocator.allocate_block();
    }

    SECTION("Lists next_block_size as multiplyer*'block_size'")
    {
      const auto multiplyer = 1 << growths;
      const auto size       = block_allocator.next_block_size();

      REQUIRE( size == (multiplyer * block_size) );
    }

    SECTION("Allocates block")
    {
      auto block = block_allocator.allocate_block();

      SECTION("Block is non-null")
      {
        auto success = (block != bit::memory::nullblock);
        REQUIRE( success );
      }

      block_allocator.deallocate_block( block );
    }

    for( auto& block : allocated_blocks ) {
      block_allocator.deallocate_block(block);
    }
  }

  //---------------------------------------------------------------------------

  SECTION("allocate_block creates read/writeable range of memory")
  {
    const auto block = block_allocator.allocate_block();
    auto start = static_cast<unsigned char*>(block.data());
    auto end   = static_cast<unsigned char*>(block.data()) + block.size();

    // test write
    std::memset( block.data(), 0x01, block.size() );

    // test read
    auto sum = 0u;
    for( ; start != end; ++start ) sum += *start;

    REQUIRE( sum == block.size() );

    block_allocator.deallocate_block( block );
  }
}

//-----------------------------------------------------------------------------
// cached_malloc_block_allocator<1024>
//-----------------------------------------------------------------------------

TEST_CASE("cached_malloc_block_allocator<block_size,power_two_growth>" "[resource management]")
{
  using growth_multiplier = bit::memory::power_two_growth;
  static constexpr auto block_size  = 1024;
  static constexpr auto growths     = 3u;

  auto multiplier = growth_multiplier(growths);
  auto block_allocator = bit::memory::cached_malloc_block_allocator<block_size,growth_multiplier>{multiplier};

  //---------------------------------------------------------------------------

  // block allocators will attempt to reuse deallocated blocks first, before
  // allocating a new block
  SECTION("allocate_block reuses previously deallocated block")
  {
    auto p1 = static_cast<void*>(nullptr);
    auto s1 = static_cast<std::size_t>(0);
    {
      auto block = block_allocator.allocate_block();
      p1 = block.data();
      s1 = block.size();
      block_allocator.deallocate_block( block );
    }

    SECTION("Lists next_block_size as 's1'")
    {
      const auto size = block_allocator.next_block_size();

      REQUIRE( size == s1 );
    }

    SECTION("Allocates a block")
    {
      const auto block = block_allocator.allocate_block();

      auto success = block != bit::memory::nullblock;
      REQUIRE( success );

      block_allocator.deallocate_block( block );
    }

    SECTION("Reuses previously allocated block")
    {
      const auto block = block_allocator.allocate_block();
      const auto p2 = block.data();
      const auto s2 = block.size();

      SECTION("Allocates same memory region")
      {
        REQUIRE( p1 == p2 );
      }

      SECTION("Allocates same block size")
      {
        REQUIRE( s1 == s2 );
      }

      block_allocator.deallocate_block( block );
    }
  }

  //---------------------------------------------------------------------------

  SECTION("allocate_block creates read/writeable range of memory")
  {
    const auto block = block_allocator.allocate_block();
    auto start = static_cast<unsigned char*>(block.data());
    auto end   = static_cast<unsigned char*>(block.data()) + block.size();

    // test write
    std::memset( block.data(), 0x01, block.size() );

    // test read
    auto sum = 0u;
    for( ; start != end; ++start ) sum += *start;

    REQUIRE( sum == block.size() );

    block_allocator.deallocate_block( block );
  }
}
