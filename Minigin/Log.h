#pragma once

#include <format>
#include <iostream>
#include <memory>
#include <string>
#include <chrono>
#include <iomanip>
#include <sstream>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace dae
{
	enum class LogLevel
	{
		Trace,
		Debug,
		Info,
		Warn,
		Error
	};

	//----------------------------------------------------------------------------------------------------------------------
	class Logger
	{
	public:
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


		Logger(const std::string& name, LogLevel level = LogLevel::Trace)
			: m_Name(name),
			m_Level(level),
			m_HConsole(GetStdHandle(STD_OUTPUT_HANDLE))
		{
		}


		void SetLevel(LogLevel level) { m_Level = level; }

		template<typename... Args>
		void Trace(std::format_string<Args...> fmt, Args &&... args)
		{
			Log(LogLevel::Trace, fmt, std::forward<Args>(args)...);
		}

		template<typename... Args>
		void Debug(std::format_string<Args...> fmt, Args &&... args)
		{
			Log(LogLevel::Debug, fmt, std::forward<Args>(args)...);
		}

		template<typename... Args>
		void Info(std::format_string<Args...> fmt, Args &&... args)
		{
			Log(LogLevel::Info, fmt, std::forward<Args>(args)...);
		}

		template<typename... Args>
		void Warn(std::format_string<Args...> fmt, Args &&... args)
		{
			Log(LogLevel::Warn, fmt, std::forward<Args>(args)...);
		}
		template<typename... Args>
		void Error(std::format_string<Args...> fmt, Args &&... args)
		{
			Log(LogLevel::Error, fmt, std::forward<Args>(args)...);
		};

		static void NewLine()
		{
			std::cout << '\n';
		}

	private:

		template<typename... Args>
		void Log(LogLevel level, std::format_string<Args...> fmt, [[maybe_unused]] Args&&... args) const
		{
			if (level < m_Level) return;

			auto now = std::chrono::system_clock::now();
			auto now_time_t = std::chrono::system_clock::to_time_t(now);
			std::tm now_tm;
			localtime_s(&now_tm, &now_time_t);

			// Format the time
			std::stringstream time_stream;
			time_stream << std::put_time(&now_tm, "%H:%M:%S");

			std::string levelStr;
			WORD color = White;
			switch (level)
			{
				case LogLevel::Trace: levelStr = "TRACE";	color = Cyan; break;
				case LogLevel::Debug: levelStr = "DEBUG";	color = Gray; break;
				case LogLevel::Info: levelStr = "INFO";		color = Green; break;
				case LogLevel::Warn: levelStr = "WARN";		color = Yellow; break;
				case LogLevel::Error: levelStr = "ERROR";	color = Red; break;
			}

			std::string message = std::format(fmt, std::forward<Args>(args)...);

			SetConsoleTextAttribute(m_HConsole, color);
			std::cout << std::format("[{}] {} [{}]: ", time_stream.str(), m_Name, levelStr);

			SetConsoleTextAttribute(m_HConsole, White);
			std::cout << message << std::endl;
		}

		std::string m_Name;
		LogLevel m_Level;
		HANDLE m_HConsole;
	};
	//----------------------------------------------------------------------------------------------------------------------




	//----------------------------------------------------------------------------------------------------------------------
	class Log
	{
	public:
		Log() : m_EngineLogger("ENGINE"), m_AppLogger("APP") {}

		Logger* Engine() { return &m_EngineLogger; }
		Logger* App() { return &m_AppLogger; }

	private:
		Logger m_EngineLogger;
		Logger m_AppLogger;
	};
	//----------------------------------------------------------------------------------------------------------------------

} // namespace dae

#define CONSOLE_NEWLINE()  dae::Logger::NewLine()
#define ENGINE_TRACE(...)  dae::Locator::Logger().Engine()->Trace(__VA_ARGS__)
#define ENGINE_INFO(...)   dae::Locator::Logger().Engine()->Info(__VA_ARGS__)
#define ENGINE_WARN(...)   dae::Locator::Logger().Engine()->Warn(__VA_ARGS__)
#define ENGINE_ERROR(...)  dae::Locator::Logger().Engine()->Error(__VA_ARGS__)

#define APP_TRACE(...)     dae::Locator::Logger().App()->Trace(__VA_ARGS__)
#define APP_INFO(...)      dae::Locator::Logger().App()->Info(__VA_ARGS__)
#define APP_WARN(...)      dae::Locator::Logger().App()->Warn(__VA_ARGS__)
#define APP_ERROR(...)     dae::Locator::Logger().App()->Error(__VA_ARGS__)
