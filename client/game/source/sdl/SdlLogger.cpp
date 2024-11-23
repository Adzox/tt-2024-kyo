#pragma once

#include "sdl/SdlLogger.hpp"

#include <SDL3/SDL_log.h>

namespace {

SDL_LogPriority GetLogPriority(Log::Level level) {
  switch (level) {
    case Log::Level::Debug:
      return SDL_LOG_PRIORITY_DEBUG;
    case Log::Level::Info:
      return SDL_LOG_PRIORITY_INFO;
    case Log::Level::Warning:
      return SDL_LOG_PRIORITY_WARN;
    case Log::Level::Error:
      return SDL_LOG_PRIORITY_ERROR;
    default:
      return SDL_LOG_PRIORITY_VERBOSE;
  }
}

}  // namespace

void SdlLogger::LogImpl(Log::Level level, std::string message) {
  SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, GetLogPriority(level), message.c_str());
};

void SdlLogger::SetLogLevel(Log::Level level) {
  SDL_SetLogPriority(SDL_LOG_CATEGORY_APPLICATION, GetLogPriority(level));
};
