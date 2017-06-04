/**
 * \file null_block_allocator.test.cpp
 *
 * \brief Unit tests for the null_block_allocator
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */

#include <bit/memory/null_block_allocator.hpp>

#include <catch.hpp>

//----------------------------------------------------------------------------
// Block Allocations
//----------------------------------------------------------------------------

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

    SECTION("Origin points to the original block allocator")
    {
      REQUIRE( block.origin() == &block_allocator );
    }
  }
}

//----------------------------------------------------------------------------

TEST_CASE("null_block_allocator::deallocate_block( owner<memory_block> )")
{
  // deallocate does nothing for null_block_allocator
  REQUIRE( true );
}
