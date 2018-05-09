/*****************************************************************************
 * \file
 * \brief This header contains a growth policy that grows in powers-of-two
 *****************************************************************************/

/*
  The MIT License (MIT)

  Copyright (c) 2018 Matthew Rodusek

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/
#ifndef BIT_MEMORY_POLICIES_GROWTH_MULTIPLIERS_POWER_TWO_GROWTH_HPP
#define BIT_MEMORY_POLICIES_GROWTH_MULTIPLIERS_POWER_TWO_GROWTH_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <cstddef> // std::size_t
#include <cstdint> // std::uint16_t

namespace bit {
  namespace memory {

    //=========================================================================
    // class : power_two_growth
    //=========================================================================

    ///////////////////////////////////////////////////////////////////////////
    /// \brief A growth multiplier that grows by repeatedly increasing the
    ///        growth size by 2, growing up until a specified cap
    ///
    /// \satisfies{GrowthMultiplier}
    ///////////////////////////////////////////////////////////////////////////
    class power_two_growth
    {
      //-----------------------------------------------------------------------
      // Constructors / Assignment
      //-----------------------------------------------------------------------
    public:

      /// \brief Constructs a growth policy that is capped at \p cap growths
      ///
      /// \param cap the number of growths to increase by
      explicit power_two_growth( std::size_t cap ) noexcept;

      /// \brief Move-constructs this growth policy from an existing one
      ///
      /// \param other the other policy to move
      power_two_growth( power_two_growth&& other ) noexcept = default;

      /// \brief Copy-constructs this growth policy from an existing one
      ///
      /// \param other the other policy to copy
      power_two_growth( const power_two_growth& other ) noexcept = default;

      //-----------------------------------------------------------------------

      /// \brief Move-assigns this growth policy from an existing one
      ///
      /// \param other the other policy to move
      /// \return reference to \c (*this)
      power_two_growth& operator=( power_two_growth&& other ) = default;

      /// \brief Copy-assigns this growth policy from an existing one
      ///
      /// \param other the other policy to copy
      /// \return reference to \c (*this)
      power_two_growth& operator=( const power_two_growth& other ) = default;

      //-----------------------------------------------------------------------
      // Growth
      //-----------------------------------------------------------------------
    public:

      /// \brief Grows this policy by a power of two
      void grow() noexcept;

      /// \brief Returns the multiplier
      ///
      /// \return the multiplier
      std::size_t multiplier() const noexcept;

      //-----------------------------------------------------------------------
      // Private Members
      //-----------------------------------------------------------------------
    private:

      std::uint8_t m_current;
      std::uint8_t m_cap;
    };

    //=========================================================================
    // class : uncapped_power_two_growth
    //=========================================================================

    ///////////////////////////////////////////////////////////////////////////
    /// \brief A growth multiplier that grows by repeatedly increasing the
    ///        growth size by 2
    ///
    /// \satisfies{GrowthMultiplier}
    ///////////////////////////////////////////////////////////////////////////
    class uncapped_power_two_growth
    {
      //-----------------------------------------------------------------------
      // Constructors / Assignment
      //-----------------------------------------------------------------------
    public:

      /// \brief Default-constructs this growth policy
      explicit uncapped_power_two_growth() noexcept;

      /// \brief Move-constructs this growth policy from an existing one
      ///
      /// \param other the other policy to move
      uncapped_power_two_growth( uncapped_power_two_growth&& other ) noexcept = default;

      /// \brief Copy-constructs this growth policy from an existing one
      ///
      /// \param other the other policy to copy
      uncapped_power_two_growth( const uncapped_power_two_growth& other ) noexcept = default;

      //-----------------------------------------------------------------------

      /// \brief Move-assigns this growth policy from an existing one
      ///
      /// \param other the other policy to move
      /// \return reference to \c (*this)
      uncapped_power_two_growth& operator=( uncapped_power_two_growth&& other ) = default;

      /// \brief Copy-assigns this growth policy from an existing one
      ///
      /// \param other the other policy to copy
      /// \return reference to \c (*this)
      uncapped_power_two_growth& operator=( const uncapped_power_two_growth& other ) = default;

      //-----------------------------------------------------------------------
      // Growth
      //-----------------------------------------------------------------------
    public:

      /// \brief Grows this policy by a power of two
      void grow() noexcept;

      /// \brief Returns the multiplier
      ///
      /// \return the multiplier
      std::size_t multiplier() const noexcept;

      //-----------------------------------------------------------------------
      // Private Members
      //-----------------------------------------------------------------------
    private:

      std::uint8_t m_current; // 255 * 255
    };

  } // namespace memory
} // namespace bit

#include "detail/power_two_growth.inl"

#endif /* BIT_MEMORY_POLICIES_GROWTH_MULTIPLIERS_POWER_TWO_GROWTH_HPP */
