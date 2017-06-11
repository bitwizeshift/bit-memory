/**
 * \file virtual_block_allocator.test.cpp
 *
 * \brief Unit tests for the virtual_block_allocator
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */

#include <bit/memory/virtual_memory.hpp>
#include <bit/memory/virtual_block_allocator.hpp>
#include <bit/memory/alignment.hpp>
#include <bit/memory/debugging.hpp>

#include <catch.hpp>

//----------------------------------------------------------------------------
// Block Allocations
//----------------------------------------------------------------------------

TEST_CASE("virtual_block_allocator::allocate_block()")
{
  auto block_allocator = bit::memory::virtual_block_allocator(3);

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

    SECTION("Aligns to virtual page boundary")
    {
      auto align = bit::memory::align_of(block.data());

      REQUIRE( align >= bit::memory::virtual_memory_page_size );
    }

    SECTION("Size is virtual page size")
    {
      REQUIRE( block.size() == bit::memory::virtual_memory_page_size );
    }
  }

  SECTION("Reallocates previously deallocated blocks")
  {
    auto block1 = block_allocator.allocate_block();

    block_allocator.deallocate_block( block1 );

    auto block2 = block_allocator.allocate_block();

    SECTION("Block is the same as previous block")
    {
      REQUIRE( block1 == block2 );
    }
  }
}

//----------------------------------------------------------------------------

TEST_CASE("virtual_block_allocator::deallocate_block( owner<memory_block> )")
{
  auto block_allocator = bit::memory::virtual_block_allocator(3);

  SECTION("Deallocates a previously allocated block")
  {
    auto block = block_allocator.allocate_block();
    block_allocator.deallocate_block( std::move(block) );

    REQUIRE( true );
  }
}
