#include "ComponentLoaders/ImageLoader.hpp"

#include "Components/Image.hpp"
#include "Log/Log.hpp"
#include "SceneParser/ComponentTokens.hpp"
#include "Scenes/World.hpp"

#include <string>

namespace ComponentLoaders {

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

ImageLoader::ImageLoader(Assets::IImageLoader& imageLoader) : imageLoader(imageLoader) {}

std::string_view ImageLoader::GetComponentType() const {
  return Components::Image::GetType();
};

std::unique_ptr<Scenes::SceneComponent> ImageLoader::LoadComponent(Scenes::IWorldApi& worldApi,
                                                                   SceneParser::SceneComponentToken& token) {
  auto image = std::make_unique<Components::Image>(worldApi, imageLoader);

  std::string path;
  if (auto pathString = token.data.ParseEntry<std::string>("path")) {
    path = *pathString;
  } else {
    Log::LogError("Required field 'path' is missing for Image component!");
    return nullptr;
  }

  if (auto sizeArray = token.data.ParseEntry<SceneParser::SceneComponentArrayValue>("size")) {
    if (auto size = sizeArray->ParseAsArray<int, 2>()) {
      image->SetSize({size->at(0), size->at(1)});
    }
  }

  if (auto colorArray = token.data.ParseEntry<SceneParser::SceneComponentArrayValue>("tintColor")) {
    if (auto color = colorArray->ParseAsArray<int, 4>()) {
      image->SetTintColor({static_cast<uint8_t>(color->at(0)),
                          static_cast<uint8_t>(color->at(1)),
                          static_cast<uint8_t>(color->at(2)),
                          static_cast<uint8_t>(color->at(3))});
    }
  }

  std::optional<Math::IntRect> source;
  if (auto sourceArray = token.data.ParseEntry<SceneParser::SceneComponentArrayValue>("source")) {
    if (auto sourceRect = sourceArray->ParseAsArray<int, 4>()) {
      source = {sourceRect->at(0), sourceRect->at(1), sourceRect->at(2), sourceRect->at(3)};
    }
  }

  std::optional<Graphics::NineSliceData> nineSlice;
  if (auto nineSliceMap = token.data.ParseEntry<SceneParser::SceneComponentMapValue>("nineSlice")) {
    nineSlice = ParseNineSlice(*nineSliceMap);
  }

  bool repeat = false;
  if (auto repeatFlag = token.data.ParseEntry<bool>("repeat")) {
    repeat = *repeatFlag;
  }

  Graphics::ImageAsset asset{path, source, nineSlice, repeat};

  image->SetImage(asset);
  return image;
};

}  // namespace ComponentLoaders
