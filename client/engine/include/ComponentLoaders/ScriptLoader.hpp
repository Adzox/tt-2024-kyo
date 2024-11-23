#pragma once

#include "ComponentLoaders/IComponentLoader.hpp"
#include "SceneParser/SceneObjectTokens.hpp"
#include "Scenes/SceneComponent.hpp"

#include <concepts>
#include <string>
#include <string_view>

namespace ComponentLoaders {

template <typename T>
concept Script = requires(T /*t*/, Scenes::IWorldApi& worldApi) {
  { T::GetType() } -> std::convertible_to<std::string_view>;
  { T(worldApi) };
  std::derived_from<T, Scenes::SceneComponent>;
};

template <Script T>
class ScriptLoader : public IComponentLoader {
 public:
  std::string_view GetComponentType() const override {
    return T::GetType();
  };

  std::unique_ptr<Scenes::SceneComponent> ScriptLoader::LoadComponent(Scenes::IWorldApi& worldApi,
                                                                      SceneParser::SceneComponentToken& token) {
    return std::make_unique<T>(worldApi);
  };
};
}  // namespace ComponentLoaders
