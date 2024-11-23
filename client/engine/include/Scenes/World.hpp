#pragma once

#include "Scenes/SceneObject.hpp"
#include "Scenes/IRenderable.hpp"
#include "Scenes/IUpdateable.hpp"
#include "Scenes/WorldApi.hpp"

#include <Graphics/IDrawApi.hpp>

#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <random>
#include <string>
#include <string_view>
#include <vector>

namespace Scenes {

class World : public IWorldApi {
 public:
  World(Events::EventManager<size_t>& eventManager);
  ~World() = default;
  World(const World&) = delete;
  World(const World&&) = delete;

  std::weak_ptr<SceneObject> GetSceneObject(size_t id) override;
  SceneObject* FindSceneObjectFromRoot(std::string_view scenePath) override;
  SceneObject* FindSceneObjectFromObject(SceneObject& sceneObject,
                                                               std::string_view scenePath) override;
  std::shared_ptr<SceneObject> CreateSceneObject(std::string_view name,
                                                         std::set<std::string> tags = {},
                                                         std::optional<size_t> parent = {}) override;

  Events::EventManager<size_t>& GetEventManager() override;
  std::shared_ptr<SceneObject> GetRoot() override;

  void GenerateLists(SceneObject& object);
  void PreUpdate();
  void Update(float dt);
  void Render(Graphics::IDrawApi& drawApi);

 private:
  std::mt19937_64 idGenerator;

  Events::EventManager<size_t>& eventManager;

  std::shared_ptr<SceneObject> root;
  std::map<size_t, std::shared_ptr<SceneObject>> objects;

  std::vector<std::reference_wrapper<Scenes::IUpdateable>> updateables;
  std::vector<std::reference_wrapper<Scenes::IRenderable>> renderables;
};

}  // namespace Scenes
