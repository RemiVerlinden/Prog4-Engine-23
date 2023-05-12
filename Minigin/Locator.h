#pragma once
#include "LoggerManager.h"
#include "NullSoundSystem.hpp"

namespace dae
{
	class LoggerManager;

	class Locator final
	{
		static LoggerManager m_Log;
		static SoundSystem* m_pSoundInstance;
		static NullSoundSystem m_DefaultSoundSystem;
	public:
		static LoggerManager& Logger();
		static SoundSystem& GetSoundSystem();
		static void RegisterSoundSystem(SoundSystem* pSoundSystem);
	};
}