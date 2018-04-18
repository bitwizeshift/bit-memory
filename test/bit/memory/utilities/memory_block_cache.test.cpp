/*****************************************************************************
 * \file
 * \brief Unit tests for the memory_block_cache
 *****************************************************************************/


#include <bit/memory/utilities/memory_block_cache.hpp>
#include <bit/memory/block_allocators/new_block_allocator.hpp>

#include <catch.hpp>

#include <utility>

//----------------------------------------------------------------------------
// Constructors
//----------------------------------------------------------------------------

TEST_CASE("memory_block_cache::memory_block_cache()")
{
  auto block_cache = bit::memory::memory_block_cache();

  SECTION("Contains no memory_blocks")
  {
    REQUIRE( block_cache.size() == 0 );
  }

  SECTION("Cache has no bytes")
  {
    REQUIRE( block_cache.size_bytes() == 0 );
  }
}

//----------------------------------------------------------------------------
// Caching
//----------------------------------------------------------------------------

TEST_CASE("memory_block_cache::store_block( owner<memory_block> )")
{
  static const auto block_size = 32;
  auto block_allocator = bit::memory::new_block_allocator<block_size>();
  auto block_cache     = bit::memory::memory_block_cache();

  auto block1 = block_allocator.allocate_block();

  auto p1 = block1.data();

  block_cache.store_block( std::move(block1) );

  SECTION("Caching a single block")
  {
    SECTION("Contains single entry")
    {
      REQUIRE( block_cache.size() == 1 );
    }
    SECTION("Increases the size")
    {
      REQUIRE( block_cache.size_bytes() == block_size );
    }
    SECTION("Contains original block")
    {
      auto block = block_cache.request_block();

      REQUIRE( block.data() == p1 );
    }
  }
}

//----------------------------------------------------------------------------

TEST_CASE("memory_block_cache::request_block()")
{
  auto block_cache = bit::memory::memory_block_cache();

  SECTION("Cache is empty")
  {
    SECTION("Request returns null block")
    {
      auto block = block_cache.request_block();

      REQUIRE( (block == bit::memory::nullblock) );
    }
  }

  SECTION("Cache is non-empty")
  {
    static const auto block_size = 1024;
    auto block_allocator = bit::memory::dynamic_new_block_allocator(block_size);
    auto block = block_allocator.allocate_block();

    auto p = block.data();
    auto s = block.size();

    block_cache.store_block( std::move(block) );

    SECTION("Request returns previously cached block")
    {
      block = block_cache.request_block();

      REQUIRE( block.data() == p );
    }
  }
}

TEST_CASE("memory_block_cache::request_block( BlockAllocator& )")
{
  auto block_cache = bit::memory::memory_block_cache();

  auto block_allocator = bit::memory::new_block_allocator<1024>();
  auto block = block_allocator.allocate_block();

  auto p = block.data();

  SECTION("Cache is empty")
  {
    SECTION("Request returns a newly constructed block")
    {
      auto new_block = block_cache.request_block( block_allocator );

      REQUIRE( (new_block != bit::memory::nullblock) );
    }
  }

  SECTION("Cache is non-empty")
  {
    block_cache.store_block( std::move(block) );

    SECTION("Request returns previously cached block")
    {
      block = block_cache.request_block();

      REQUIRE( block.data() == p );
    }
  }
}
