/*****************************************************************************
 * \file
 * \brief This header contains a null-growth policy
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
#ifndef BIT_MEMORY_POLICIES_GROWTH_MULTIPLIERS_NO_GROWTH_HPP
#define BIT_MEMORY_POLICIES_GROWTH_MULTIPLIERS_NO_GROWTH_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <cstddef> // std::size_t

namespace bit {
  namespace memory {

    ///////////////////////////////////////////////////////////////////////////
    /// \brief A multiplier that doesn't actually grow
    ///
    /// \satisfies{GrowthMultiplier}
    /// \satisfies{Stateless}
    ///////////////////////////////////////////////////////////////////////////
    class no_growth_multiplier
    {
      //-----------------------------------------------------------------------
      // Constructors / Assignment
      //-----------------------------------------------------------------------
    public:

      /// \brief Default-constructs this growth policy
      constexpr no_growth_multiplier() noexcept = default;

      /// \brief Move-constructs this growth policy from an existing one
      ///
      /// \param other the other policy to move
      no_growth_multiplier( no_growth_multiplier&& other ) noexcept = default;

      /// \brief Copy-constructs this growth policy from an existing one
      ///
      /// \param other the other policy to copy
      no_growth_multiplier( const no_growth_multiplier& other ) noexcept = default;

      //-----------------------------------------------------------------------

      /// \brief Move-assigns this growth policy from an existing one
      ///
      /// \param other the other policy to move
      /// \return reference to \c (*this)
      no_growth_multiplier& operator=( no_growth_multiplier&& other ) = default;

      /// \brief Copy-assigns this growth policy from an existing one
      ///
      /// \param other the other policy to copy
      /// \return reference to \c (*this)
      no_growth_multiplier& operator=( const no_growth_multiplier& other ) = default;

      //-----------------------------------------------------------------------
      // Growth
      //-----------------------------------------------------------------------
    public:

      /// \brief No-op
      constexpr void grow() noexcept;

      /// \brief Returns the multiplier
      ///
      /// \return 1
      constexpr std::size_t multiplier() const noexcept;
    };

  } // namespace memory
} // namespace bit

#include "detail/no_growth.inl"

#endif /* BIT_MEMORY_POLICIES_GROWTH_MULTIPLIERS_NO_GROWTH_HPP */
