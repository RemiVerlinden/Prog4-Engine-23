#pragma once
#include "ILogger.h"

class NullLogger final : public ILogger
{
public:
	virtual ~NullLogger() = default;
	void SetLevel(LogLevel) override {};
protected:
	void Log(LogLevel, const std::string&) const override {};
};