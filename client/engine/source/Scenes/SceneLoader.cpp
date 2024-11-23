#include "Scenes/SceneLoader.hpp"

#include "Components/Transform.hpp"
#include "SceneParser/ComponentTokens.hpp"
#include "SceneParser/ISceneParser.hpp"
#include "Scenes/SceneEvents.hpp"

#include <Log/Log.hpp>

#include <functional>
#include <optional>
#include <variant>

namespace Scenes {

SceneLoader::SceneLoader(SceneParser::ISceneParser& sceneParser, IWorldApi& worldApi)
    : sceneParser(sceneParser), worldApi(worldApi), componentLoaders() {}

std::shared_ptr<SceneObject> SceneLoader::LoadScene(std::string_view scenePath, std::shared_ptr<SceneObject> parent) {
  auto scene = LoadSceneInternal(scenePath, parent);
  if (parent) {
    std::shared_ptr<SceneObject> clone = scene;
    parent->AddChild(std::move(clone));
  }
  scene->NotifyComponents(Scenes::SceneLoadedEvent{scene->GetId()}, true);
  return scene;
}

std::shared_ptr<SceneObject> SceneLoader::LoadSceneInternal(std::string_view scenePath, std::shared_ptr<SceneObject> parent) {
  auto parsedRootToken = sceneParser.ParseSceneData(scenePath);
  if (!parsedRootToken.has_value()) {
    Log::LogError("Failed to load scene '{}': Failed parsing file", scenePath);
    return nullptr;
  }
  auto rootToken = parsedRootToken.value();

  auto parentId = parent ? std::optional<size_t>{parent->GetId()} : std::optional<size_t>{};

  auto root = worldApi.CreateSceneObject(rootToken.name, rootToken.tags, parentId);

  for (SceneParser::SceneComponentToken& componentToken : rootToken.components) {
    if (!ConstructComponent(componentToken, *root)) {
      Log::LogError("Failed to load scene '{}': Failed loading component", scenePath);
      root->RequestDestruction();
      return nullptr;
    }
  }
  // Transform component is required by all objects
  if (!root->HasComponent<Components::Transform>()) {
    Log::LogError("SceneObject '{}' in '{}' is missing the required Transform component", root->GetName(), scenePath);
    return nullptr;
  }

  for (auto child : rootToken.children) {
    if (SceneParser::SceneObjectToken* childObject = std::get_if<SceneParser::SceneObjectToken>(&child)) {
      if (!ConstructObject(*childObject, *root)) {
        Log::LogError("Failed to load scene '{}': Failed loading child '{}'", scenePath, childObject->name);
        root->RequestDestruction();
        return nullptr;
      }
    } else if (SceneParser::SceneObjectIncludeToken* includeToken =
                   std::get_if<SceneParser::SceneObjectIncludeToken>(&child)) {
      auto loadedChild = LoadSceneInternal(includeToken->path, root);
      if (!loadedChild) {
        Log::LogError("Failed to load scene '{}': Failed including child '{}'", scenePath, includeToken->path);
        root->RequestDestruction();
        return nullptr;
      }
      if (includeToken->nameOverride.has_value()) {
        loadedChild->SetName(includeToken->nameOverride.value());
      }
      for (auto& tag : includeToken->extraTags) {
        loadedChild->AddTag(tag);
      }
      loadedChild->SetEnabled(includeToken->enableOverride);
    } else {
      Log::LogError("Failed to load scene '{}': Unsupported child", scenePath);
      root->RequestDestruction();
      return nullptr;
    }
  }

  Log::LogDebug("Loaded '{}' successfully", scenePath);

  root->SetEnabled(rootToken.enabled);

  return root;
}

bool SceneLoader::ConstructObject(SceneParser::SceneObjectToken& token, SceneObject& parentObject) {
  auto sceneObject = worldApi.CreateSceneObject(token.name, token.tags, parentObject.GetId());
  for (SceneParser::SceneComponentToken& componentToken : token.components) {
    if (!ConstructComponent(componentToken, *sceneObject)) {
      return false;
    }
  }
  if (!sceneObject->HasComponent<Components::Transform>()) {
    Log::LogError("SceneObject '{}', child of '{}' is missing the required Transform component",
                  sceneObject->GetName(),
                  parentObject.GetName());
    return false;
  }
  for (auto& child : token.children) {
    if (SceneParser::SceneObjectToken* childObject = std::get_if<SceneParser::SceneObjectToken>(&child)) {
      if (!ConstructObject(*childObject, *sceneObject)) {
        Log::LogError(
            "Failed to load scene object '{}': Failed including child '{}'", sceneObject->GetName(), childObject->name);
        return false;
      }
    } else if (SceneParser::SceneObjectIncludeToken* includeToken =
                   std::get_if<SceneParser::SceneObjectIncludeToken>(&child)) {
      auto loadedChild = LoadSceneInternal(includeToken->path, sceneObject);
      if (!loadedChild) {
        Log::LogError("Failed to load scene object '{}': Failed including child '{}'",
                      sceneObject->GetName(),
                      includeToken->path);
        return false;
      }
      if (includeToken->nameOverride.has_value()) {
        loadedChild->SetName(includeToken->nameOverride.value());
      }
      for (auto& tag : includeToken->extraTags) {
        loadedChild->AddTag(tag);
      }
      loadedChild->SetEnabled(includeToken->enableOverride);
      sceneObject->AddChild(std::move(loadedChild));
    } else {
      Log::LogError("Failed to load scene object '{}': Unsupported child", sceneObject->GetName());
      return false;
    }
  }
  parentObject.AddChild(std::move(sceneObject));

  return true;
}

bool SceneLoader::ConstructComponent(SceneParser::SceneComponentToken& token, SceneObject& object) {
  if (!componentLoaders.contains(token.id)) {
    Log::LogError("Failed to construct component '{}': Not registered", token.id);
    return false;
  }
  auto& loader = componentLoaders.at(token.id).get();
  auto component = loader.LoadComponent(worldApi, token);
  if (!component) {
    Log::LogError("Failed to load component '{}': Loader returned nullptr", token.id);
    return true;
  }
  object.AddComponent(loader.GetComponentType(), std::move(component));

  return true;
}

void SceneLoader::RegisterComponentLoader(ComponentLoaders::IComponentLoader& loader) {
  componentLoaders.emplace(loader.GetComponentType(), loader);
};

}  // namespace Scenes
