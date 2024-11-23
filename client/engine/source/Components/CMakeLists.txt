target_sources(${LIBRARY_NAME} PUBLIC FILE_SET HEADERS BASE_DIRS "../../include/Components" FILES
    "../../include/Components/Transform.hpp"
    "../../include/Components/Image.hpp"
    "../../include/Components/ClickableArea.hpp")

target_sources(${LIBRARY_NAME} PRIVATE
    "Image.cpp"
    "Transform.cpp"
    "ClickableArea.cpp")
