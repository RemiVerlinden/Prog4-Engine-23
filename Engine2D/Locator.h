#pragma once
// new convention

#include "Logger.h"
#include "SDL2SoundSystem.h" 
#include "LogSoundSystem.h"
#include "PhysicsSystem.h"

namespace engine
{
	class Locator final
	{
		static std::unique_ptr<ILogger> m_spLogEngineInstance;
		static std::unique_ptr<ILogger> m_spLogAppInstance;
		static std::unique_ptr<ISoundSystem> m_spSoundInstance;
		static std::unique_ptr<IPhysicsSystem> m_spPhysicsInstance;
	public:
		enum class LoggerType
		{
			Engine,
			App
		};

		static ILogger& GetLogger(LoggerType type);
		static ISoundSystem& GetSoundSystem();
		static IPhysicsSystem& GetPhysicsSystem();
		static void RegisterSoundSystem(std::unique_ptr<ISoundSystem>&& spSoundSystem);
		static void RegisterLogger(LoggerType type, std::unique_ptr<ILogger>&& spLogger);
		static void RegisterPhysicsSystem(std::unique_ptr<IPhysicsSystem>&& spPhysicsSystem);
	};
}

#include "LoggerMacros.h"
