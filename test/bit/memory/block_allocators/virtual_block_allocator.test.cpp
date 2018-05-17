/*****************************************************************************
 * \file
 * \brief Unit tests for the virtual_block_allocator
 *****************************************************************************/


#include <bit/memory/block_allocators/virtual_block_allocator.hpp>
#include <bit/memory/regions/virtual_memory.hpp>
#include <bit/memory/concepts/BlockAllocator.hpp>

#include <bit/memory/policies/growth_multipliers/power_two_growth.hpp>

#include <catch.hpp>

#include <cstring> // std::memset
#include <array> // std::array

//=============================================================================
// Static Requirements
//=============================================================================

using static_type       = bit::memory::virtual_block_allocator<3u>;
using named_static_type = bit::memory::named_virtual_block_allocator<3u>;

//=============================================================================

static_assert( bit::memory::is_block_allocator<static_type>::value,
               "virtual block allocator must be a block allocator" );

static_assert( bit::memory::is_block_allocator<named_static_type>::value,
               "named virtual block allocator must be a block allocator" );

//=============================================================================
// Unit Tests
//=============================================================================

//-----------------------------------------------------------------------------
// virtual_block_allocator
//-----------------------------------------------------------------------------

TEST_CASE("virtual_block_allocator" "[resource management]")
{
  static constexpr auto blocks     = 3u;
  static const     auto block_size = bit::memory::virtual_memory_page_size();
  auto block_allocator = bit::memory::virtual_block_allocator<blocks>{};

  //---------------------------------------------------------------------------

  SECTION("allocate_block with blocks available")
  {
    SECTION("Lists next_block_size as 'block_size'")
    {
      const auto size = block_allocator.next_block_size();

      REQUIRE( size == block_size );
    }

    SECTION("Allocates non-null block")
    {
      auto block = block_allocator.allocate_block();

      auto success = block != bit::memory::nullblock;
      REQUIRE( success );

      block_allocator.deallocate_block( block );
    }
  }

  //---------------------------------------------------------------------------

  SECTION("allocate_block without blocks available")
  {
    auto allocated_blocks = std::array<bit::memory::memory_block,blocks>{};
    for( auto i = 0; i < blocks; ++i ) {
      allocated_blocks[i] = block_allocator.allocate_block();
    }

    SECTION("Lists next_block_size as 0")
    {
      const auto size = block_allocator.next_block_size();

      REQUIRE( size == 0 );
    }

    SECTION("Allocates a null block")
    {
      const auto null_block = block_allocator.allocate_block();

      auto success = null_block == bit::memory::nullblock;
      REQUIRE( success );
    }

    for( auto i = 0; i < blocks; ++i ) {
      block_allocator.deallocate_block( allocated_blocks[i] );
    }
  }

  //---------------------------------------------------------------------------

  // This test only works because 'static_block_allocator<N,1>' holds a single
  // block; otherwise the order of reuse is implementation defined.
  SECTION("allocate_block reuses previously deallocated block")
  {
    auto p1 = static_cast<void*>(nullptr);
    auto s1 = static_cast<std::size_t>(0);
    {
      auto block = block_allocator.allocate_block();
      p1 = block.data();
      s1 = block.size();
      block_allocator.deallocate_block( block );
    }

    SECTION("Lists next_block_size as 's1'")
    {
      const auto size = block_allocator.next_block_size();

      REQUIRE( size == s1 );
    }

    SECTION("Allocates a block")
    {
      const auto block = block_allocator.allocate_block();

      auto success = block != bit::memory::nullblock;
      REQUIRE( success );

      block_allocator.deallocate_block( block );
    }

    SECTION("Reuses previously allocated block")
    {
      const auto block = block_allocator.allocate_block();
      const auto p2 = block.data();
      const auto s2 = block.size();

      SECTION("Allocates same memory region")
      {
        REQUIRE( p1 == p2 );
      }

      SECTION("Allocates same block size")
      {
        REQUIRE( s1 == s2 );
      }

      block_allocator.deallocate_block( block );
    }
  }

  //---------------------------------------------------------------------------

  SECTION("allocate_block creates read/writeable range of memory")
  {
    const auto block = block_allocator.allocate_block();
    auto start = static_cast<unsigned char*>(block.data());
    auto end   = static_cast<unsigned char*>(block.data()) + block.size();

    // test write
    std::memset( block.data(), 0x01, block.size() );

    // test read
    auto sum = 0u;
    for( ; start != end; ++start ) sum += *start;

    REQUIRE( sum == block.size() );

    block_allocator.deallocate_block( block );
  }
}

