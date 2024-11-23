#pragma once

#include "Base/BasicEvents.hpp"
#include "Base/GameConfig.hpp"

#include "Assets/IImageLoader.hpp"
#include "ComponentLoaders/ClickableAreaLoader.hpp"
#include "ComponentLoaders/IComponentLoader.hpp"
#include "ComponentLoaders/ImageLoader.hpp"
#include "ComponentLoaders/TransformLoader.hpp"
#include "Events/EventManager.hpp"
#include "Input/BaseInputManager.hpp"
#include "Events/IEventListener.hpp"
#include "Graphics/IDrawApi.hpp"
#include "SceneParser/ISceneParser.hpp"
#include "Scenes/SceneLoader.hpp"
#include "Scenes/SceneObject.hpp"
#include "Scenes/World.hpp"

#include <memory>
#include <string_view>

namespace Base {

class BaseGame : public Events::IEventListener<ExitGameEvent> {
 public:
  BaseGame(GameConfig config,
          Events::EventManager<size_t>& eventManager,
           Graphics::IDrawApi& drawApi,
           Assets::IImageLoader& imageAssetLoader,
           SceneParser::ISceneParser& sceneParser,
           Input::BaseInputManager& inputManager);
  BaseGame(const BaseGame&) = delete;
  BaseGame(const BaseGame&&) = delete;
  ~BaseGame();

  virtual void Init(std::string_view initialScene);

  virtual void Update(float timeSinceLastLoop);
  virtual void Render();
  virtual void Destroy();
  virtual bool QuitRequested();
  Scenes::SceneLoader& GetSceneLoader();

  void RegisterCustomComponentLoader(ComponentLoaders::IComponentLoader& componentLoader);


  void OnEvent(const ExitGameEvent& event) override;

 protected:
  GameConfig config;
  Scenes::World world;

  Events::EventManager<size_t>& eventManager;
  Graphics::IDrawApi& drawApi;
  Assets::IImageLoader& imageAssetLoader;
  Scenes::SceneLoader sceneLoader;
  Input::BaseInputManager& inputManager;

  ComponentLoaders::ImageLoader imageLoader;
  ComponentLoaders::TransformLoader transformLoader;
  ComponentLoaders::ClickableAreaLoader clickableAreaLoader;

  std::shared_ptr<size_t> globalEventToken;

 private:
  bool exitGame = false;
};

}  // namespace Base
