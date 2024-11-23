#include "Scenes/SceneComponent.hpp"
#include "Scenes/SceneObject.hpp"

namespace Scenes {

SceneComponent::SceneComponent(IWorldApi& worldApi) : worldApi(worldApi) {}
void SceneComponent::OnAttached(SceneObject& object) {
  ownerId = object.GetId();
}

void SceneComponent::OnDetached() {
  ownerId = std::nullopt;
}

std::optional<size_t> SceneComponent::GetOwnerId() {
  return ownerId;
}

std::weak_ptr<SceneObject> SceneComponent::GetOwner() {
  if (ownerId.has_value()) {
    return worldApi.GetSceneObject(*ownerId);
  } else {
    return {};
  }
}

IWorldApi& SceneComponent::GetWorldApi() {
  return worldApi;
}

}  // namespace Scenes
