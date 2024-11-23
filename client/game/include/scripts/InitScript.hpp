#pragma once

#include <Base/BasicEvents.hpp>
#include <Events/IEventListener.hpp>
#include <Scenes/SceneComponent.hpp>
#include <Scenes/SceneLoader.hpp>
#include <Scenes/SceneObject.hpp>
#include <Scenes/WorldApi.hpp>

#include <memory>
#include <optional>
#include <string>
#include <string_view>

class InitScript : public Scenes::SceneComponent, public Events::IEventListener<Base::StartGameEvent> {
 public:
  static std::string_view GetType() {
    return "InitScript";
  }

  InitScript(Scenes::IWorldApi& world);
  void OnAttached(Scenes::SceneObject& object) override;
  void OnDetached() override;

  void OnEvent(const Base::StartGameEvent& event) override;

 private:
  std::shared_ptr<size_t> idToken;
};
