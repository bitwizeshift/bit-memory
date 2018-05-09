#ifndef BIT_MEMORY_POLICIES_GROWTH_MULTIPLIERS_DETAIL_LINEAR_GROWTH_INL
#define BIT_MEMORY_POLICIES_GROWTH_MULTIPLIERS_DETAIL_LINEAR_GROWTH_INL

//=============================================================================
// class definitions : linear_growth
//=============================================================================

//-----------------------------------------------------------------------------
// Constructors
//-----------------------------------------------------------------------------

inline bit::memory::linear_growth::linear_growth( std::size_t cap )
  noexcept
  : m_current(1),
    m_cap(cap)
{

}

//-----------------------------------------------------------------------------
// Growth
//-----------------------------------------------------------------------------

inline void bit::memory::linear_growth::grow()
  noexcept
{
  if( m_cap == m_current ) return;
  ++m_current;
}

inline std::size_t bit::memory::linear_growth::multiplier()
  const noexcept
{
  return m_current;
}

//=============================================================================
// class definitions : uncapped_linear_growth
//=============================================================================

//-----------------------------------------------------------------------------
// Constructors
//-----------------------------------------------------------------------------

inline bit::memory::uncapped_linear_growth::uncapped_linear_growth()
  noexcept
  : m_current(1)
{

}

//-----------------------------------------------------------------------------
// Growth
//-----------------------------------------------------------------------------

inline void bit::memory::uncapped_linear_growth::grow()
  noexcept
{
  ++m_current;
}

inline std::size_t bit::memory::uncapped_linear_growth::multiplier()
  const noexcept
{
  return m_current;
}

#endif /* BIT_MEMORY_POLICIES_GROWTH_MULTIPLIERS_DETAIL_LINEAR_GROWTH_INL */
