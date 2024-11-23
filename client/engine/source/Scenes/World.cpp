#include "Scenes/World.hpp"

#include "Scenes/SceneObject.hpp"
#include "Scenes/ScenePathResolver.hpp"

#include <algorithm>
#include <chrono>

namespace Scenes {

World::World(Events::EventManager<size_t>& eventManager)
    : idGenerator(std::chrono::system_clock::now().time_since_epoch().count()),
      eventManager(eventManager),
      root(),
      objects(),
      updateables(),
      renderables() {
  root = CreateSceneObject("root");
}

std::weak_ptr<SceneObject> World::GetSceneObject(size_t id) {
  if (objects.contains(id)) {
    return std::weak_ptr<SceneObject>(objects[id]);
  } else {
    return {};
  }
};

SceneObject* World::FindSceneObjectFromRoot(std::string_view scenePath) {
  return ParseScenePathFirst(*root, scenePath);
}

SceneObject* World::FindSceneObjectFromObject(SceneObject& sceneObject, std::string_view scenePath) {
  return ParseScenePathFirst(sceneObject, scenePath);
}

std::shared_ptr<SceneObject> World::CreateSceneObject(std::string_view name,
                                                      std::set<std::string> tags,
                                                      std::optional<size_t> parent) {
  auto object = std::make_shared<SceneObject>(static_cast<IWorldApi&>(*this), idGenerator(), name, tags, parent);
  objects[object->GetId()] = object;
  return object;
};

Events::EventManager<size_t>& World::GetEventManager() {
  return eventManager;
}

std::shared_ptr<SceneObject> World::GetRoot() {
  return root;
};

void World::PreUpdate() {
  std::erase_if(objects, [this](const auto& item) {
    const auto& [_, object] = item;
    return !object || object->DestructionRequested();
  });

  // Todo: Replace with tree change detection
  updateables.clear();
  renderables.clear();
  GenerateLists(*root);
}

void World::Update(float dt) {
  for (auto updateable : updateables) {
    updateable.get().Update(dt);
  }
}

void World::Render(Graphics::IDrawApi& drawApi) {
  for (auto& renderable : renderables) {
    renderable.get().Render(drawApi);
  }
}

void World::GenerateLists(SceneObject& object) {
  updateables.append_range(object.GetUpdateables());
  renderables.append_range(object.GetRenderables());

  for (auto& child : object.GetChildren()) {
    if (auto x = child.lock()) {
      GenerateLists(*x);
    }
  }
}

}  // namespace Scenes
