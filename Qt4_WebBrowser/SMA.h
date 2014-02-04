#ifndef SMA_H
#define SMA_H
#include <cstddef>
#include <cassert>

class SMA
{

public:
   // construct with a window size eg 8
        SMA(unsigned int period=8) :
            period(period), window(new double[period]), head(NULL), tail(NULL), derivative(new double[period-1]),
            total(0) {
            assert(period >= 1);
        }
        ~SMA() {
            delete[] window;
        }
        void add(double val);

        double avg() const ;

        double velocity() const;



    private:
        unsigned int period;
        double * window; // Holds the values to calculate the average of.
        double * derivative; // Holds velocity
        // Logically, head is before tail
        double * head; // Points at the oldest element we've stored.
        double * tail; // Points at the newest element we've stored.
        double total; // Cache the total so we don't sum everything each time.
        // Bumps the given pointer up by one.
        // Wraps to the start of the array if needed.
        void inc(double * & p);
        // Returns how many numbers we have stored.
        ptrdiff_t size() const;
};

#endif // SMA_H
