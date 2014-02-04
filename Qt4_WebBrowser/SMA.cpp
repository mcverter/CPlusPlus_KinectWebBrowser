#include "SMA.h"

    void SMA::add(double val) {
        // Special case: Initialization
        if (head == NULL) {
            head = window;
            *head = val;
            tail = head;
            inc(tail);
            total = val;
            return;
        }
        // Were we already full?
        if (head == tail) {
            // Fix total-cache
            total -= *head;
            // Make room
            inc(head);
        }
        // Write the value in the next spot.
        *tail = val;
        inc(tail);

        // Update our total-cache
        total += val;
    }
    /* Returns the average of the last P elements added to this SMA.
     If no elements have been added yet, returns 0.0 */
    double SMA::avg() const {
        ptrdiff_t size = this->size();
        if (size == 0) {
            return 0; // No entries => 0 average
        }
        return total / (double) size; // Cast to double for floating point arithmetic
    }

    double SMA::velocity() const{
        ptrdiff_t size = this->size();
        if (size == 0) {
            return 0; // No entries => 0 average
        }
        //


    }


    // Bumps the given pointer up by one.
    // Wraps to the start of the array if needed.
    void SMA::inc(double * & p) {
        if (++p >= window + period) {
            p = window;
        }
    }
    // Returns how many numbers we have stored.
    ptrdiff_t SMA::size() const {
        if (head == NULL)
            return 0;
        if (head == tail)
            return period;
        return (period + tail - head) % period;
    }
