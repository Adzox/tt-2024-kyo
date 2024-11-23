#include "scripts/OnClickedSwitchSceneScript.hpp"

#include "cmp/LoadingScreen.hpp"

OnClickedSwitchSceneScript::OnClickedSwitchSceneScript(Scenes::IWorldApi& world) : SceneComponent(world) {}

void OnClickedSwitchSceneScript::OnAttached(Scenes::SceneObject& object) {
  SceneComponent::OnAttached(object);
  idToken = std::make_shared<size_t>(object.GetId());
  GetWorldApi().GetEventManager().SubscribeListener<Components::AreaReleased>(*this, idToken);
}

void OnClickedSwitchSceneScript::OnDetached() {
  idToken.reset();
}

void OnClickedSwitchSceneScript::OnEvent(const Components::AreaReleased& event) {
  if (auto* a = GetWorldApi().FindSceneObjectFromRoot("*/Base/Game")) {
    GetWorldApi().GetEventManager().NotifyNow(LoadSceneEvent{"res/common/scenes/game.json", a->GetId()});
  }
}
