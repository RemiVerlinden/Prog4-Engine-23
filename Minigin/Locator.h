#pragma once
#include "Log.h"

namespace dae
{
	class Log;
	class Locator
	{
	public:
		static Log& Logger();
	private:
		static Log m_Log;
	};
}