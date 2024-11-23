#pragma once

#include "ComponentLoaders/IComponentLoader.hpp"
#include "SceneParser/SceneObjectTokens.hpp"

namespace ComponentLoaders {

class TransformLoader : public IComponentLoader {
public:
  std::string_view GetComponentType() const override;

  std::unique_ptr<Scenes::SceneComponent> LoadComponent(Scenes::IWorldApi& worldApi,
                                                        SceneParser::SceneComponentToken& token) override;
};

}  // namespace ComponentLoaders
