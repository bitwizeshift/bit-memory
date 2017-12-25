/**
 * \file null_block_allocator.test.cpp
 *
 * \brief Unit tests for the null_block_allocator
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */

#include <bit/memory/block_allocators/null_block_allocator.hpp>
#include <bit/memory/concepts/Stateless.hpp>
#include <bit/memory/concepts/BlockAllocator.hpp>

#include <catch.hpp>

//=============================================================================
// Static Requirements
//=============================================================================

using static_type       = bit::memory::null_block_allocator;
using named_static_type = bit::memory::named_null_block_allocator;

//=============================================================================

static_assert( bit::memory::is_block_allocator<static_type>::value,
               "null block allocator must be a block allocator" );

static_assert( bit::memory::is_block_allocator<named_static_type>::value,
               "named null block allocator must be a block allocator" );

//=============================================================================

static_assert( bit::memory::is_stateless<static_type>::value,
               "null block allocator must be stateless" );

static_assert( !bit::memory::is_stateless<named_static_type>::value,
               "named null block allocator cannot be stateless stateless" );

//=============================================================================
// Unit Tests
//=============================================================================

//-----------------------------------------------------------------------------
// Block Allocations
//-----------------------------------------------------------------------------

TEST_CASE("null_block_allocator::allocate_block()")
{
  auto block_allocator = bit::memory::null_block_allocator();

  SECTION("Allocates a memory block")
  {
    auto block = block_allocator.allocate_block();

    SECTION("Block is null")
    {
      auto succeeds = block == bit::memory::nullblock;
      REQUIRE( succeeds );
    }
  }
}

//-----------------------------------------------------------------------------

TEST_CASE("null_block_allocator::deallocate_block( owner<memory_block> )")
{
  // deallocate does nothing for null_block_allocator
  REQUIRE( true );
}
