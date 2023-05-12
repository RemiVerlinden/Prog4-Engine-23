#pragma once
#include "SoundSystem.h"
#include <queue>
namespace dae 
{
	class SDL2_SoundSystem : public SoundSystem
	{
		struct AudioClip
		{
			std::string filename{};
			int volume{};
			bool isSFX{}; // multiple sound effects can play but with music, only 1 plays at a time
		};
	public:
		SDL2_SoundSystem(const std::string& filePath);
		~SDL2_SoundSystem() override;

		SDL2_SoundSystem(const SDL2_SoundSystem& other) = delete;
		SDL2_SoundSystem(SDL2_SoundSystem&& other) noexcept = delete;
		SDL2_SoundSystem& operator=(const SDL2_SoundSystem& other) = delete;
		SDL2_SoundSystem& operator=(SDL2_SoundSystem&& other) noexcept = delete;

		void Play(const std::string& filename, float volume, bool isSoundEffect = true) override;
		void StopAll() override;
		virtual void StopMusic() override;
		virtual void PauseAll() override;
		virtual void UnpauseAll() override;
	private:

		std::string m_FilePath;
		std::queue<AudioClip> `;
		bool m_IsPlaying;
	};
}