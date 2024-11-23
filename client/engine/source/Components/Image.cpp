#include "Components/Image.hpp"
#include "Components/Transform.hpp"
#include "Log/Log.hpp"
#include "Scenes/SceneObject.hpp"
#include "Scenes/WorldApi.hpp"

namespace Components {

Image::Image(Scenes::IWorldApi& world, Assets::IImageLoader& imageLoader) : SceneComponent(world), imageLoader(imageLoader) {}

Image::~Image() {
  if (image) {
    imageLoader.UnloadImage(*image.get());  
  }
}

void Image::SetImage(const Graphics::ImageAsset& newImage) {
  if (image) {
    imageLoader.UnloadImage(*image.get());  
  }
  image = std::make_unique<Graphics::ImageAsset>(newImage);
  imageLoader.LoadImage(*image.get());
}

void Image::SetSize(Math::IntPoint2d newSize) {
  size = newSize;
}

std::optional<Math::IntPoint2d> Image::GetSize() {
  return size;
}

void Image::SetTintColor(Graphics::ColorRgba color) {
  tintColor = color;
};

Graphics::ColorRgba Image::GetTintColor() const {
  return tintColor;
};

void Image::Render(Graphics::IDrawApi& api) {
  if (auto owner = GetOwner().lock()) {
    if (!image) {
      Log::LogWarn("Missing image asset from Image component on {}", owner->GetName());
      return;
    }

    auto component = owner->GetComponent<Transform>();
    if (component.expired()) {
      Log::LogError("Missing required Transform component from Image component on {}", owner->GetName());
      return;
    }
    auto transform = component.lock();
    auto pos = transform->GetGlobalPosition();
    auto scale = transform->GetGlobalScale();
    auto rotation = transform->GetGlobalRotation();

    if (size.has_value()) {
      api.RenderImageWithSize(
              *image, pos.GetX(), pos.GetY(), size->GetX() * scale.GetX(), size->GetY() * scale.GetY(), rotation, tintColor);
    } else {
      api.RenderImageWithScale(
        *image, pos.GetX(), pos.GetY(), scale.GetX(), scale.GetY(), rotation, tintColor);
    }    
  }
}

}  // namespace Components
