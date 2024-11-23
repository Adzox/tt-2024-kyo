#pragma once

#include "SceneParser/SceneObjectTokens.hpp"
#include "Scenes/SceneComponent.hpp"
#include "Scenes/WorldApi.hpp"

#include <memory>
#include <string>
#include <string_view>
#include <vector>

namespace ComponentLoaders {

class IComponentLoader {
 public:
  virtual ~IComponentLoader() = default;
  virtual std::unique_ptr<Scenes::SceneComponent> LoadComponent(Scenes::IWorldApi& worldApi,
                                                                SceneParser::SceneComponentToken& token) = 0;

  virtual std::string_view GetComponentType() const = 0;
};

}  // namespace ComponentLoaders
