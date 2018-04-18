/*****************************************************************************
 * \file
 * \brief Unit tests for the block_allocator_reference
 *****************************************************************************/


#include <bit/memory/block_allocators/block_allocator_reference.hpp>
#include <bit/memory/utilities/allocator_info.hpp>

#include <catch.hpp>

namespace {
  class test_block_allocator
  {
  public:

    bool did_allocate = false;
    bool did_deallocate = false;

    bit::memory::memory_block allocate_block()
    {
      did_allocate = true;
      return bit::memory::nullblock;
    }

    void deallocate_block( bit::memory::memory_block )
    {
      did_deallocate = true;
    }

    std::size_t next_block_size() const noexcept
    {
      return 0;
    }

    bit::memory::allocator_info info() const noexcept
    {
      return {"test_allocator",this};
    }
  };
}


//-----------------------------------------------------------------------------
// Allocation / Deallocation
//-----------------------------------------------------------------------------

TEST_CASE("any_block_allocator::allocate_block()")
{
  auto underlying_allocator = test_block_allocator{};
  auto allocator = bit::memory::block_allocator_reference{underlying_allocator};

  SECTION("Allocates memory using the underlying allocator")
  {
    allocator.allocate_block();

    REQUIRE( underlying_allocator.did_allocate );
  }
}

//-----------------------------------------------------------------------------

TEST_CASE("any_block_allocator::deallocate_block( bit::memory::memory_block )")
{
  auto underlying_allocator = test_block_allocator{};
  auto allocator = bit::memory::block_allocator_reference{underlying_allocator};

  SECTION("Deallocates memory using the underlying allocator")
  {
    allocator.deallocate_block(bit::memory::nullblock);

    REQUIRE( underlying_allocator.did_deallocate );
  }
}

//-----------------------------------------------------------------------------

TEST_CASE("any_block_allocator::info()")
{
  auto underlying_allocator = test_block_allocator{};
  auto allocator = bit::memory::block_allocator_reference{underlying_allocator};

  SECTION("Accesses the underlying allocator info")
  {
    auto info = allocator.info();

    REQUIRE( info.address() == &underlying_allocator );
  }
}
