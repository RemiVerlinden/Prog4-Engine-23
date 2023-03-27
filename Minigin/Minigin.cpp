#include <stdexcept>
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "Minigin.h"
#include "InputSystem.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"


#include "scene.h"
#include <thread>
#include "Timers.h"
#include <iostream>
#include "UpdateContext.h"

#include "Utils.hpp"

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



void dae::Minigin::Run(const std::function<void()>& load)
{

	auto& input = Input::InputSystem::GetInstance();
	Input::InputSystem::GetInstance().Initialize();

	load();

	auto& renderer = Renderer::GetInstance();
	auto& sceneManager = SceneManager::GetInstance();


	//==========TIMER-VARIABLES=========

	UpdateContext						updateContext{};
	float								accumulator = 0.f; // time since last fixed timestep

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
			doContinue = input.ProcessInput(updateContext);
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
			renderer.Render(updateContext);
			//==================================
		}

		//==========UPDATE-TIME=============
		// Frame rate limiter
		if (updateContext.HasFrameRateLimit())
		{
			const float minimumFrameTime = updateContext.GetLimitedFrameTime();
			Milliseconds measuredSleepTime;

			if (frameTime < minimumFrameTime)
			{
				ScopedTimer<PlatformClock> sleepTimer(measuredSleepTime);

				Milliseconds sleepTime = minimumFrameTime - frameTime;
				Utils::preciseSleep(sleepTime.ToNanoseconds());
			}
				frameTime += measuredSleepTime;
		}
		//==================================

		updateContext.UpdateDeltaTime(frameTime);
		EngineClock::Update(frameTime);
	}
}
