#pragma once

#include "Graphics/Color.hpp"
#include "Graphics/ImageAsset.hpp"

namespace Graphics {

class IDrawApi {
 public:
  virtual ~IDrawApi() noexcept = default;

  virtual void RenderImageWithScale(const ImageAsset& image,
                                    int x,
                                    int y,
                                    float scaleX,
                                    float scaleY,
                                    float angle,
                                    const Graphics::ColorRgba& multiplyColor) = 0;

  virtual void RenderImageWithSize(const ImageAsset& image,
                                   int x,
                                   int y,
                                   int width,
                                   int height,
                                   float angle,
                                   const Graphics::ColorRgba& multiplyColor) = 0;
};

}  // namespace Graphics
