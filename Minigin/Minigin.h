#pragma once
#include <string>
#include <functional>
#include "UpdateContext.h"

namespace dae
{
struct TimeStuff
{
	dae::Milliseconds						physicalSleepTime = 0.f;
	dae::Milliseconds						theoreticalSleepTime = 0.f;
	dae::Milliseconds						physicalFrameTime = 0.f;
	bool timerToggle = true;
};
	class Minigin
	{
	public:
		explicit Minigin(const std::string& dataPath);
		~Minigin();
		void Run(const std::function<void()>& load);

		Minigin(const Minigin& other) = delete;
		Minigin(Minigin&& other) = delete;
		Minigin& operator=(const Minigin& other) = delete;
		Minigin& operator=(Minigin&& other) = delete;
	};
}
