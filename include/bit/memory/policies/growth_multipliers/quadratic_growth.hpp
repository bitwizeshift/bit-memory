/*****************************************************************************
 * \file
 * \brief This header contains growth policies that grow quadratically
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
#ifndef BIT_MEMORY_POLICIES_GROWTH_MULTIPLIERS_QUADRATIC_GROWTH_HPP
#define BIT_MEMORY_POLICIES_GROWTH_MULTIPLIERS_QUADRATIC_GROWTH_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <cstddef> // std::size_t
#include <cstdint> // std::uint16_t

namespace bit {
  namespace memory {

    //=========================================================================
    // class : quadratic_growth
    //=========================================================================

    ///////////////////////////////////////////////////////////////////////////
    /// \brief A growth policy that grows quadratically toward a limit
    ///
    /// \satisfies{GrowthMultiplier}
    ///////////////////////////////////////////////////////////////////////////
    class quadratic_growth
    {
      //-----------------------------------------------------------------------
      // Constructors / Assignment
      //-----------------------------------------------------------------------
    public:

      /// \brief Constructs a growth policy that is capped at \p cap growths
      ///
      /// \param cap the number of growths to increase by
      explicit quadratic_growth( std::size_t cap ) noexcept;

      /// \brief Move-constructs this growth policy from an existing one
      ///
      /// \param other the other policy to move
      quadratic_growth( quadratic_growth&& other ) noexcept = default;

      /// \brief Copy-constructs this growth policy from an existing one
      ///
      /// \param other the other policy to copy
      quadratic_growth( const quadratic_growth& other ) noexcept = default;

      //-----------------------------------------------------------------------

      /// \brief Move-assigns this growth policy from an existing one
      ///
      /// \param other the other policy to move
      /// \return reference to \c (*this)
      quadratic_growth& operator=( quadratic_growth&& other ) = default;

      /// \brief Copy-assigns this growth policy from an existing one
      ///
      /// \param other the other policy to copy
      /// \return reference to \c (*this)
      quadratic_growth& operator=( const quadratic_growth& other ) = default;

      //-----------------------------------------------------------------------
      // Growth
      //-----------------------------------------------------------------------
    public:

      /// \brief Grows this policy quadratically
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
    // class : uncapped_quadratic_growth
    //=========================================================================

    ///////////////////////////////////////////////////////////////////////////
    /// \brief A growth policy that grows quadratically without limit
    ///
    /// \satisfies{GrowthMultiplier}
    ///////////////////////////////////////////////////////////////////////////
    class uncapped_quadratic_growth
    {
      //-----------------------------------------------------------------------
      // Constructors / Assignment
      //-----------------------------------------------------------------------
    public:

      /// \brief Default-constructs this growth policy
      explicit uncapped_quadratic_growth() noexcept;

      /// \brief Move-constructs this growth policy from an existing one
      ///
      /// \param other the other policy to move
      uncapped_quadratic_growth( uncapped_quadratic_growth&& other ) noexcept = default;

      /// \brief Copy-constructs this growth policy from an existing one
      ///
      /// \param other the other policy to copy
      uncapped_quadratic_growth( const uncapped_quadratic_growth& other ) noexcept = default;

      //-----------------------------------------------------------------------

      /// \brief Move-assigns this growth policy from an existing one
      ///
      /// \param other the other policy to move
      /// \return reference to \c (*this)
      uncapped_quadratic_growth& operator=( uncapped_quadratic_growth&& other ) = default;

      /// \brief Copy-assigns this growth policy from an existing one
      ///
      /// \param other the other policy to copy
      /// \return reference to \c (*this)
      uncapped_quadratic_growth& operator=( const uncapped_quadratic_growth& other ) = default;

      //-----------------------------------------------------------------------
      // Growth
      //-----------------------------------------------------------------------
    public:

      /// \brief Grows this policy quadratically
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

#include "detail/quadratic_growth.inl"

#endif /* BIT_MEMORY_POLICIES_GROWTH_MULTIPLIERS_QUADRATIC_GROWTH_HPP */
