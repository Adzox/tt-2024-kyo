#pragma once

#include "Log/ILogger.hpp"
#include "Log/Level.hpp"

#include <string>

namespace Log {

extern ILogger* globalLogger;

static void SetGlobalLogger(ILogger& logger) {
  globalLogger = &logger;
}

static void SetGlobalLogLevel(Level level) {
  globalLogger->SetLogLevel(level);
}

template<typename... Args>
void Log(Level level, std::string_view message, Args&&... args) {
  globalLogger->LogImpl(level, std::vformat(message, std::make_format_args(args...)));
};

template <class... Args>
static void LogDebug(std::string msg, Args&&... args) {
  Log(Level::Debug, msg, args...);
}

template <class... Args>
static void LogInfo(std::string msg, Args&&... args) {
  Log(Level::Info, msg, args...);
}

template <class... Args>
static void LogWarn(std::string msg, Args&&... args) {
  Log(Level::Warning, msg, args...);
}

template <class... Args>
static void LogError(std::string msg, Args&&... args) {
  Log(Level::Error, msg, args...);
}

}  // namespace Log
