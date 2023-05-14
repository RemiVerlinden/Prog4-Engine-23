#pragma once

#define CONSOLE_NEWLINE()  dae::Logger::NewLine()
#define ENGINE_TRACE(...)  dae::Locator::GetLogger(dae::Locator::LoggerType::Engine).Trace(__VA_ARGS__)
#define ENGINE_DEBUG(...)  dae::Locator::GetLogger(dae::Locator::LoggerType::Engine).Debug(__VA_ARGS__)
#define ENGINE_INFO(...)   dae::Locator::GetLogger(dae::Locator::LoggerType::Engine).Info(__VA_ARGS__)
#define ENGINE_WARN(...)   dae::Locator::GetLogger(dae::Locator::LoggerType::Engine).Warn(__VA_ARGS__)
#define ENGINE_ERROR(...)  dae::Locator::GetLogger(dae::Locator::LoggerType::Engine).Error(__VA_ARGS__)

#define APP_TRACE(...)     dae::Locator::GetLogger(dae::Locator::LoggerType::App).Trace(__VA_ARGS__)
#define APP_DEBUG(...)     dae::Locator::GetLogger(dae::Locator::LoggerType::App).Debug(__VA_ARGS__)
#define APP_INFO(...)      dae::Locator::GetLogger(dae::Locator::LoggerType::App).Info(__VA_ARGS__)
#define APP_WARN(...)      dae::Locator::GetLogger(dae::Locator::LoggerType::App).Warn(__VA_ARGS__)
#define APP_ERROR(...)     dae::Locator::GetLogger(dae::Locator::LoggerType::App).Error(__VA_ARGS__)
