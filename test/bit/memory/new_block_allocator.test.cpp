/**
 * \file new_block_allocator.test.cpp
 *
 * \brief Unit tests for the new_block_allocator
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */

#include <bit/memory/new_block_allocator.hpp>

#include <catch.hpp>

//----------------------------------------------------------------------------
// Block Allocations
//----------------------------------------------------------------------------

TEST_CASE("new_block_allocator::allocate_block()")
{
  const auto block_size = 1024;
  auto block_allocator = bit::memory::new_block_allocator(block_size);

  SECTION("Allocates a memory block")
  {
    auto block = block_allocator.allocate_block();

    SECTION("Block is not null")
    {
      auto succeeds = block != bit::memory::nullblock;
      REQUIRE( succeeds );
    }

    SECTION("Origin points to the original block allocator")
    {
      REQUIRE( block.origin() == &block_allocator );
    }

    SECTION("Size is specified by constructor")
    {
      REQUIRE( block.size() == block_size );
    }
  }
}

//----------------------------------------------------------------------------

TEST_CASE("new_block_allocator::deallocate_block( owner<memory_block> )")
{
  //
}
