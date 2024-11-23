target_sources(${LIBRARY_NAME} PUBLIC FILE_SET HEADERS BASE_DIRS "../../include/Scenes" FILES
    "../../include/Scenes/IRenderable.hpp"
    "../../include/Scenes/IUpdateable.hpp"
    "../../include/Scenes/SceneComponent.hpp"
    "../../include/Scenes/SceneObject.hpp"
    "../../include/Scenes/ScenePathResolver.hpp"
    "../../include/Scenes/World.hpp"
    "../../include/Scenes/WorldApi.hpp"
    "../../include/Scenes/SceneEvents.hpp"
    "../../include/Scenes/SceneLoader.hpp")

target_sources(${LIBRARY_NAME} PRIVATE
    "SceneComponent.cpp"
    "SceneObject.cpp"
    "ScenePathResolver.cpp"
    "SceneLoader.cpp"
    "World.cpp")
