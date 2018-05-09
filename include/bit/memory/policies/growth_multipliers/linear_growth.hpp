/*****************************************************************************
 * \file
 * \brief This header contains growth policies for linear growth
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
#ifndef BIT_MEMORY_POLICIES_GROWTH_MULTIPLIERS_LINEAR_GROWTH_HPP
#define BIT_MEMORY_POLICIES_GROWTH_MULTIPLIERS_LINEAR_GROWTH_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <cstddef> // std::size_t
#include <cstdint> // std::uint16_t

namespace bit {
  namespace memory {

    //=========================================================================
    // class : linear_growth
    //=========================================================================

    ///////////////////////////////////////////////////////////////////////////
    /// \brief A growth policy that grows linearly
    ///
    /// \satisfies{GrowthMultiplier}
    ///////////////////////////////////////////////////////////////////////////
    class linear_growth
    {
      //-----------------------------------------------------------------------
      // Constructors / Assignment
      //-----------------------------------------------------------------------
    public:

      /// \brief Constructs the linear growth policy that stops growing after
      ///        \p cap growths
      ///
      /// \param cap the maximum number of growths
      explicit linear_growth( std::size_t cap ) noexcept;

      /// \brief Move-constructs this growth policy from an existing one
      ///
      /// \param other the other policy to move
      linear_growth( linear_growth&& other ) noexcept = default;

      /// \brief Copy-constructs this growth policy from an existing one
      ///
      /// \param other the other policy to copy
      linear_growth( const linear_growth& other ) noexcept = default;

      //-----------------------------------------------------------------------

      /// \brief Move-assigns this growth policy from an existing one
      ///
      /// \param other the other policy to move
      /// \return reference to \c (*this)
      linear_growth& operator=( linear_growth&& other ) = default;

      /// \brief Copy-assigns this growth policy from an existing one
      ///
      /// \param other the other policy to copy
      /// \return reference to \c (*this)
      linear_growth& operator=( const linear_growth& other ) = default;

      //-----------------------------------------------------------------------
      // Growth
      //-----------------------------------------------------------------------
    public:

      /// \brief Grows this linear growth policy
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
    // class : uncapped_linear_growth
    //=========================================================================

    ///////////////////////////////////////////////////////////////////////////
    /// \brief A growth policy that grows linearly without any limits
    ///
    /// \satisfies{GrowthMultiplier}
    ///////////////////////////////////////////////////////////////////////////
    class uncapped_linear_growth
    {
      //-----------------------------------------------------------------------
      // Constructors / Assignment
      //-----------------------------------------------------------------------
    public:

      /// \brief Default-constructs an uncapped linear growth policy
      explicit uncapped_linear_growth() noexcept;

      /// \brief Move-constructs this growth policy from an existing one
      ///
      /// \param other the other policy to move
      uncapped_linear_growth( uncapped_linear_growth&& other ) noexcept = default;

      /// \brief Copy-constructs this growth policy from an existing one
      ///
      /// \param other the other policy to copy
      uncapped_linear_growth( const uncapped_linear_growth& other ) noexcept = default;

      //-----------------------------------------------------------------------

      /// \brief Move-assigns this growth policy from an existing one
      ///
      /// \param other the other policy to move
      /// \return reference to \c (*this)
      uncapped_linear_growth& operator=( uncapped_linear_growth&& other ) = default;

      /// \brief Copy-assigns this growth policy from an existing one
      ///
      /// \param other the other policy to copy
      /// \return reference to \c (*this)
      uncapped_linear_growth& operator=( const uncapped_linear_growth& other ) = default;

      //-----------------------------------------------------------------------
      // Growth
      //-----------------------------------------------------------------------
    public:

      /// \brief Grows this linear growth policy
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

#include "detail/linear_growth.inl"

#endif /* BIT_MEMORY_POLICIES_GROWTH_MULTIPLIERS_LINEAR_GROWTH_HPP */
