#pragma once
#include "ISoundSystem.h"
//#include "Logger.h"

namespace dae
{
	class NullSoundSystem final : public ISoundSystem
	{
	public:
		void Play(const std::string& /*filename*/, float /*volume*/, bool /*isSoundEffect*/ = true) override { /*Logger::GetInstance().LogWarning("Tried to use NullSoundSystem");*/ }
		void StopAll() override { /*Logger::GetInstance().LogWarning("Tried to use NullSoundSystem");*/ }
		void StopMusic() override { /*Logger::GetInstance().LogWarning("Tried to use NullSoundSystem");*/ }
		void PauseAll() override { /*Logger::GetInstance().LogWarning("Tried to use NullSoundSystem");*/ };
		void UnpauseAll() override { /*Logger::GetInstance().LogWarning("Tried to use NullSoundSystem");*/ };
	};
}

