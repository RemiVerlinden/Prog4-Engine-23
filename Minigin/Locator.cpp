// new convention
#include "Locator.h"
#include "NullSoundSystem.hpp"

using namespace dae;

	Logger Locator::m_spLogEngineInstance("ENGINE");
	Logger Locator::m_spLogAppInstance("APP");

	std::unique_ptr<SoundSystem> Locator::m_spSoundInstance = std::make_unique<NullSoundSystem>();
	

	Logger& dae::Locator::GetLogger(LoggerType type) {	return type == LoggerType::Engine ? m_spLogEngineInstance : m_spLogAppInstance; }

	SoundSystem& dae::Locator::GetSoundSystem() { return *m_spSoundInstance; }

	void Locator::RegisterSoundSystem(std::unique_ptr<SoundSystem>&& spSoundSystem)
	{
		m_spSoundInstance = spSoundSystem == nullptr ? std::make_unique<NullSoundSystem>() : std::move(spSoundSystem);
	}

	//void dae::Locator::RegisterLogger(LoggerType type, std::unique_ptr<ILogger>&& spLogger)
	//{
	//	if (type == LoggerType::Engine)
	//		m_spLogEngineInstance = spLogger == nullptr ? std::make_unique<NullLogger>() : std::move(spLogger);
	//	else
	//		m_spLogAppInstance = spLogger == nullptr ? std::make_unique<NullLogger>() : std::move(spLogger);
	//}

