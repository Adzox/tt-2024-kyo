#pragma once

#include "Assets/IImageLoader.hpp"
#include "ComponentLoaders/IComponentLoader.hpp"
#include "SceneParser/SceneObjectTokens.hpp"

namespace ComponentLoaders {

class ImageLoader : public IComponentLoader {
 public:
  ImageLoader(Assets::IImageLoader& imageLoader);

  std::string_view GetComponentType() const override;

  std::unique_ptr<Scenes::SceneComponent> LoadComponent(Scenes::IWorldApi& worldApi,
                                                        SceneParser::SceneComponentToken& token) override;

 private:
  Assets::IImageLoader& imageLoader;
};

}  // namespace ComponentLoaders
