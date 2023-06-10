#pragma once

#define CONSOLE_NEWLINE()  engine::Logger::NewLine()
#define ENGINE_TRACE(...)  engine::Locator::GetLogger(engine::Locator::LoggerType::Engine).Trace(__VA_ARGS__)
#define ENGINE_DEBUG(...)  engine::Locator::GetLogger(engine::Locator::LoggerType::Engine).Debug(__VA_ARGS__)
#define ENGINE_INFO(...)   engine::Locator::GetLogger(engine::Locator::LoggerType::Engine).Info(__VA_ARGS__)
#define ENGINE_WARN(...)   engine::Locator::GetLogger(engine::Locator::LoggerType::Engine).Warn(__VA_ARGS__)
#define ENGINE_ERROR(...)  engine::Locator::GetLogger(engine::Locator::LoggerType::Engine).Error(__VA_ARGS__)

#define APP_TRACE(...)     engine::Locator::GetLogger(engine::Locator::LoggerType::App).Trace(__VA_ARGS__)
#define APP_DEBUG(...)     engine::Locator::GetLogger(engine::Locator::LoggerType::App).Debug(__VA_ARGS__)
#define APP_INFO(...)      engine::Locator::GetLogger(engine::Locator::LoggerType::App).Info(__VA_ARGS__)
#define APP_WARN(...)      engine::Locator::GetLogger(engine::Locator::LoggerType::App).Warn(__VA_ARGS__)
#define APP_ERROR(...)     engine::Locator::GetLogger(engine::Locator::LoggerType::App).Error(__VA_ARGS__)
