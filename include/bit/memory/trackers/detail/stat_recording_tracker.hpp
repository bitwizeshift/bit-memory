/**
 * \file stat_recording_tracker.hpp
 *
 * \brief This header contains the implementation of a statistic tracking
 *        MemoryTracker
 *
 * \note This is an internal header file, included by other library headers.
 *       Do not attempt to use it directly.
 */
#ifndef BIT_MEMORY_TRACKERS_DETAIL_STAT_RECORDING_TRACKER_HPP
#define BIT_MEMORY_TRACKERS_DETAIL_STAT_RECORDING_TRACKER_HPP

#include "../../allocator_info.hpp" // allocator_info

#include <cstddef> // std::size_t

namespace bit {
  namespace memory {
    namespace detail {

      /////////////////////////////////////////////////////////////////////////
      /// \brief A statistic-tracking MemoryTracker. This wraps around an
      ///        existing MemoryTracker to record information about the number
      ///        of allocations, peak request size, etc
      ///
      /// \tparam MemoryTracker the underlying MemoryTracker
      ///
      /// \satisfies{MemoryTracker}
      /////////////////////////////////////////////////////////////////////////
      template<typename MemoryTracker>
      class stat_recording_tracker : public MemoryTracker
      {
        //---------------------------------------------------------------------
        // Tracking
        //---------------------------------------------------------------------
      public:

        /// \brief Records an allocation occurring of size \p bytes
        ///
        /// \param p the pointer allocated
        /// \param bytes the number of bytes allocated
        /// \param align the alignment of the allocation
        void on_allocate( void* p, std::size_t bytes, std::size_t align ) noexcept;

        /// \brief Records a deallocation occuring of size \p bytes
        ///
        /// \param info the info for the allocator
        /// \param p the pointer to the memory being deallocated
        /// \param bytes the nuber of bytes being deallocated
        void on_deallocate( const allocator_info& info, void* p, std::size_t bytes ) noexcept;

        /// \brief Records all memory being truncated deallocated
        void on_deallocate_all() noexcept;

        // Inherit the 'finalize' member function from MemoryTracker
        using MemoryTracker::finalize;

        //---------------------------------------------------------------------
        // Element Access
        //---------------------------------------------------------------------
      public:

        /// \brief Returns the largest request size from this tracker
        ///
        /// \return the largest request size, in bytes
        std::size_t largest_request() const noexcept;

        /// \brief Returns the smallest request size from this tracker
        ///
        /// \return the smallest request size, in bytes
        std::size_t smallest_request() const noexcept;

        //---------------------------------------------------------------------

        /// \brief Returns the largest alignment request size from this tracker
        ///
        /// \return the largest alignment request size, in bytes
        std::size_t largest_alignment_request() const noexcept;

        /// \brief Returns the smallest alignment request size from this tracker
        ///
        /// \return the smallest alignment request size, in bytes
        std::size_t smallest_alignment_request() const noexcept;

        //---------------------------------------------------------------------

        /// \brief Returns the peak size of this tracker
        ///
        /// This is the maximum size the tracked allocator reached before being
        /// deallocated
        ///
        /// \return the peak size, in bytes
        std::size_t peak_size() const noexcept;

        /// \brief Returns the total amount of memory allocated
        ///
        /// This is the total amount of memory allocated from the tracked
        /// allocator; meaning it does not pay attention to any deallocations.
        ///
        /// \return the total size allocated, in bytes
        std::size_t total_allocated() const noexcept;

        /// \brief Returns the total number of allocations (e.g. calls to either
        ///        'try_allocate' or 'allocate')
        ///
        /// \return the number of allocations
        std::size_t allocations() const noexcept;

        /// \brief Returns the total number of deallocations (e.g. calls to
        ///        'deallocate')
        ///
        /// Note that this does not include calls to 'deallocate_all', since
        /// that truncates all deallocations
        ///
        /// \return the number of deallocations
        std::size_t deallocations() const noexcept;

        //---------------------------------------------------------------------
        // Private Members
        //---------------------------------------------------------------------
      private:

        std::size_t m_largest_request            = 0;
        std::size_t m_smallest_request           = 0;
        std::size_t m_largest_alignment_request  = 0;
        std::size_t m_smallest_alignment_request = 0;
        std::size_t m_total_allocated            = 0;
        std::size_t m_total_allocations          = 0;
        std::size_t m_total_deallocations        = 0;
        std::ptrdiff_t m_peak_size               = 0;
        std::ptrdiff_t m_running_total           = 0;
      };

    } // namespace detail
  } // namespace memory
} // namespace bit

#include "stat_recording_tracker.inl"

#endif /* BIT_MEMORY_TRACKERS_DETAIL_STAT_RECORDING_TRACKER_HPP */
