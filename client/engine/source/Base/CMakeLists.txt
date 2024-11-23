target_sources(${LIBRARY_NAME} PUBLIC FILE_SET HEADERS BASE_DIRS "../../include/Base" FILES
    "../../include/Base/BaseGame.hpp"
    "../../include/Base/BasicEvents.hpp"
    "../../include/Base/GameConfig.hpp")

target_sources(${LIBRARY_NAME} PRIVATE
    "BaseGame.cpp")
