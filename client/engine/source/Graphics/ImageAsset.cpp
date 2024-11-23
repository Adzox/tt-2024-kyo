#include "Graphics/ImageAsset.hpp"

namespace Graphics {

ImageAsset::ImageAsset(std::string_view resourcePath,
                       std::optional<Math::IntRect> source,
                       std::optional<NineSliceData> nineSliceData,
                       bool repeat)
    : resourcePath(resourcePath), source(source), nineSliceData(nineSliceData), repeat(repeat) {}

std::string_view ImageAsset::GetResourcePath() const {
  return resourcePath;
}

std::optional<Math::IntRect> ImageAsset::GetSourceRect() const {
  return source;
}

std::optional<NineSliceData> ImageAsset::GetNineSliceData() const {
  return nineSliceData;
}

bool ImageAsset::RepeatImage() const {
  return repeat;
}

}  // namespace Graphics
