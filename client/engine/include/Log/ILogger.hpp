#pragma once

#include "Log/Level.hpp"

#include <format>
#include <string>

namespace Log {

class ILogger {
 public:

  virtual ~ILogger() = default;

  virtual void LogImpl(Level level, std::string message) = 0;
  virtual void SetLogLevel(Level level) = 0;
};

}  // namespace Log
