#include "ComponentLoaders/TransformLoader.hpp"

#include "Components/Transform.hpp"
#include "Math/Point2d.hpp"
#include "SceneParser/ComponentTokens.hpp"

namespace ComponentLoaders {

std::string_view TransformLoader::GetComponentType() const {
  return Components::Transform::GetType();
};

std::unique_ptr<Scenes::SceneComponent> TransformLoader::LoadComponent(Scenes::IWorldApi& worldApi,
                                                                       SceneParser::SceneComponentToken& token) {
  auto transform = std::make_unique<Components::Transform>(worldApi);

  if (auto positionArray = token.data.ParseEntry<SceneParser::SceneComponentArrayValue>("position")) {
    if (auto position = positionArray->ParseAsArray<float, 2>()) {
      transform->SetLocalPosition({position->at(0), position->at(1)});
    }
    if (auto position = positionArray->ParseAsArray<int, 2>()) {
      transform->SetLocalPosition({static_cast<float>(position->at(0)), static_cast<float>(position->at(1))});
    }
  }

  if (auto scaleArray = token.data.ParseEntry<SceneParser::SceneComponentArrayValue>("scale")) {
    if (auto scale = scaleArray->ParseAsArray<float, 2>()) {
      transform->SetLocalScale({scale->at(0), scale->at(1)});
    }
    if (auto scale = scaleArray->ParseAsArray<int, 2>()) {
      transform->SetLocalScale({static_cast<float>(scale->at(0)), static_cast<float>(scale->at(1))});
    }
  }

  if (auto rotation = token.data.ParseEntry<float>("rotation")) {
    transform->SetLocalRotation(*rotation);
  } else if (auto rotation = token.data.ParseEntry<int>("rotation")) {
    transform->SetLocalRotation(static_cast<float>(*rotation));
  }

  return transform;
};

}  // namespace ComponentLoaders
