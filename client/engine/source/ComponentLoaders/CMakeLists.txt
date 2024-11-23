target_sources(${LIBRARY_NAME} PUBLIC FILE_SET HEADERS BASE_DIRS "../../include/ComponentLoaders" FILES
    "../../include/ComponentLoaders/IComponentLoader.hpp"
    "../../include/ComponentLoaders/ImageLoader.hpp"
    "../../include/ComponentLoaders/ScriptLoader.hpp"
    "../../include/ComponentLoaders/TransformLoader.hpp"
    "../../include/ComponentLoaders/ClickableAreaLoader.hpp")

target_sources(${LIBRARY_NAME} PRIVATE
    "ImageLoader.cpp"
    "TransformLoader.cpp"
    "ClickableAreaLoader.cpp")
