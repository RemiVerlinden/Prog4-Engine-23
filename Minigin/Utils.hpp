#pragma once
#include <chrono>
#include <thread>
#include <math.h>
#include <vector>
#include <utility>
#include "EngineTime.h"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#endif

namespace dae
{
	namespace Utils
	{
#ifdef _WIN32
		//------------------------------------------------------------------------------------
		// This function uses the windows API to sleep for a much more precise amount of time
		//  
		//------------------------------------------------------------------------------------
		// https://blat-blatnik.github.io/computerBear/making-accurate-sleep-function/
		//------------------------------------------------------------------------------------
		void preciseSleep(Nanoseconds sleepTime)
		{
			using namespace std::chrono;

			static HANDLE timer = CreateWaitableTimer(NULL, FALSE, NULL);
			static int64_t estimate = Milliseconds(5).ToNanoseconds();
			static int64_t mean = Milliseconds(5).ToNanoseconds();
			static int64_t m2 = 0;
			static int64_t count = 1;
			int64_t nanoSeconds = (int64_t)sleepTime;

			while (nanoSeconds - estimate > 100) {
				int64_t toWait = nanoSeconds - estimate;
				LARGE_INTEGER due;
				due.QuadPart = -int64_t(toWait >> 2);
				auto start = high_resolution_clock::now();
				SetWaitableTimerEx(timer, &due, 0, NULL, NULL, NULL, 0);
				WaitForSingleObject(timer, INFINITE);
				auto end = high_resolution_clock::now();

				int64_t observed = (end - start).count();
				nanoSeconds -= observed;

				++count;
				int64_t error = observed - toWait;
				int64_t delta = error - mean;
				mean += delta / count;
				m2 += delta * (error - mean);
				int64_t stddev = (int64_t)sqrt(m2 / (count - 1));
				estimate = mean + stddev;
			}

			// spin lock
			auto start = high_resolution_clock::now();
			while ((high_resolution_clock::now() - start).count() < nanoSeconds);
		}


#else
		//------------------------------------------------------------------------------------
		//  Note: this function allows for high precision sleep (down to microsecond range)
		// this_thread::sleep_for is will behave unpredicatably for microsecond sleep times
		// this function is a little more heavy on the system but is much more accurate
		//------------------------------------------------------------------------------------
		// https://blat-blatnik.github.io/computerBear/making-accurate-sleep-function/
		//------------------------------------------------------------------------------------
		void preciseSleep(Nanoseconds sleepTime) {
			using namespace std::chrono;

			static int64_t estimate = Milliseconds(5).ToNanoseconds();
			static int64_t mean = Milliseconds(5).ToNanoseconds();
			static int64_t m2 = 0;
			static int64_t count = 1;
			int64_t nanoSeconds = (int64_t)sleepTime;

			while (nanoSeconds > estimate) {
				auto start = high_resolution_clock::now();
				std::this_thread::sleep_for(milliseconds(1));
				auto end = high_resolution_clock::now();

				int64_t observed = (end - start).count();
				nanoSeconds -= observed;

				++count;
				int64_t delta = observed - mean;
				mean += delta / count;
				m2 += delta * (observed - mean);
				int64_t stddev = (int64_t)sqrt(m2 / (count - 1));
				estimate = mean + stddev;
			}

			// spin lock
			auto start = high_resolution_clock::now();
			while ((high_resolution_clock::now() - start).count() < nanoSeconds);
		}
#endif
		//-------------------------------------------------------------------------------


		//-------------------------------------------------------------------------------
#pragma region RingBuffer Template
		template <typename Element>
		class RingBuffer
		{
		public:	
			RingBuffer(const size_t capacity) : buffer(capacity) {}

			void push(Element&& value)
			{
				buffer[tail] = std::forward<Element>(value);
				tail = (tail + 1) % buffer.size();
				if (head == tail) {
					head = (head + 1) % buffer.size();
				}
			}

			Element& front()
			{
				return buffer[head];
			}

			const Element& front() const
			{
				return buffer[head];
			}

			Element& back()
			{
				return buffer[(tail + buffer.size() - 1) % buffer.size()];
			}

			const Element& back() const
			{
				return buffer[(tail + buffer.size() - 1) % buffer.size()];
			}

			void pop()
			{
				head = (head + 1) % buffer.size();
			}

			bool empty() const
			{
				return head == tail;
			}

			size_t size() const
			{
				return tail >= head ? tail - head : buffer.size() - head + tail;
			}

			size_t capacity() const
			{
				return buffer.size();
			}

		private:
			std::vector<Element> buffer;
			size_t head = 0;
			size_t tail = 0;
		};
		//-------------------------------------------------------------------------------

#pragma endregion

	}
}