#pragma once
#include "Log.h"

namespace dae
{
	class Locator
	{
	public:
		static Log& Logger() { return m_Log; }
	private:
		inline static Log m_Log;
	};
}