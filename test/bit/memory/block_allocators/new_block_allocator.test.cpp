/**
 * \file new_block_allocator.test.cpp
 *
 * \brief Unit tests for the new_block_allocator
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */

#include <bit/memory/block_allocators/new_block_allocator.hpp>
#include <bit/memory/concepts/Stateless.hpp>
#include <bit/memory/concepts/BlockAllocator.hpp>

#include <catch.hpp>

//=============================================================================
// Static Requirements
//=============================================================================

using static_type              = bit::memory::new_block_allocator<64>;
using named_static_type        = bit::memory::named_new_block_allocator<64>;
using cached_static_type       = bit::memory::cached_new_block_allocator<64>;
using named_cached_static_type = bit::memory::named_cached_new_block_allocator<64>;

using dynamic_type              = bit::memory::dynamic_new_block_allocator;
using named_dynamic_type        = bit::memory::named_dynamic_new_block_allocator;
using cached_dynamic_type       = bit::memory::cached_dynamic_new_block_allocator;
using named_cached_dynamic_type = bit::memory::named_cached_dynamic_new_block_allocator;

//=============================================================================

static_assert( bit::memory::is_block_allocator<static_type>::value,
               "static new block allocator must be a block allocator" );

static_assert( bit::memory::is_block_allocator<named_static_type>::value,
               "named static new block allocator must be a block allocator" );

static_assert( bit::memory::is_block_allocator<cached_static_type>::value,
               "cached static new allocator must be a block allocator" );

static_assert( bit::memory::is_block_allocator<named_cached_static_type>::value,
               "named cached static new block allocator must be a block allocator" );

//-----------------------------------------------------------------------------

static_assert( bit::memory::is_block_allocator<dynamic_type>::value,
               "dynamic new block allocator must be a block allocator" );

static_assert( bit::memory::is_block_allocator<named_dynamic_type>::value,
               "named dynamic new block allocator must be a block allocator");

static_assert( bit::memory::is_block_allocator<cached_dynamic_type>::value,
               "cached dynamic new block allocator must be a block allocator");

static_assert( bit::memory::is_block_allocator<named_cached_dynamic_type>::value,
               "named cached dynamic cached new block allocator must be a block allocator");

//=============================================================================

static_assert( bit::memory::is_stateless<static_type>::value,
               "static new block allocator must be stateless" );

static_assert( !bit::memory::is_stateless<named_static_type>::value,
               "named static new block allocator cannot be stateless" );

static_assert( !bit::memory::is_stateless<cached_static_type>::value,
               "cached static new allocator cannot be stateless" );

static_assert( !bit::memory::is_stateless<named_cached_static_type>::value,
               "named cached static new block allocator cannot be stateless" );

//-----------------------------------------------------------------------------

static_assert( !bit::memory::is_stateless<dynamic_type>::value,
               "dynamic new block allocator cannot be stateless" );

static_assert( !bit::memory::is_stateless<named_dynamic_type>::value,
               "named dynamic new block allocator cannot be stateless");

static_assert( !bit::memory::is_stateless<cached_dynamic_type>::value,
               "cached dynamic new block allocator cannot be stateless");

static_assert( !bit::memory::is_stateless<named_cached_dynamic_type>::value,
               "named cached dynamic cached new block allocator cannot be stateless");

//=============================================================================
// Unit Tests
//=============================================================================

//-----------------------------------------------------------------------------
// Block Allocations
//-----------------------------------------------------------------------------

TEST_CASE("new_block_allocator::allocate_block<1024>()")
{
  static const auto block_size = 1024;
  auto block_allocator = bit::memory::new_block_allocator<block_size>();

  SECTION("Allocates a memory block")
  {
    auto block = block_allocator.allocate_block();

    SECTION("Block is not null")
    {
      auto succeeds = block != bit::memory::nullblock;
      REQUIRE( succeeds );
    }

    SECTION("Size is specified by constructor")
    {
      REQUIRE( block.size() == block_size );
    }
  }
}

//-----------------------------------------------------------------------------

TEST_CASE("new_block_allocator::deallocate_block( owner<memory_block> )")
{
  //
}
