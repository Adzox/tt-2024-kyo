target_sources(${LIBRARY_NAME} PUBLIC FILE_SET HEADERS BASE_DIRS "../../include/Events" FILES
    "../../include/Events/Event.hpp"
    "../../include/Events/EventManager.hpp"
    "../../include/Events/IEventListener.hpp")
