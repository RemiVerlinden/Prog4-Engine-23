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
        void preciseSleep(const Nanoseconds& sleepTime) {
            using namespace std;
            using namespace std::chrono;

            static int64_t estimate = Milliseconds(5).ToNanoseconds();
            static int64_t mean = Milliseconds(5).ToNanoseconds();
            static int64_t m2 = 0;
            static int64_t count = 1;
            int64_t nanoSeconds = (int64_t)sleepTime;

            while (nanoSeconds > estimate) {
                auto start = high_resolution_clock::now();
                this_thread::sleep_for(milliseconds(1));
                auto end = high_resolution_clock::now();

                int64_t observed = (end - start).count();
                nanoSeconds -= observed;

                ++count;
                int64_t delta = observed - mean;
                mean    += delta / count;
                m2      += delta * (observed - mean);
                int64_t stddev = (int64_t)sqrt(m2 / (count - 1));
                estimate = mean + stddev;
            }

            // spin lock
            auto start = high_resolution_clock::now();
            while ((high_resolution_clock::now() - start).count() < nanoSeconds);
        }
	}
}