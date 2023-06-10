#pragma once
#include <string>

namespace engine
{
	class ISoundSystem
	{
	public:
		virtual ~ISoundSystem() = default;

		virtual void Play(const std::string& filename, float volume, bool isSoundEffect = true) = 0;
		virtual void StopAll() = 0;
		virtual void StopMusic() = 0;
		virtual void PauseAll() = 0;
		virtual void UnpauseAll() = 0;
	};

}

