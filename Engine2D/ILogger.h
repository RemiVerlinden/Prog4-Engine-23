#pragma once
#include <format>

class ILogger
{
protected:
	enum class LogLevel
	{
		Trace,
		Debug,
		Info,
		Warn,
		Error
	};

public:
	virtual ~ILogger() = default;
	virtual void SetLevel(LogLevel level) = 0;

	template<typename... Args>
	void Trace(std::format_string<Args...> fmt, Args &&... args) const
	{
		std::string message = std::format(fmt, std::forward<Args>(args)...);
		Log(LogLevel::Trace, message);
	}

	template<typename... Args>
	void Debug(std::format_string<Args...> fmt, Args &&... args) const
	{
		std::string message = std::format(fmt, std::forward<Args>(args)...);
		Log(LogLevel::Debug, message);
	}

	template<typename... Args>
	void Info(std::format_string<Args...> fmt, Args &&... args) const
	{
		std::string message = std::format(fmt, std::forward<Args>(args)...);
		Log(LogLevel::Info, message);
	}

	template<typename... Args>
	void Warn(std::format_string<Args...> fmt, Args &&... args) const
	{
		std::string message = std::format(fmt, std::forward<Args>(args)...);
		Log(LogLevel::Warn, message);
	}

	template<typename... Args>
	void Error(std::format_string<Args...> fmt, Args &&... args) const
	{
		std::string message = std::format(fmt, std::forward<Args>(args)...);
		Log(LogLevel::Error, message);
	};

	static void NewLine();

protected:
	virtual void Log(LogLevel level, const std::string& message) const = 0;
};