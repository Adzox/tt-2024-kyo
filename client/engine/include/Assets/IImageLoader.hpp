#pragma once

#include "Graphics/ImageAsset.hpp"

namespace Assets {

class IImageLoader {
 public:
  virtual ~IImageLoader() noexcept = default;
  virtual bool LoadImage(const Graphics::ImageAsset& image) = 0;
  virtual void UnloadImage(const Graphics::ImageAsset& image) = 0;
  virtual void UnloadAll() = 0;
};

}  // namespace Assets
