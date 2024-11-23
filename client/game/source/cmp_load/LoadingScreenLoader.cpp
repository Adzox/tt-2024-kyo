#include "cmp_load/LoadingScreenLoader.hpp"

#include "cmp/LoadingScreen.hpp"

#include <Log/Log.hpp>
#include <SceneParser/ComponentTokens.hpp>
#include <Scenes/World.hpp>

#include <string>

namespace {

std::optional<Graphics::NineSliceData> ParseNineSlice(SceneParser::SceneComponentMapValue& map) {
  int width = 0;
  if (auto widthEntry = map.ParseEntry<int>("width")) {
    width = *widthEntry;
  } else {
    return {};
  }
  int height = 0;
  if (auto heightEntry = map.ParseEntry<int>("height")) {
    height = *heightEntry;
  } else {
    return {};
  }

  Math::IntPoint2d topLeft{};
  if (auto topLeftArray = map.ParseEntry<SceneParser::SceneComponentArrayValue>("topLeft")) {
    if (auto topLeftPoint = topLeftArray->ParseAsArray<int, 2>()) {
      topLeft = {topLeftPoint->at(0), topLeftPoint->at(1)};
    } else {
      return {};
    }

  } else {
    return {};
  }
  Math::IntPoint2d bottomRight{};
  if (auto bottomRightArray = map.ParseEntry<SceneParser::SceneComponentArrayValue>("bottomRight")) {
    if (auto bottomRightPoint = bottomRightArray->ParseAsArray<int, 2>()) {
      bottomRight = {bottomRightPoint->at(0), bottomRightPoint->at(1)};
    } else {
      return {};
    }
  } else {
    return {};
  }

  return Graphics::NineSliceData{width, height, topLeft, bottomRight};
}

}  // namespace

LoadingScreenLoader::LoadingScreenLoader(Scenes::SceneLoader& sceneLoader) : sceneLoader(sceneLoader) {}

std::string_view LoadingScreenLoader::GetComponentType() const {
  return LoadingScreen::GetType();
};

std::unique_ptr<Scenes::SceneComponent> LoadingScreenLoader::LoadComponent(Scenes::IWorldApi& worldApi,
                                                                           SceneParser::SceneComponentToken& token) {
  float minDuration = 0;
  if (auto floatMinDuration = token.data.ParseEntry<float>("minDuration")) {
    minDuration = *floatMinDuration;
  } else if (auto intMinDuration = token.data.ParseEntry<int>("minDuration")) {
    minDuration = static_cast<float>(*intMinDuration);
  } else {
    Log::LogError("Required number field 'minDuration' is missing for LoadingScreen component!");
    return nullptr;
  }

  float fadeTime = 0;
  if (auto floatFadeTime = token.data.ParseEntry<float>("fadeTime")) {
    fadeTime = *floatFadeTime;
  } else if (auto intFadeTime = token.data.ParseEntry<int>("fadeTime")) {
    fadeTime = static_cast<float>(*intFadeTime);
  } else {
    Log::LogError("Required number field 'fadeTime' is missing for LoadingScreen component!");
    return nullptr;
  }

  auto loadingScreen = std::make_unique<LoadingScreen>(worldApi, sceneLoader, minDuration, fadeTime);

  return loadingScreen;
};
