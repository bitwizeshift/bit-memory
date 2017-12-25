/**
 * \file virtual_block_allocator.test.cpp
 *
 * \brief Unit tests for the virtual_block_allocator
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */

#include <bit/memory/block_allocators/virtual_block_allocator.hpp>
#include <bit/memory/virtual_memory.hpp>
#include <bit/memory/concepts/BlockAllocator.hpp>

#include <catch.hpp>

//=============================================================================
// Static Requirements
//=============================================================================

using static_type       = bit::memory::virtual_block_allocator;
using named_static_type = bit::memory::named_virtual_block_allocator;

//=============================================================================

static_assert( bit::memory::is_block_allocator<static_type>::value,
               "virtual block allocator must be a block allocator" );

static_assert( bit::memory::is_block_allocator<named_static_type>::value,
               "named virtual block allocator must be a block allocator" );

//=============================================================================
// Unit Tests
//=============================================================================

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

    SECTION("Aligns to virtual page boundary")
    {
      auto align = bit::memory::align_of(block.data());

      REQUIRE( align >= bit::memory::virtual_memory_page_size() );
    }

    SECTION("Size is virtual page size")
    {
      REQUIRE( block.size() == bit::memory::virtual_memory_page_size() );
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
