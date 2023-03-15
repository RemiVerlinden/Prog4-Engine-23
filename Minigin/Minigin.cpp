#include <stdexcept>
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "Minigin.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"


#include "scene.h"
#include <thread>
#include "Timers.h"
#include <iostream>
#include "UpdateContext.h"

SDL_Window* g_window{};

void PrintSDLVersion()
{
	SDL_version version{};
	SDL_VERSION(&version);
	printf("We compiled against SDL version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	SDL_GetVersion(&version);
	printf("We are linking against SDL version %u.%u.%u.\n",
		version.major, version.minor, version.patch);

	SDL_IMAGE_VERSION(&version);
	printf("We compiled against SDL_image version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	version = *IMG_Linked_Version();
	printf("We are linking against SDL_image version %u.%u.%u.\n",
		version.major, version.minor, version.patch);

	SDL_TTF_VERSION(&version)
		printf("We compiled against SDL_ttf version %u.%u.%u ...\n",
			version.major, version.minor, version.patch);

	version = *TTF_Linked_Version();
	printf("We are linking against SDL_ttf version %u.%u.%u.\n",
		version.major, version.minor, version.patch);
}

dae::Minigin::Minigin(const std::string& dataPath)
{
	PrintSDLVersion();

	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	g_window = SDL_CreateWindow(
		"Programming 4 assignment",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		640,
		480,
		SDL_WINDOW_OPENGL
	);
	if (g_window == nullptr)
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	Renderer::GetInstance().Init(g_window);

	ResourceManager::GetInstance().Init(dataPath);
}

dae::Minigin::~Minigin()
{
	Renderer::GetInstance().Destroy();
	SDL_DestroyWindow(g_window);
	g_window = nullptr;
	SDL_Quit();
}

void timerSleep(double seconds) {
	using namespace std::chrono;

	static HANDLE timer = CreateWaitableTimer(NULL, FALSE, NULL);
	static double estimate = 5e-3;
	static double mean = 5e-3;
	static double m2 = 0;
	static int64_t count = 1;

	while (seconds - estimate > 1e-7) {
		double toWait = seconds - estimate;
		LARGE_INTEGER due;
		due.QuadPart = -int64_t(toWait * 1e7);
		auto start = high_resolution_clock::now();
		SetWaitableTimerEx(timer, &due, 0, NULL, NULL, NULL, 0);
		WaitForSingleObject(timer, INFINITE);
		auto end = high_resolution_clock::now();

		double observed = (end - start).count() / 1e9;
		seconds -= observed;

		++count;
		double error = observed - toWait;
		double delta = error - mean;
		mean += delta / count;
		m2 += delta * (error - mean);
		double stddev = sqrt(m2 / (count - 1));
		estimate = mean + stddev;
	}

	// spin lock
	auto start = high_resolution_clock::now();
	while ((high_resolution_clock::now() - start).count() / 1e9 < seconds);
}

void preciseSleep(double seconds) {
	using namespace std;
	using namespace std::chrono;

	static double estimate = 5e-3;
	static double mean = 5e-3;
	static double m2 = 0;
	static int64_t count = 1;

	while (seconds > estimate) {
		auto start = high_resolution_clock::now();
		this_thread::sleep_for(milliseconds(1));
		auto end = high_resolution_clock::now();

		double observed = (end - start).count() / 1e9;
		seconds -= observed;

		++count;
		double delta = observed - mean;
		mean += delta / count;
		m2 += delta * (observed - mean);
		double stddev = sqrt(m2 / (count - 1));
		estimate = mean + stddev;
	}

	// spin lock
	auto start = high_resolution_clock::now();
	while ((high_resolution_clock::now() - start).count() / 1e9 < seconds);
}


void dae::Minigin::Run(const std::function<void()>& load)
{
	load();

	auto& renderer = Renderer::GetInstance();
	auto& sceneManager = SceneManager::GetInstance();
	auto& input = InputManager::GetInstance();


	//==========TIMER-VARIABLES=========

	UpdateContext						updateContext{};
	float								accumulator = 0.f; // time since last fixed timestep

	TimeStuff timeProps{};
	//==================================

	sceneManager.Initialize(); // IMPORTANT

	bool doContinue = true;
	while (doContinue)
	{
		Milliseconds frameTime = 0;
		{
			ScopedTimer<PlatformClock> frameTimer(frameTime);

			accumulator += updateContext.GetDeltaTime();

			//============INPUT=================
			doContinue = input.ProcessInput();
			//==================================


			//=============UPDATE===============
			sceneManager.CheckNewActiveGameScene(); // before we start to update the active scene, check if we have a new one

			while (accumulator >= updateContext.GetFixedTimeStep())
			{
				sceneManager.FixedUpdate(updateContext);
				accumulator -= updateContext.GetFixedTimeStep();
			}
			sceneManager.Update(updateContext);
			sceneManager.LateUpdate(updateContext);
			//==================================


			//============RENDER================
			renderer.Render(updateContext, timeProps);
			//==================================
		}

		//==========UPDATE-TIME=============
		// Frame rate limiter
		if (updateContext.HasFrameRateLimit())
		{
			const float minimumFrameTime = updateContext.GetLimitedFrameTime();
			timeProps.physicalSleepTime = 0;
				//Microseconds temp;
			if (frameTime < minimumFrameTime)
			{
				ScopedTimer<PlatformClock> sleepTimer(timeProps.physicalSleepTime);

				timeProps.theoreticalSleepTime = minimumFrameTime - frameTime;
				//temp = theoreticalSleepTime.ToMicroseconds();
				if (timeProps.timerToggle)
					preciseSleep(timeProps.theoreticalSleepTime.ToSeconds());
				else
					timerSleep(timeProps.theoreticalSleepTime.ToSeconds());

				//std::this_thread::sleep_for(std::chrono::microseconds(theoreticalSleepTime));
				timeProps.physicalFrameTime = frameTime;
			}
				frameTime += timeProps.physicalSleepTime;
				//frameTime += physicalSleepTime;
				timeProps.physicalFrameTime += timeProps.physicalSleepTime;
		}
		//==================================
		updateContext.UpdateDeltaTime(frameTime);
		EngineClock::Update(frameTime);
	}
}
