#pragma once

#include <Log/ILogger.hpp>

class SdlLogger : public Log::ILogger {
public:
  void LogImpl(Log::Level level, std::string message) override;
  void SetLogLevel(Log::Level level) override;
};
