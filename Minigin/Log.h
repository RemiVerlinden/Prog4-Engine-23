//#pragma once
//
//#include "spdlog\spdlog.h"
//#include "spdlog\sinks\stdout_color_sinks.h"
//
//#include <memory>
//
//
//namespace dae
//{
//
//	class Log
//	{
//	public:
//		Log();
//
//		spdlog::logger* Engine() { return m_EngineLogger.get(); }
//		spdlog::logger* App() { return m_AppLogger.get(); }
//	private:
//		std::shared_ptr<spdlog::logger> m_EngineLogger;
//		std::shared_ptr<spdlog::logger> m_AppLogger;
//	};
//
//}
//
//#define ENGINE_TRACE(...)	dae::Locator::Logger().Engine()->trace(__VA_ARGS__)
//#define ENGINE_INFO(...)	dae::Locator::Logger().Engine()->info(__VA_ARGS__)
//#define ENGINE_WARN(...)	dae::Locator::Logger().Engine()->warn(__VA_ARGS__)
//#define ENGINE_ERROR(...)	dae::Locator::Logger().Engine()->error(__VA_ARGS__)
//
//#define APP_TRACE(...)		dae::Locator::Logger().App()->trace(__VA_ARGS__)
//#define APP_INFO(...)		dae::Locator::Logger().App()->info(__VA_ARGS__)
//#define APP_WARN(...)		dae::Locator::Logger().App()->warn(__VA_ARGS__)
//#define APP_ERROR(...)		dae::Locator::Logger().App()->error(__VA_ARGS__)


#pragma once

#include <iostream>
#include <format>
#include <memory>
#include <string>
#include "Time.h"

namespace dae {

	enum class LogLevel
	{
		Trace,
		Info,
		Warn,
		Error
	};

	class Logger
	{
	public:
		Logger(const std::string& name, LogLevel level = LogLevel::Trace)
			: m_Name(name), m_Level(level)
		{
		}

		void SetLevel(LogLevel level) { m_Level = level; }

		template<typename... Args>
		void Trace(Args&&... args) { Log(LogLevel::Trace, std::forward<Args>(args)...); }

		template<typename... Args>
		void Info(Args&&... args) { Log(LogLevel::Info, std::forward<Args>(args)...); }

		template<typename... Args>
		void Warn(Args&&... args) { Log(LogLevel::Warn, std::forward<Args>(args)...); }

		template<typename... Args>
		void Error(Args&&... args) { Log(LogLevel::Error, std::forward<Args>(args)...); }

	private:
		template<typename... Args>
		void Log(LogLevel level, Args&&... args)
		{
			if (level < m_Level) return;

			float runTime = dae::PlatformClock::GetTimeInSeconds().ToFloat();

			std::string levelStr;
			switch (level)
			{
				case LogLevel::Trace: levelStr = "TRACE"; break;
				case LogLevel::Info: levelStr = "INFO"; break;
				case LogLevel::Warn: levelStr = "WARN"; break;
				case LogLevel::Error: levelStr = "ERROR"; break;
			}

			std::string message = std::format(std::forward<Args>(args)...);
			//std::string message = std::format(formatString, std::forward<Args>(args)...);
			std::cout << std::format("[{:.2f}] {} [{}] {}: {}\n",
				runTime,
				m_Name,
				levelStr,
				std::this_thread::get_id(),
				message);
			//std::cout << message;
		}

		std::string m_Name;
		LogLevel m_Level;
	};

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

} // namespace dae

#define ENGINE_TRACE(...)  dae::Locator::Logger().Engine()->Trace(__VA_ARGS__)
#define ENGINE_INFO(...)   dae::Locator::Logger().Engine()->Info(__VA_ARGS__)
#define ENGINE_WARN(...)   dae::Locator::Logger().Engine()->Warn(__VA_ARGS__)
#define ENGINE_ERROR(...)  dae::Locator::Logger().Engine()->Error(__VA_ARGS__)

#define APP_TRACE(...)     dae::Locator::Logger().App()->Trace(__VA_ARGS__)
#define APP_INFO(...)      dae::Locator::Logger().App()->Info(__VA_ARGS__)
#define APP_WARN(...)      dae::Locator::Logger().App()->Warn(__VA_ARGS__)
#define APP_ERROR(...)     dae::Locator::Logger().App()->Error(__VA_ARGS__)
