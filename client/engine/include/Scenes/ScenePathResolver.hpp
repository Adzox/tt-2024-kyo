#pragma once

#include "Scenes/SceneObject.hpp"

#include <functional>
#include <optional>
#include <string_view>
#include <vector>

namespace Scenes {

SceneObject* ParseScenePathFirst(SceneObject& sceneObject, std::string_view scenePath);

}  // namespace Scenes
