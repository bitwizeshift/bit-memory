/*****************************************************************************
 * \file
 * \brief Unit tests for the growing_malloc_block_allocator
 *****************************************************************************/


#include <bit/memory/block_allocators/growing_malloc_block_allocator.hpp>
#include <bit/memory/concepts/BlockAllocator.hpp>

#include <catch.hpp>

#include <cstring> // std::memset
#include <array> // std::array

//=============================================================================
// Static Requirements
//=============================================================================

using static_type              = bit::memory::growing_malloc_block_allocator<64>;
using named_static_type        = bit::memory::named_growing_malloc_block_allocator<64>;
using cached_static_type       = bit::memory::cached_growing_malloc_block_allocator<64>;
using named_cached_static_type = bit::memory::named_cached_growing_malloc_block_allocator<64>;

using dynamic_type              = bit::memory::dynamic_growing_malloc_block_allocator;
using named_dynamic_type        = bit::memory::named_dynamic_growing_malloc_block_allocator;
using cached_dynamic_type       = bit::memory::cached_dynamic_growing_malloc_block_allocator;
using named_cached_dynamic_type = bit::memory::named_cached_dynamic_growing_malloc_block_allocator;

//=============================================================================

static_assert( bit::memory::is_block_allocator<static_type>::value,
               "static growing malloc block allocator must be a block allocator" );

static_assert( bit::memory::is_block_allocator<named_static_type>::value,
               "named static growing malloc block allocator must be a block allocator" );

static_assert( bit::memory::is_block_allocator<cached_static_type>::value,
               "cached static growing malloc allocator must be a block allocator" );

static_assert( bit::memory::is_block_allocator<named_cached_static_type>::value,
               "named cached growing malloc malloc block allocator must be a block allocator" );

//-----------------------------------------------------------------------------

static_assert( bit::memory::is_block_allocator<dynamic_type>::value,
               "dynamic growing malloc block allocator must be a block allocator" );

static_assert( bit::memory::is_block_allocator<named_dynamic_type>::value,
               "named dynamic growing malloc block allocator must be a block allocator");

static_assert( bit::memory::is_block_allocator<cached_dynamic_type>::value,
               "cached dynamic growing malloc block allocator must be a block allocator");

static_assert( bit::memory::is_block_allocator<named_cached_dynamic_type>::value,
               "named cached dynamic cached growing malloc block allocator must be a block allocator");

//=============================================================================
// Unit Tests
//=============================================================================

//-----------------------------------------------------------------------------
// growing_malloc_block_allocator<1024>
//-----------------------------------------------------------------------------

TEST_CASE("growing_malloc_block_allocator<block_size>" "[resource management]")
{
  static constexpr auto block_size  = 1024;
  static constexpr auto growths     = 3u;
  auto block_allocator = bit::memory::growing_malloc_block_allocator<block_size>{growths};

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
  }
}

//-----------------------------------------------------------------------------
// cached_growing_malloc_block_allocator<1024>
//-----------------------------------------------------------------------------

TEST_CASE("cached_growing_malloc_block_allocator<block_size>" "[resource management]")
{
  static constexpr auto block_size = 1024;
  static constexpr auto growths    = 3u;
  auto block_allocator = bit::memory::cached_growing_malloc_block_allocator<block_size>{growths};

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
  }
}
