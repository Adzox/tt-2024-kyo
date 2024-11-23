#pragma once

#include <ComponentLoaders/IComponentLoader.hpp>
#include <Scenes/SceneLoader.hpp>
#include <SceneParser/SceneObjectTokens.hpp>

class LoadingScreenLoader : public ComponentLoaders::IComponentLoader {
 public:
  LoadingScreenLoader(Scenes::SceneLoader& sceneLoader);

  std::string_view GetComponentType() const override;

  std::unique_ptr<Scenes::SceneComponent> LoadComponent(Scenes::IWorldApi& worldApi,
                                                        SceneParser::SceneComponentToken& token) override;

 private:
  Scenes::SceneLoader& sceneLoader;
};
