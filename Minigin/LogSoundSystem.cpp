#include "LogSoundSystem.h"
#include "Locator.h"

using namespace dae;
LogSoundSystem::LogSoundSystem(std::unique_ptr<SoundSystem>&& upSystemToLog)
{
	m_upSystem = std::move(upSystemToLog);
	ENGINE_INFO("Created sound system");
}

LogSoundSystem::~LogSoundSystem()
{
	ENGINE_INFO("destroyed sound system\n");
}

void LogSoundSystem::Play(const std::string& filename, float volume, bool isSoundEffect)
{
	m_upSystem->Play(filename, volume, isSoundEffect);
	if (isSoundEffect)
		ENGINE_INFO("playing sound: {}", filename);
	else
		ENGINE_INFO("playing music: {}", filename);
}

void LogSoundSystem::StopAll()
{
	m_upSystem->StopAll();
	ENGINE_INFO("Stopped all sound");
}

void LogSoundSystem::StopMusic()
{
	m_upSystem->StopMusic();
	ENGINE_INFO("Stopped music");
}

void LogSoundSystem::PauseAll()
{
	m_upSystem->PauseAll();
	ENGINE_INFO("Paused all sound");
}

void LogSoundSystem::UnpauseAll()
{
	m_upSystem->UnpauseAll();
	ENGINE_INFO("Unpaused all sound");
}
