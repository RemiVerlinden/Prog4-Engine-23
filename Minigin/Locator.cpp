// new convention
#include "Locator.h"
#include "NullSoundSystem.hpp"
#include "NullLogger.h"

using namespace dae;

	std::unique_ptr<ILogger> Locator::m_spLogEngineInstance= std::make_unique<Logger>("ENGINE");
	std::unique_ptr<ILogger> Locator::m_spLogAppInstance = std::make_unique<Logger>("APP");

	std::unique_ptr<ISoundSystem> Locator::m_spSoundInstance = std::make_unique<NullSoundSystem>();
	

	ILogger& dae::Locator::GetLogger(LoggerType type) {	return type == LoggerType::Engine ? *m_spLogEngineInstance : *m_spLogAppInstance; }

	ISoundSystem& dae::Locator::GetSoundSystem() { return *m_spSoundInstance; }

	void Locator::RegisterSoundSystem(std::unique_ptr<ISoundSystem>&& spSoundSystem)
	{
		m_spSoundInstance = spSoundSystem == nullptr ? std::make_unique<NullSoundSystem>() : std::move(spSoundSystem);
	}

	void dae::Locator::RegisterLogger(LoggerType type, std::unique_ptr<ILogger>&& spLogger)
	{
		if (type == LoggerType::Engine)
			m_spLogEngineInstance = spLogger == nullptr ? std::make_unique<NullLogger>() : std::move(spLogger);
		else
			m_spLogAppInstance = spLogger == nullptr ? std::make_unique<NullLogger>() : std::move(spLogger);
	}

