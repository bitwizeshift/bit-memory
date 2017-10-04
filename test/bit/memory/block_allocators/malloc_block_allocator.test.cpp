/**
 * \file malloc_block_allocator.test.cpp
 *
 * \brief Unit tests for the malloc_block_allocator
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */

#include <bit/memory/block_allocators/malloc_block_allocator.hpp>

#include <catch.hpp>

//----------------------------------------------------------------------------
// Block Allocations
//----------------------------------------------------------------------------

TEST_CASE("malloc_block_allocator::allocate_block()")
{
  static const auto block_size = 1024;
  auto block_allocator = bit::memory::malloc_block_allocator<block_size>();

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

//----------------------------------------------------------------------------

TEST_CASE("malloc_block_allocator::deallocate_block( owner<memory_block> )")
{
  //
}
