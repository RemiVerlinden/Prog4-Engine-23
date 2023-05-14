#pragma once
#include "SoundSystem.h"
#include <memory>

namespace dae
{
	class LogSoundSystem final : public SoundSystem
	{
	public:
		LogSoundSystem(std::unique_ptr<SoundSystem>&& systemToLog);
		~LogSoundSystem() override;

		LogSoundSystem(const LogSoundSystem& other) = delete;
		LogSoundSystem(LogSoundSystem&& other) noexcept = delete;
		LogSoundSystem& operator=(const LogSoundSystem& other) = delete;
		LogSoundSystem& operator=(LogSoundSystem&& other) noexcept = delete;

		void Play(const std::string& filename, float volume, bool isSoundEffect) override;
		void StopAll() override;
		void StopMusic() override;
		void PauseAll() override;
		void UnpauseAll() override;

	private:
		std::unique_ptr<SoundSystem> m_upSystem;
	};
}
