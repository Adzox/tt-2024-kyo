#include "Base/BaseGame.hpp"

#include <functional>
#include <memory>

namespace Base {

BaseGame::BaseGame(GameConfig config,
                   Events::EventManager<size_t>& eventManager,
                   Graphics::IDrawApi& drawApi,
                   Assets::IImageLoader& imageAssetLoader,
                   SceneParser::ISceneParser& sceneParser,
                   Input::BaseInputManager& inputManager)
    : config(config),
      eventManager(eventManager),
      world(eventManager),
      drawApi(drawApi),
      imageAssetLoader(imageAssetLoader),
      sceneLoader(sceneParser, world),
      inputManager(inputManager),
      imageLoader(imageAssetLoader),
      transformLoader(),
      clickableAreaLoader() {
  globalEventToken = std::make_shared<size_t>(world.GetRoot()->GetId());
  eventManager.SubscribeListener<ExitGameEvent>(*this, globalEventToken);

  sceneLoader.RegisterComponentLoader(imageLoader);
  sceneLoader.RegisterComponentLoader(transformLoader);
  sceneLoader.RegisterComponentLoader(clickableAreaLoader);
}

BaseGame::~BaseGame() {}

void BaseGame::Init(std::string_view initialScene) {
  auto scene = sceneLoader.LoadScene(initialScene, world.GetRoot());
  if (!scene) {
    exitGame = true;
  }
  eventManager.PushQueue(StartGameEvent{});
}

void BaseGame::Update(float timeSinceLastLoop) {
  // Read and forward remote events

  inputManager.UpdateInput(timeSinceLastLoop);
  eventManager.FlushQueue();
  world.PreUpdate();
  world.Update(timeSinceLastLoop);

  // Send events going remotely
}

void BaseGame::Render() {
  world.Render(drawApi);
}

void BaseGame::Destroy() {
  imageAssetLoader.UnloadAll();
}

bool BaseGame::QuitRequested() {
  return exitGame;
}

Scenes::SceneLoader& BaseGame::GetSceneLoader() {
  return sceneLoader;
}

void BaseGame::RegisterCustomComponentLoader(ComponentLoaders::IComponentLoader& componentLoader) {
  sceneLoader.RegisterComponentLoader(componentLoader);
}

void BaseGame::OnEvent(const ExitGameEvent&) {
  exitGame = true;
}

}  // namespace Base
