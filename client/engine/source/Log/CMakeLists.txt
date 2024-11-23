target_sources(${LIBRARY_NAME} PUBLIC FILE_SET HEADERS BASE_DIRS "../../include/Log" FILES
    "../../include/Log/ILogger.hpp"
    "../../include/Log/Log.hpp")

target_sources(${LIBRARY_NAME} PRIVATE
    "Log.cpp")