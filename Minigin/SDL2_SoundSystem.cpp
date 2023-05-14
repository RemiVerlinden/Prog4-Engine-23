#include "SDL2_SoundSystem.h"
#include "SDLAudioWrapper.h"
#include "Locator.h"
#include <filesystem>

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
    if (m_AudioThread.joinable())
        m_AudioThread.join();
}

void SDL2_SoundSystem::Play(const std::string& filename, float volume, bool isSoundEffect)
{
#ifdef _DEBUG
	const std::string extention = filename.substr(filename.find_last_of("."));
	if (extention != ".wav")
		ENGINE_ERROR("{} Unsuported file type SDL2_SoundSystem only supports .wav files", filename);
#endif

	AudioClip audio;
	audio.filename = m_FilePath + filename;
	audio.volume = static_cast<int>(std::clamp(volume, 0.f, 1.f) * SDL_MIX_MAXVOLUME);
	audio.isSFX = isSoundEffect;

	if (!std::filesystem::exists(audio.filename))
	{
		ENGINE_ERROR("no sound file at: {} exists", audio.filename);
		return;
	}

    std::unique_lock<std::mutex> lock(m_Mtx);
    m_SoundQueue.push(new Sound{ audio.filename });
    lock.unlock();

    // Notify the condition variable
    m_ConditionVariable.notify_one();
}

void SDL2_SoundSystem::ProcessQueue()
{
    while (m_IsPlaying)
    {
        std::unique_lock<std::mutex> lock(m_Mtx);

        // Wait until a sound is added to the queue
        while (m_SoundQueue.empty())
        {
            m_ConditionVariable.wait(lock);
        }

        // Get the next sound from the queue
        Sound* soundEffect = m_SoundQueue.front();
        m_SoundQueue.pop();
        lock.unlock();

        // Play the sound
        soundEffect->PlaySound();

        // Delete the sound when finished
    }
}