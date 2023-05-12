#include "Locator.h"
#include "SoundSystem.h"

using namespace dae;

	LoggerManager Locator::m_Log;

	NullSoundSystem Locator::m_DefaultSoundSystem;
	SoundSystem* Locator::m_pSoundInstance = &m_DefaultSoundSystem;
	
	LoggerManager& dae::Locator::Logger() {	return m_Log; }

	SoundSystem& dae::Locator::GetSoundSystem() { return *m_pSoundInstance; }

	void Locator::RegisterSoundSystem(SoundSystem* soundSystem)
	{
		if (m_pSoundInstance != &m_DefaultSoundSystem)
			delete m_pSoundInstance;
		m_pSoundInstance = soundSystem == nullptr ? &m_DefaultSoundSystem : soundSystem;
	}

