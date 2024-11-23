#pragma once

#include <Components/ClickableArea.hpp>
#include <Events/IEventListener.hpp>
#include <Scenes/SceneComponent.hpp>
#include <Scenes/SceneObject.hpp>
#include <Scenes/WorldApi.hpp>

#include <memory>
#include <string_view>

class OnClickedSwitchSceneScript : public Scenes::SceneComponent, public Events::IEventListener<Components::AreaReleased> {
 public:
  static std::string_view GetType() {
    return "OnClickedSwitchSceneScript";
  }

  OnClickedSwitchSceneScript(Scenes::IWorldApi& world);

  void OnAttached(Scenes::SceneObject& object) override;
  void OnDetached() override;

  void OnEvent(const Components::AreaReleased& onReleased) override;

 private:
  std::shared_ptr<size_t> idToken;
};