//-----------------------------------------------------------------------------
// virtual_block_allocator<N,power_two_growth>
//-----------------------------------------------------------------------------

TEST_CASE("virtual_block_allocator<N,power_two_growth>" "[resource management]")
{
  using growth_multiplier = bit::memory::uncapped_power_two_growth;
  static constexpr auto blocks     = 4u;
  static const     auto block_size = bit::memory::virtual_memory_page_size();
  auto block_allocator = bit::memory::virtual_block_allocator<blocks,growth_multiplier>{};

  //---------------------------------------------------------------------------

  SECTION("allocate_block with blocks available")
  {
    SECTION("Lists next_block_size as 'block_size'")
    {
      const auto size = block_allocator.next_block_size();

      REQUIRE( size == block_size );
    }

    SECTION("Allocates non-null block")
    {
      auto block = block_allocator.allocate_block();

      auto success = block != bit::memory::nullblock;
      REQUIRE( success );

      block_allocator.deallocate_block( block );
    }
  }

  //---------------------------------------------------------------------------

  SECTION("allocate_block without blocks available")
  {
    auto allocated_blocks = std::array<bit::memory::memory_block,3>{};
    for( auto i = 0; i < 3; ++i ) {
      allocated_blocks[i] = block_allocator.allocate_block();
    }

    SECTION("Lists next_block_size as 0")
    {
      const auto size = block_allocator.next_block_size();

      REQUIRE( size == 0 );
    }

    SECTION("Allocates a null block")
    {
      const auto null_block = block_allocator.allocate_block();

      auto success = null_block == bit::memory::nullblock;
      REQUIRE( success );
    }

    for( auto i = 0; i < 3; ++i ) {
      block_allocator.deallocate_block( allocated_blocks[i] );
    }
  }

  //---------------------------------------------------------------------------

  // This test only works because 'static_block_allocator<N,1>' holds a single
  // block; otherwise the order of reuse is implementation defined.
  SECTION("allocate_block reuses previously deallocated block")
  {
    auto p1 = static_cast<void*>(nullptr);
    auto s1 = static_cast<std::size_t>(0);
    {
      auto block = block_allocator.allocate_block();
      p1 = block.data();
      s1 = block.size();
      block_allocator.deallocate_block( block );
    }

    SECTION("Lists next_block_size as 's1'")
    {
      const auto size = block_allocator.next_block_size();

      REQUIRE( size == s1 );
    }

    SECTION("Allocates a block")
    {
      const auto block = block_allocator.allocate_block();

      auto success = block != bit::memory::nullblock;
      REQUIRE( success );

      block_allocator.deallocate_block( block );
    }

    SECTION("Reuses previously allocated block")
    {
      const auto block = block_allocator.allocate_block();
      const auto p2 = block.data();
      const auto s2 = block.size();

      SECTION("Allocates same memory region")
      {
        REQUIRE( p1 == p2 );
      }

      SECTION("Allocates same block size")
      {
        REQUIRE( s1 == s2 );
      }

      block_allocator.deallocate_block( block );
    }
  }

  //---------------------------------------------------------------------------

  SECTION("allocate_block creates read/writeable range of memory")
  {
    const auto block = block_allocator.allocate_block();
    auto start = static_cast<unsigned char*>(block.data());
    auto end   = static_cast<unsigned char*>(block.data()) + block.size();

    // test write
    std::memset( block.data(), 0x01, block.size() );

    // test read
    auto sum = 0u;
    for( ; start != end; ++start ) sum += *start;

    REQUIRE( sum == block.size() );

    block_allocator.deallocate_block( block );
  }
}
