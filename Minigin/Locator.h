#pragma once
// new convention

#include "Logger.h"
#include "SDL2_SoundSystem.h" 
#include "SimpleSDL2SoundSystem.h"


namespace dae
{
	class Locator final
	{
		static std::unique_ptr<ILogger> m_spLogEngineInstance;
		static std::unique_ptr<ILogger> m_spLogAppInstance;
		static std::unique_ptr<SoundSystem> m_spSoundInstance;
	public:
		enum class LoggerType
		{
			Engine,
			App
		};

		static ILogger& GetLogger(LoggerType type);
		static SoundSystem& GetSoundSystem();
		static void RegisterSoundSystem(std::unique_ptr<SoundSystem>&& spSoundSystem);
		static void RegisterLogger(LoggerType type, std::unique_ptr<ILogger>&& spLogger);
	};
}

#include "LoggerMacros.h"
