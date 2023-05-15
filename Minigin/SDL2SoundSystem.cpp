#include "SDL2SoundSystem.h"
#include "SDLAudioWrapper.h"


using namespace dae;

SDL2SoundSystem::SDL2SoundSystem(const std::string& dataPath)
	:m_AudioThread(&SDL2SoundSystem::ProcessQueue, this)
	,m_Running(true)
{
	m_DataPath = dataPath;
}

SDL2SoundSystem::~SDL2SoundSystem()
{
	m_Running = false;
	m_ConditionVariable.notify_one();
	m_AudioThread.join();

	/* End Simple-SDL2-Audio */
	endAudio();
}

void SDL2SoundSystem::Play(const std::string& filename, float volume, bool isSoundEffect)
{

	std::unique_lock lock{ m_Mtx };

	queuedAudio audio;
	audio.filename = m_DataPath + filename;
	audio.volume = static_cast<int>(std::clamp(volume, 0.f, 1.f) * SDL_MIX_MAXVOLUME);
	audio.isSoundEffect = isSoundEffect;

	m_AudioQueue.emplace(audio);

	m_ConditionVariable.notify_one();
}

void SDL2SoundSystem::StopAll()
{
	//https://stackoverflow.com/questions/709146/how-do-i-clear-the-stdqueue-efficiently
	std::queue<queuedAudio> empty;
	std::swap(m_AudioQueue, empty);

	stopMusic();
}

void SDL2SoundSystem::StopMusic()
{
	stopMusic();
}

void SDL2SoundSystem::PauseAll()
{
	pauseAudio();
}

void SDL2SoundSystem::UnpauseAll()
{
	unpauseAudio();
}

void SDL2SoundSystem::ProcessQueue()
{
	std::unique_lock lock{ m_Mtx };
	lock.unlock();

	while (m_Running)
	{
		lock.lock();
		if (!m_AudioQueue.empty())
		{
			const queuedAudio audio = m_AudioQueue.front();
			m_AudioQueue.pop();
			lock.unlock();

			if (audio.isSoundEffect)
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
