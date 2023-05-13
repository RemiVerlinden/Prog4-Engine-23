#pragma once
// new convention

#include "LoggerManager.h"
#include "SoundSystem.h"

namespace dae
{
	class Locator final
	{
		static Logger m_spLogEngineInstance;
		static Logger m_spLogAppInstance;
		static std::unique_ptr<SoundSystem> m_spSoundInstance;
	public:
		enum class LoggerType
		{
			Engine,
			App
		};

		static Logger& GetLogger(LoggerType type);
		static SoundSystem& GetSoundSystem();
		static void RegisterSoundSystem(std::unique_ptr<SoundSystem>&& spSoundSystem);
		//static void RegisterLogger(LoggerType type, std::unique_ptr<ILogger>&& spLogger);
	};
}