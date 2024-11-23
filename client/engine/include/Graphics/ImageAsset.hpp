#pragma once

#include "Graphics/Color.hpp"
#include "Graphics/NineSliceData.hpp"

#include <Math/IntRect.hpp>

#include <optional>
#include <string>
#include <string_view>

namespace Graphics {

class ImageAsset {
 public:
  ImageAsset(std::string_view resourcePath,
             std::optional<Math::IntRect> source,
             std::optional<NineSliceData> nineSliceData,
             bool repeat);
  virtual ~ImageAsset() = default;

  std::string_view GetResourcePath() const;
  std::optional<Math::IntRect> GetSourceRect() const;
  std::optional<NineSliceData> GetNineSliceData() const;
  bool RepeatImage() const;

 private:
  std::string resourcePath;
  std::optional<Math::IntRect> source;
  std::optional<NineSliceData> nineSliceData;

  bool repeat = false;
};

}  // namespace Graphics
