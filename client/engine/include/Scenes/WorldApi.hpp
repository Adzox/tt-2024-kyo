#pragma once

#include <Events/Event.hpp>
#include <Events/EventManager.hpp>

#include <memory>
#include <optional>
#include <set>
#include <string>
#include <string_view>

namespace Scenes {

class SceneObject;

class IWorldApi {
 public:
  virtual ~IWorldApi() = default;

  virtual std::weak_ptr<SceneObject> GetSceneObject(size_t id) = 0;
  virtual SceneObject* FindSceneObjectFromRoot(std::string_view scenePath) = 0;
  virtual SceneObject* FindSceneObjectFromObject(SceneObject& sceneObject, std::string_view scenePath) = 0;

  virtual std::shared_ptr<SceneObject> CreateSceneObject(std::string_view name,
                                                         std::set<std::string> tags = {},
                                                         std::optional<size_t> parent = {}) = 0;
  virtual Events::EventManager<size_t>& GetEventManager() = 0;
  virtual std::shared_ptr<SceneObject> GetRoot() = 0;
};

}  // namespace Scenes
