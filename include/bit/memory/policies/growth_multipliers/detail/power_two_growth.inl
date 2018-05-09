#ifndef BIT_MEMORY_POLICIES_GROWTH_MULTIPLIERS_DETAIL_POWER_TWO_GROWTH_INL
#define BIT_MEMORY_POLICIES_GROWTH_MULTIPLIERS_DETAIL_POWER_TWO_GROWTH_INL

//=============================================================================
// class definitions : power_two_growth
//=============================================================================

//-----------------------------------------------------------------------------
// Constructors
//-----------------------------------------------------------------------------

inline bit::memory::power_two_growth::power_two_growth( std::size_t cap )
  noexcept
  : m_current(1),
    m_cap(static_cast<std::uint8_t>(cap))
{

}

//-----------------------------------------------------------------------------
// Growth
//-----------------------------------------------------------------------------

inline void bit::memory::power_two_growth::grow()
  noexcept
{
  if( m_cap == m_current ) return;
  ++m_current;
}

inline std::size_t bit::memory::power_two_growth::multiplier()
  const noexcept
{
  return (1 << (m_current-1));
}

//=============================================================================
// class definitions : uncapped_power_two_growth
//=============================================================================

//-----------------------------------------------------------------------------
// Constructors
//-----------------------------------------------------------------------------

inline bit::memory::uncapped_power_two_growth::uncapped_power_two_growth()
  noexcept
  : m_current(0)
{

}

//-----------------------------------------------------------------------------
// Growth
//-----------------------------------------------------------------------------

inline void bit::memory::uncapped_power_two_growth::grow()
  noexcept
{
  ++m_current;
}

inline std::size_t bit::memory::uncapped_power_two_growth::multiplier()
  const noexcept
{
  return (1 << m_current);
}

#endif /* BIT_MEMORY_POLICIES_GROWTH_MULTIPLIERS_DETAIL_POWER_TWO_GROWTH_INL */
