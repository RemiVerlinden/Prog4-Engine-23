#pragma once
#include <mutex>
#include <queue>
#include "ISoundSystem.h"

//very simple audio system
namespace dae 
{

class SDL2SoundSystem final : public ISoundSystem
{
	struct queuedAudio
	{
		std::string filename{};
		int volume{};
		bool isSoundEffect{}; //multiple sound effects can play but with music, only 1 at a time plays
	};

public:
	SDL2SoundSystem(const std::string& dataPath);
	~SDL2SoundSystem() override;

	SDL2SoundSystem(const SDL2SoundSystem& other) = delete;
	SDL2SoundSystem(SDL2SoundSystem&& other) noexcept = delete;
	SDL2SoundSystem& operator=(const SDL2SoundSystem& other) = delete;
	SDL2SoundSystem& operator=(SDL2SoundSystem&& other) noexcept = delete;

	void Play(const std::string& filename, float volume, bool isSoundEffect = true) override;
	void StopAll() override;
	void StopMusic() override;
	void PauseAll() override;
	void UnpauseAll() override;

private:
	void ProcessQueue();

	std::string m_DataPath;
	std::queue<queuedAudio> m_AudioQueue;

	std::thread m_AudioThread;
	std::mutex m_Mtx;
	std::condition_variable m_ConditionVariable;
	bool m_Running;
};

}


