target_sources(${LIBRARY_NAME} PUBLIC FILE_SET HEADERS BASE_DIRS "../../include/Input" FILES
    "../../include/Input/BaseInputManager.hpp"
    "../../include/Input/InputEvents.hpp")

target_sources(${LIBRARY_NAME} PRIVATE
    "BaseInputManager.cpp")