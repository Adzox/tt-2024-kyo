#pragma once

#include "Assets/IImageLoader.hpp"
#include "Graphics/Color.hpp"
#include "Graphics/ImageAsset.hpp"
#include "Math/Point2d.hpp"
#include "Scenes/IRenderable.hpp"
#include "Scenes/SceneComponent.hpp"

namespace Components {

class Image : public Scenes::SceneComponent, public Scenes::IRenderable {
 public:
  Image(Scenes::IWorldApi& world, Assets::IImageLoader& imageLoader);
  virtual ~Image();
  void SetImage(const Graphics::ImageAsset& image);
  void SetSize(Math::IntPoint2d size);
  std::optional<Math::IntPoint2d> GetSize();

  void SetTintColor(Graphics::ColorRgba color);
  Graphics::ColorRgba GetTintColor() const;

  void Render(Graphics::IDrawApi& api) override;

  static std::string_view GetType() {
    return "Image";
  }

 private:
  Assets::IImageLoader& imageLoader;
  std::unique_ptr<Graphics::ImageAsset> image;
  Graphics::ColorRgba tintColor = {255, 255, 255, 255};
  std::optional<Math::IntPoint2d> size;
};

}  // namespace Components
