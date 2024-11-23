#include "ComponentLoaders/ClickableAreaLoader.hpp"

#include "Components/ClickableArea.hpp"
#include "Log/Log.hpp"
#include "SceneParser/ComponentTokens.hpp"
#include "Scenes/World.hpp"

#include <string>

namespace ComponentLoaders {

ClickableAreaLoader::ClickableAreaLoader() {}

std::string_view ClickableAreaLoader::GetComponentType() const {
  return Components::ClickableArea::GetType();
};

std::unique_ptr<Scenes::SceneComponent> ClickableAreaLoader::LoadComponent(Scenes::IWorldApi& worldApi,
                                                                   SceneParser::SceneComponentToken& token) {
  auto area = std::make_unique<Components::ClickableArea>(worldApi);

  if (auto areaArray = token.data.ParseEntry<SceneParser::SceneComponentArrayValue>("area")) {
    if (auto areaValues = areaArray->ParseAsArray<int, 2>()) {
      area->SetArea({areaValues->at(0), areaValues->at(1)});
    } else {
    Log::LogError("Required field 'area' failed parsing!");
    return nullptr;
  }
  } else {
    Log::LogError("Required field 'area' is missing for ClickableArea component!");
    return nullptr;
  }
  return area;
};

}  // namespace ComponentLoaders
