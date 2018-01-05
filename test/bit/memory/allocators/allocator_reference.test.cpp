/**
 * \file allocator_reference.test.cpp
 *
 * \brief Unit tests for the allocator_reference
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */

#include <bit/memory/allocators/allocator_reference.hpp>
#include <bit/memory/allocator_info.hpp>

#include <catch.hpp>

namespace {
  class test_allocator
  {
  public:

    bool did_allocate = false;
    bool did_deallocate = false;

    void* try_allocate( std::size_t, std::size_t )
    {
      did_allocate = true;
      return nullptr;
    }

    void deallocate( void*, std::size_t )
    {
      did_deallocate = true;
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

TEST_CASE("allocator_reference::try_allocate( std::size_t, std::size_t )")
{
  auto underlying_allocator = test_allocator{};
  auto allocator = bit::memory::allocator_reference{underlying_allocator};

  SECTION("Allocates memory using the underlying allocator")
  {
    allocator.try_allocate(64,64);

    REQUIRE( underlying_allocator.did_allocate );
  }
}

//-----------------------------------------------------------------------------

TEST_CASE("allocator_reference::deallocate( void*, std::size_t )")
{
  auto underlying_allocator = test_allocator{};
  auto allocator = bit::memory::allocator_reference{underlying_allocator};

  SECTION("Deallocates memory using the underlying allocator")
  {
    allocator.deallocate(nullptr,64);

    REQUIRE( underlying_allocator.did_deallocate );
  }
}

//-----------------------------------------------------------------------------

TEST_CASE("allocator_reference::info()")
{
  auto underlying_allocator = test_allocator{};
  auto allocator = bit::memory::allocator_reference{underlying_allocator};

  SECTION("Accesses the underlying allocator info")
  {
    auto info = allocator.info();

    REQUIRE( info.address() == &underlying_allocator );
  }
}
