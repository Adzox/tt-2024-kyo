#pragma once

#include "ComponentLoaders/IComponentLoader.hpp"
#include "SceneParser/ISceneParser.hpp"
#include "Scenes/SceneComponent.hpp"
#include "Scenes/SceneObject.hpp"
#include "Scenes/WorldApi.hpp"

#include <functional>
#include <map>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

namespace Scenes {

class SceneLoader {
 public:
  SceneLoader(SceneParser::ISceneParser& sceneParser, IWorldApi& worldApi);
  virtual ~SceneLoader() = default;

  std::shared_ptr<SceneObject> LoadScene(std::string_view path, std::shared_ptr<SceneObject> parent = nullptr);
  void RegisterComponentLoader(ComponentLoaders::IComponentLoader& loader);

 private:
  std::shared_ptr<SceneObject> LoadSceneInternal(std::string_view path, std::shared_ptr<SceneObject> parent = nullptr);
  bool ConstructObject(SceneParser::SceneObjectToken& token, SceneObject& parentObject);
  bool ConstructComponent(SceneParser::SceneComponentToken& token, SceneObject& object);

  SceneParser::ISceneParser& sceneParser;
  IWorldApi& worldApi;
  std::map<std::string, std::reference_wrapper<ComponentLoaders::IComponentLoader>> componentLoaders;
};

}  // namespace Scenes
