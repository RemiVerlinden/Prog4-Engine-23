#pragma once
#include <chrono>
#include <thread>
#include <math.h>
#include "Time.h"
namespace dae
{
	namespace Utils
	{
        //------------------------------------------------------------------------------------
        //  Note: this function allows for high precision sleep (down to microsecond range)
        // this_thread::sleep_for is will behave unpredicatably for microsecond sleep times
        // this function is a little more heavy on the system but is much more accurate
        //------------------------------------------------------------------------------------
        // https://blat-blatnik.github.io/computerBear/making-accurate-sleep-function/
        //------------------------------------------------------------------------------------
        void preciseSleep(Nanoseconds sleepTime) {
            using namespace std;
            using namespace std::chrono;

            static Nanoseconds estimate = Milliseconds(5).ToNanoseconds();
            static Nanoseconds mean = Milliseconds(5).ToNanoseconds();
            static Nanoseconds m2 = 0;
            static uint64_t count = 1;

            while (sleepTime > estimate) {
                auto start = high_resolution_clock::now();
                this_thread::sleep_for(milliseconds(1));
                auto end = high_resolution_clock::now();

                Nanoseconds observed = (end - start).count();
                sleepTime -= observed;

                ++count;
                Nanoseconds delta = observed - mean;
                mean += delta / count;
                m2 += delta * (observed - mean);
                Nanoseconds stddev = (uint64_t)sqrt(m2 / (count - 1));
                estimate = mean + stddev;
            }

            // spin lock
            auto start = high_resolution_clock::now();
            while ((high_resolution_clock::now() - start).count() < (int64_t)sleepTime);
        }
	}
}