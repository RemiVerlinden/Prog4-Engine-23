#pragma once
// new convention

#include "Logger.h"
#include "SDL2SoundSystem.h" 
#include "LogSoundSystem.h"

namespace dae
{
	class Locator final
	{
		static std::unique_ptr<ILogger> m_spLogEngineInstance;
		static std::unique_ptr<ILogger> m_spLogAppInstance;
		static std::unique_ptr<ISoundSystem> m_spSoundInstance;
	public:
		enum class LoggerType
		{
			Engine,
			App
		};

		static ILogger& GetLogger(LoggerType type);
		static ISoundSystem& GetSoundSystem();
		static void RegisterSoundSystem(std::unique_ptr<ISoundSystem>&& spSoundSystem);
		static void RegisterLogger(LoggerType type, std::unique_ptr<ILogger>&& spLogger);
	};
}

#include "LoggerMacros.h"
