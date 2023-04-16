#pragma once
#include "Log.h"

// service locator pattern from game programming patterns book
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