#pragma once

#include <Components/Image.hpp>
#include <Events/IEventListener.hpp>
#include <Scenes/SceneEvents.hpp>
#include <Scenes/IUpdateable.hpp>
#include <Scenes/SceneComponent.hpp>
#include <Scenes/SceneLoader.hpp>
#include <Scenes/WorldApi.hpp>

#include <memory>
#include <optional>
#include <string>
#include <string_view>

struct LoadSceneEvent {
  std::string scenePath;
  size_t parentId;
};

struct LoadingStartAppearEvent {};
struct LoadingAppearedEvent {};
struct LoadingStartDisappearEvent {};
struct LoadingDissapeared {};

class LoadingScreen : public Scenes::SceneComponent, public Scenes::IUpdateable, public Events::IEventListener<LoadSceneEvent>, public Events::IEventListener<Scenes::SceneLoadedEvent> {
 public:

  static std::string_view GetType() {
    return "LoadingScreen";
  }

  LoadingScreen(Scenes::IWorldApi& world, Scenes::SceneLoader& sceneLoader, float minDuration, float fadeTime);

  void OnAttached(Scenes::SceneObject& object) override;
  void OnDetached() override;

  void OnEvent(const Scenes::SceneLoadedEvent& event) override;

  void Update(float dt) override;
  void UpdateStartFadeInState();
  bool UpdateFadeInState(float dt);
  void LoadScene();
  bool UpdateLoadingState(float dt);
  bool UpdateFadeOutState(float dt);
  void UpdateEndedFadeOutState();

  void OnEvent(const LoadSceneEvent& event) override;

private:

  enum class State {
    Inactive,
    StartFadeIn,
    FadeIn,
    Loading,
    FadeOut,
    EndedFadeOut,
  };

  Scenes::SceneLoader& sceneLoader;
  float minDuration;
  float fadeTime;
  State state;

  float animationTimer = 0;

  std::optional<LoadSceneEvent> loadSceneRequest;

  std::weak_ptr<Components::Image> image;
  std::optional<size_t> imageOwnerId;

  std::shared_ptr<size_t> idToken;
};
