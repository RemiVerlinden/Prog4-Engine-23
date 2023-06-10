#include "Logger.h"
#include <iostream>
#include <chrono>
#include <sstream>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

using namespace engine;

Logger::Logger(const std::string& name, LogLevel level)
	: m_Name(name),
	m_Level(level),
	m_pHConsole(new HANDLE{})
{
	*m_pHConsole = GetStdHandle(STD_OUTPUT_HANDLE);
}

// I dont want to have the windows library in my header file, so I need to forward declare the HANDLE type and use RAW pointers
Logger::~Logger()
{
	delete m_pHConsole;
}

void Logger::SetLevel(LogLevel level) { m_Level = level; }

void Logger::Log(LogLevel level, const std::string& message) const
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

	SetConsoleTextAttribute(*m_pHConsole, color);
	std::cout << std::format("[{}] {} [{}]: ", time_stream.str(), m_Name, levelStr);

	SetConsoleTextAttribute(*m_pHConsole, White);
	std::cout << message << std::endl;
}
