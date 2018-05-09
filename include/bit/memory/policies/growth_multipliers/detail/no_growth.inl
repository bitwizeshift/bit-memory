#ifndef BIT_MEMORY_POLICIES_GROWTH_MULTIPLIERS_DETAIL_NO_GROWTH_INL
#define BIT_MEMORY_POLICIES_GROWTH_MULTIPLIERS_DETAIL_NO_GROWTH_INL

//=============================================================================
// no_growth_multiplier
//=============================================================================

//-----------------------------------------------------------------------------
// Growth
//-----------------------------------------------------------------------------

inline constexpr void bit::memory::no_growth_multiplier::grow()
  noexcept
{
  // Do nothing
}

inline constexpr std::size_t bit::memory::no_growth_multiplier::multiplier()
  const noexcept
{
  return 1;
}

#endif /* BIT_MEMORY_POLICIES_GROWTH_MULTIPLIERS_DETAIL_NO_GROWTH_INL */
