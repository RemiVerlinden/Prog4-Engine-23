#include "SDL2_SoundSystem.h"
#include "audio.h"
#include "Locator.h"

using namespace dae;

SDL2_SoundSystem::SDL2_SoundSystem(const std::string& dataPath)
	:m_AudioThread(&SDL2_SoundSystem::ProcessQueue, this)
	, m_IsPlaying(true)
{
	m_FilePath = dataPath;
}

SDL2_SoundSystem::~SDL2_SoundSystem()
{
	m_IsPlaying = false;
	m_ConditionVariable.notify_one();
	m_AudioThread.join();

	/* End Simple-SDL2-Audio */
	endAudio();
}

void SDL2_SoundSystem::Play(const std::string& filename, float volume, bool isSoundEffect)
{
#ifdef _DEBUG
	const std::string extention = filename.substr(filename.find_last_of("."));
	if (extention != ".wav")
		ENGINE_ERROR("{} Unsuported file type SDL2_SoundSystem only supports .wav files", filename);
#endif

	std::unique_lock lock{ m_Mtx };

	AudioClip audio;
	audio.filename = m_FilePath + filename;
	audio.volume = static_cast<int>(std::clamp(volume, 0.f, 1.f) * SDL_MIX_MAXVOLUME);
	audio.isSFX = isSoundEffect;

	m_QueuedAudio.emplace(audio);

	m_ConditionVariable.notify_one();
}

void SDL2_SoundSystem::StopAll()
{
	//https://stackoverflow.com/questions/709146/how-do-i-clear-the-stdqueue-efficiently
	std::queue<AudioClip> empty;
	std::swap(m_QueuedAudio, empty);

	
	//stopMusic();
}

void SDL2_SoundSystem::StopMusic()
{
	//stopMusic();
}

void SDL2_SoundSystem::PauseAll()
{
	pauseAudio();
}

void SDL2_SoundSystem::UnpauseAll()
{
	unpauseAudio();
}

void SDL2_SoundSystem::ProcessQueue()
{
	std::unique_lock lock{ m_Mtx };
	lock.unlock();

	while (m_IsPlaying)
	{
		lock.lock();
		if (!m_QueuedAudio.empty())
		{
			const AudioClip audio = m_QueuedAudio.front();
			m_QueuedAudio.pop();
			lock.unlock();

			if (audio.isSFX)
				playSound(audio.filename.c_str(), audio.volume);
			else
				playMusic(audio.filename.c_str(), audio.volume);
		}
		else
		{
			//unlocks lock, then sleeps
			m_ConditionVariable.wait(lock); //wait until woken up
			//enter lock

			lock.unlock();
		}
	}
}
