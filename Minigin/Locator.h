#pragma once
#include "LoggerManager.h"

// service locator pattern from game programming patterns book
namespace dae
{
	class LoggerManager;
	class Locator final
	{
	public:
		static LoggerManager& Logger();
	private:
		static LoggerManager m_Log;
	};
}