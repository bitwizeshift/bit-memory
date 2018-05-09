#ifndef BIT_MEMORY_POLICIES_GROWTH_MULTIPLIERS_DETAIL_QUADRATIC_GROWTH_INL
#define BIT_MEMORY_POLICIES_GROWTH_MULTIPLIERS_DETAIL_QUADRATIC_GROWTH_INL

//=============================================================================
// class definitions : quadratic_growth
//=============================================================================

//-----------------------------------------------------------------------------
// Constructors
//-----------------------------------------------------------------------------

inline bit::memory::quadratic_growth::quadratic_growth( std::size_t cap )
  noexcept
  : m_current(1),
    m_cap(cap)
{

}

//-----------------------------------------------------------------------------
// Growth
//-----------------------------------------------------------------------------

inline void bit::memory::quadratic_growth::grow()
  noexcept
{
  if( m_cap == m_current ) return;
  ++m_current;
}

inline std::size_t bit::memory::quadratic_growth::multiplier()
  const noexcept
{
  const auto x = static_cast<std::size_t>(m_current);

  return x*x;
}

//=============================================================================
// class definitions : uncapped_quadratic_growth
//=============================================================================

//-----------------------------------------------------------------------------
// Constructors
//-----------------------------------------------------------------------------

inline bit::memory::uncapped_quadratic_growth::uncapped_quadratic_growth()
  noexcept
  : m_current(1)
{

}

//-----------------------------------------------------------------------------
// Growth
//-----------------------------------------------------------------------------

inline void bit::memory::uncapped_quadratic_growth::grow()
  noexcept
{
  ++m_current;
}

inline std::size_t bit::memory::uncapped_quadratic_growth::multiplier()
  const noexcept
{
  const auto x = static_cast<std::size_t>(m_current);

  return x*x;
}

#endif /* BIT_MEMORY_POLICIES_GROWTH_MULTIPLIERS_DETAIL_QUADRATIC_GROWTH_INL */
