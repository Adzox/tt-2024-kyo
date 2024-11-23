#include "scripts/InitScript.hpp"

#include "cmp/LoadingScreen.hpp"

InitScript::InitScript(Scenes::IWorldApi& world) : SceneComponent(world) {}

void InitScript::OnAttached(Scenes::SceneObject& owner) {
  SceneComponent::OnAttached(owner);
  idToken = std::make_shared<size_t>(owner.GetId());
  GetWorldApi().GetEventManager().SubscribeListener(*this, idToken);
}

void InitScript::OnDetached() {
  idToken.reset();
}

void InitScript::OnEvent(const Base::StartGameEvent& /*event*/) {
  if (auto* gameContainer = GetWorldApi().FindSceneObjectFromRoot("*/*/Game")) {
    GetWorldApi().GetEventManager().PushQueue(LoadSceneEvent{"res/common/scenes/title.json", gameContainer->GetId()});
  }
}
