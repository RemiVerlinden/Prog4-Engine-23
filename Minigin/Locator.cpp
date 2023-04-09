#include "Locator.h"

#include "Log.h"

namespace dae
{
	Log Locator::m_Log;
	
	Log& dae::Locator::Logger()
	{
		return m_Log;
	}
}
