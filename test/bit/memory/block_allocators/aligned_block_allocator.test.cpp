/**
 * \file aligned_block_allocator.test.cpp
 *
 * \brief Unit tests for the aligned_block_allocator
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */

#include <bit/memory/block_allocators/aligned_block_allocator.hpp>
#include <bit/memory/concepts/Stateless.hpp>
#include <bit/memory/concepts/BlockAllocator.hpp>
#include <bit/memory/pointer_utilities.hpp> // align_of

#include <catch.hpp>

#include <cstring> // std::memset

//=============================================================================
// Static Requirements
//=============================================================================

using static_type              = bit::memory::aligned_block_allocator<64,64>;
using named_static_type        = bit::memory::named_aligned_block_allocator<64,64>;
using cached_static_type       = bit::memory::cached_aligned_block_allocator<64,64>;
using named_cached_static_type = bit::memory::named_cached_aligned_block_allocator<64,64>;

using dynamic_type              = bit::memory::dynamic_aligned_block_allocator;
using named_dynamic_type        = bit::memory::named_dynamic_aligned_block_allocator;
using cached_dynamic_type       = bit::memory::cached_dynamic_aligned_block_allocator;
using named_cached_dynamic_type = bit::memory::named_cached_dynamic_aligned_block_allocator;

//=============================================================================

static_assert( bit::memory::is_block_allocator<static_type>::value,
               "static malloc block allocator must be a block allocator" );

static_assert( bit::memory::is_block_allocator<named_static_type>::value,
               "named static malloc block allocator must be a block allocator" );

static_assert( bit::memory::is_block_allocator<cached_static_type>::value,
               "cached static malloc allocator must be a block allocator" );

static_assert( bit::memory::is_block_allocator<named_cached_static_type>::value,
               "named cached static malloc block allocator must be a block allocator" );

//-----------------------------------------------------------------------------

static_assert( bit::memory::is_block_allocator<dynamic_type>::value,
               "dynamic malloc block allocator must be a block allocator" );

static_assert( bit::memory::is_block_allocator<named_dynamic_type>::value,
               "named dynamic malloc block allocator must be a block allocator");

static_assert( bit::memory::is_block_allocator<cached_dynamic_type>::value,
               "cached dynamic malloc block allocator must be a block allocator");

static_assert( bit::memory::is_block_allocator<named_cached_dynamic_type>::value,
               "named cached dynamic cached malloc block allocator must be a block allocator");

//=============================================================================

static_assert( bit::memory::is_stateless<static_type>::value,
               "static malloc block allocator must be stateless" );

static_assert( !bit::memory::is_stateless<named_static_type>::value,
               "named static malloc block allocator cannot be stateless" );

static_assert( !bit::memory::is_stateless<cached_static_type>::value,
               "cached static malloc allocator cannot be stateless" );

static_assert( !bit::memory::is_stateless<named_cached_static_type>::value,
               "named cached static malloc block allocator cannot be stateless" );

//-----------------------------------------------------------------------------

static_assert( !bit::memory::is_stateless<dynamic_type>::value,
               "dynamic malloc block allocator cannot be stateless" );

static_assert( !bit::memory::is_stateless<named_dynamic_type>::value,
               "named dynamic malloc block allocator cannot be stateless");

static_assert( !bit::memory::is_stateless<cached_dynamic_type>::value,
               "cached dynamic malloc block allocator cannot be stateless");

static_assert( !bit::memory::is_stateless<named_cached_dynamic_type>::value,
               "named cached dynamic cached malloc block allocator cannot be stateless");

//=============================================================================
// Unit Tests
//=============================================================================

//-----------------------------------------------------------------------------
// aligned_block_allocator<1024>
//-----------------------------------------------------------------------------

TEST_CASE("aligned_block_allocator<block_size,align>" "[resource management]")
{
  static constexpr auto block_size = 1024;
  static constexpr auto align      = 1024;
  auto block_allocator = bit::memory::aligned_block_allocator<block_size,align>();

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

    SECTION("Allocates block aligned to at least 'align'")
    {
      auto block = block_allocator.allocate_block();

      REQUIRE( bit::memory::align_of(block.data()) >= align );

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
  }
}

//-----------------------------------------------------------------------------
// cached_aligned_block_allocator<1024>
//-----------------------------------------------------------------------------

TEST_CASE("cached_aligned_block_allocator<block_size,align>" "[resource management]")
{
  static constexpr auto block_size = 1024;
  static constexpr auto align      = 1024;
  auto block_allocator = bit::memory::cached_aligned_block_allocator<block_size,align>{};

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

      REQUIRE( block != bit::memory::nullblock );

      block_allocator.deallocate_block( block );
    }

    SECTION("Allocates block aligned to at least 'align'")
    {
      auto block = block_allocator.allocate_block();

      REQUIRE( bit::memory::align_of(block.data()) >= align );

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
  }
}

