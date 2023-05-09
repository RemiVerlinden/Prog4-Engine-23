#include "Locator.h"

#include "LoggerManager.h"

namespace dae
{
	LoggerManager Locator::m_Log;
	
	LoggerManager& dae::Locator::Logger()
	{
		return m_Log;
	}
}
