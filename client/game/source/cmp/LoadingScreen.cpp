#include "cmp/LoadingScreen.hpp"

#include <Base/BasicEvents.hpp>
#include <Graphics/Color.hpp>
#include <Log/Log.hpp>
#include <Math/Utils.hpp>

namespace {

Graphics::ColorRgba transparent{0, 0, 0, 0};
Graphics::ColorRgba black{0, 0, 0, 255};

void AnimateFadeIn(float elapsed, float totalTime, std::weak_ptr<Components::Image> imagePtr) {
  if (auto image = imagePtr.lock()) {
    image->SetTintColor(Graphics::ColorRgba::Lerp(transparent, black, Math::Clamp(elapsed / totalTime, 0, 1)));
  }
}

}  // namespace

LoadingScreen::LoadingScreen(Scenes::IWorldApi& world,
                             Scenes::SceneLoader& sceneLoader,
                             float minDuration,
                             float fadeTime)
    : SceneComponent(world),
      sceneLoader(sceneLoader),
      minDuration(minDuration),
      fadeTime(fadeTime),
      state(State::Inactive) {}

void LoadingScreen::OnAttached(Scenes::SceneObject& object) {
  SceneComponent::OnAttached(object);
  idToken = std::make_shared<size_t>(object.GetId());
  GetWorldApi().GetEventManager().SubscribeListener<Scenes::SceneLoadedEvent>(*this, idToken);
  GetWorldApi().GetEventManager().SubscribeListener<LoadSceneEvent>(*this, idToken);
}

void LoadingScreen::OnDetached() {
  idToken.reset();
}

void LoadingScreen::OnEvent(const Scenes::SceneLoadedEvent& /*event*/) {
  if (auto owner = GetOwner().lock()) {
    auto imageHolder = GetWorldApi().FindSceneObjectFromObject(*owner, "*/@Image");
    if (imageHolder) {
      image = imageHolder->GetComponent<Components::Image>();
      imageOwnerId = imageHolder->GetId();
    }
  }
};

void LoadingScreen::Update(float dt) {
  if (state == State::StartFadeIn) {
    UpdateStartFadeInState();
    state = State::FadeIn;
    GetWorldApi().GetEventManager().PushQueue(LoadingStartAppearEvent{});
  } else if (state == State::FadeIn) {
    if (UpdateFadeInState(dt)) {
      state = State::Loading;
      GetWorldApi().GetEventManager().PushQueue(LoadingAppearedEvent{});
      // Todo: Async loading off main thread if possible
      LoadScene();
    }
  } else if (state == State::Loading) {
    if (UpdateLoadingState(dt)) {
      state = State::FadeOut;
      GetWorldApi().GetEventManager().PushQueue(LoadingStartDisappearEvent{});
    }
  } else if (state == State::FadeOut) {
    if (UpdateFadeOutState(dt)) {
      state = State::EndedFadeOut;
    }
  } else if (state == State::EndedFadeOut) {
    UpdateEndedFadeOutState();
    state = State::Inactive;
    GetWorldApi().GetEventManager().PushQueue(LoadingDissapeared{});
  }
};

void LoadingScreen::UpdateStartFadeInState() {
  if (!imageOwnerId.has_value()) {
    Log::LogError("LoadingScreen: Image Owner has been forgotten!");
    GetWorldApi().GetEventManager().NotifyNow(Base::ExitGameEvent{});
    return;
  }
  auto imageOwner = GetWorldApi().GetSceneObject(*imageOwnerId).lock();
  if (!imageOwner) {
    Log::LogError("LoadingScreen: Image Owner has been destroyed!");
    GetWorldApi().GetEventManager().NotifyNow(Base::ExitGameEvent{});
    return;
  }
  imageOwner->SetEnabled(true);
  animationTimer = 0;
  AnimateFadeIn(0, fadeTime, image);
}

bool LoadingScreen::UpdateFadeInState(float dt) {
  if (animationTimer < fadeTime) {
    animationTimer += dt;
    AnimateFadeIn(animationTimer, fadeTime, image);
    return false;
  } else {
    animationTimer = 0;
    AnimateFadeIn(fadeTime, fadeTime, image);
    return true;
  }
}

void LoadingScreen::LoadScene() {
  if (!loadSceneRequest.has_value()) {
    Log::LogError("LoadingScreen: No scene to load!");
    GetWorldApi().GetEventManager().NotifyNow(Base::ExitGameEvent{});
    return;
  }
  if (!GetOwnerId().has_value()) {
    Log::LogWarn("LoadingScreen: Is detached, cannot load scene!");
    return;
  }
  auto loadParent = GetWorldApi().GetSceneObject(loadSceneRequest->parentId).lock();
  if (!loadParent) {
    Log::LogWarn("LoadingScreen: loadParent has been destroyed, cannot load scene!");
    return;
  }

  Log::LogDebug("LoadingScreen: Started loading '{}'!", loadSceneRequest->scenePath);
  if (auto child = loadParent->GetChild(0).lock()) {
    child->RequestDestruction();
  }
  // Todo: Replace with concurrent loading!
  sceneLoader.LoadScene(loadSceneRequest->scenePath, loadParent);
  loadSceneRequest = std::nullopt;
}

bool LoadingScreen::UpdateLoadingState(float dt) {
  if (animationTimer < minDuration) {
    animationTimer += dt;
    return false;
  } else {
    animationTimer = fadeTime;
    return true;
  }
}

bool LoadingScreen::UpdateFadeOutState(float dt) {
  if (animationTimer >= 0) {
    animationTimer -= dt;
    AnimateFadeIn(animationTimer, fadeTime, image);
    return false;
  } else {
    AnimateFadeIn(0, fadeTime, image);
    return true;
  }
}

void LoadingScreen::UpdateEndedFadeOutState() {
  if (!imageOwnerId.has_value()) {
    Log::LogError("LoadingScreen: Image Owner has been forgotten!");
    GetWorldApi().GetEventManager().NotifyNow(Base::ExitGameEvent{});
    return;
  }
  auto imageOwner = GetWorldApi().GetSceneObject(*imageOwnerId).lock();
  if (!imageOwner) {
    Log::LogError("LoadingScreen: Image Owner has been destroyed!");
    GetWorldApi().GetEventManager().NotifyNow(Base::ExitGameEvent{});
    return;
  }

  imageOwner->SetEnabled(false);
}

void LoadingScreen::OnEvent(const LoadSceneEvent& event) {
  if (state == State::Inactive && !image.expired()) {
    loadSceneRequest = event;
    state = State::StartFadeIn;
  }
};
