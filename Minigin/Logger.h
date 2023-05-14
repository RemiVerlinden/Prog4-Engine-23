#pragma once
#include "ILogger.h"


typedef void* HANDLE; // Forward declaration for HANDLE

namespace dae
{
	class Logger final : public ILogger
	{
		enum ConsoleColor
		{
			Black = 0,
			DarkBlue = 1,
			DarkGreen = 2,
			DarkCyan = 3,
			DarkRed = 4,
			DarkMagenta = 5,
			DarkYellow = 6,
			Gray = 7,
			DarkGray = 8,
			Blue = 9,
			Green = 10,
			Cyan = 11,
			Red = 12,
			Magenta = 13,
			Yellow = 14,
			White = 15
		};
	public:
		Logger(const std::string& name, LogLevel level = LogLevel::Trace);
		virtual ~Logger();

		void SetLevel(LogLevel level) override;

	protected:
		void Log(LogLevel level, const std::string& message) const override;

		std::string m_Name;
		LogLevel m_Level;
		HANDLE* m_pHConsole;
	};
}
