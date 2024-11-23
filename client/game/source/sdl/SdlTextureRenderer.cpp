#include "sdl/SdlTextureRenderer.hpp"

#include <Log/Log.hpp>
#include <Math/IntRect.hpp>
#include <Math/Utils.hpp>

#include <SDL3/SDL_filesystem.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_surface.h>
#include <SDL3_image/SDL_image.h>

#include <optional>
#include <string>

SdlTextureRenderer::SdlTextureRenderer(SDL_Renderer& renderer) : renderer(renderer) {
  IMG_Init(IMG_INIT_PNG);
  SDL_SetRenderDrawColor(&renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
}

SdlTextureRenderer::~SdlTextureRenderer() {
  IMG_Quit();
}

void SdlTextureRenderer::RenderImageWithScale(const Graphics::ImageAsset& image,
                                              int x,
                                              int y,
                                              float scaleX,
                                              float scaleY,
                                              float angle,
                                              const Graphics::ColorRgba& multiplyColor) {
  if (!loadedTextures.contains(image.GetResourcePath())) {
    Log::LogError("Failed to render image (not loaded): {}", image.GetResourcePath());
    return;
  }

  auto path = std::string(image.GetResourcePath());
  SDL_Texture* texture = loadedTextures[path];

  int width = texture->w;
  int height = texture->h;
  std::optional<SDL_FRect> source = std::nullopt;
  if (image.GetSourceRect().has_value()) {
    auto rect = image.GetSourceRect().value();
    source = SDL_FRect{static_cast<float>(rect.x),
                       static_cast<float>(rect.y),
                       static_cast<float>(rect.width),
                       static_cast<float>(rect.height)};
    width = rect.width;
    height = rect.height;
  }
  SDL_FRect destination = {
      static_cast<float>(x),
      static_cast<float>(y),
      static_cast<float>(width * scaleX),
      static_cast<float>(height * scaleY),
  };

  RenderImage(image, x, y, source, destination, texture, angle, multiplyColor);
};

void SdlTextureRenderer::RenderImageWithSize(const Graphics::ImageAsset& image,
                                             int x,
                                             int y,
                                             int width,
                                             int height,
                                             float angle,
                                             const Graphics::ColorRgba& multiplyColor) {
  if (!loadedTextures.contains(image.GetResourcePath())) {
    Log::LogError("Failed to render image (not loaded): {}", image.GetResourcePath());
    return;
  }
  auto path = std::string(image.GetResourcePath());
  SDL_Texture* texture = loadedTextures[path];

  std::optional<SDL_FRect> source = std::nullopt;
  if (image.GetSourceRect().has_value()) {
    auto rect = image.GetSourceRect().value();
    source = SDL_FRect{static_cast<float>(rect.x),
                       static_cast<float>(rect.y),
                       static_cast<float>(rect.width),
                       static_cast<float>(rect.height)};
  }
  SDL_FRect destination = {
      static_cast<float>(x),
      static_cast<float>(y),
      static_cast<float>(width),
      static_cast<float>(height),
  };

  RenderImage(image, x, y, source, destination, texture, angle, multiplyColor);
};

void SdlTextureRenderer::RenderImage(const Graphics::ImageAsset& image,
                                     int x,
                                     int y,
                                     std::optional<SDL_FRect>& source,
                                     SDL_FRect& destination,
                                     SDL_Texture* texture,
                                     float angle,
                                     const Graphics::ColorRgba& color) {
  if (color.GetAlpha() == 0) {
    return;
  }

  SDL_SetTextureColorMod(texture, color.GetR(), color.GetG(), color.GetB());
  SDL_SetTextureAlphaMod(texture, color.GetAlpha());

  // Todo: Implement 9 slice and repeating manually in order to support rotating image
  if (image.GetNineSliceData().has_value()) {
    auto data = image.GetNineSliceData().value();
    SDL_RenderTexture9Grid(&renderer,
                           texture,
                           source.has_value() ? &source.value() : nullptr,
                           data.topLeft.width,
                           data.topRight.width,
                           data.topCenter.height,
                           data.bottomCenter.height,
                           1.f,
                           &destination);
  } else if (image.RepeatImage()) {
    SDL_RenderTextureTiled(&renderer, texture, source.has_value() ? &source.value() : nullptr, 1.f, &destination);
  } else if (!Math::ApproximatelyEqual(0, angle, 0.001)) {
    SDL_RenderTextureRotated(
        &renderer, texture, source.has_value() ? &source.value() : nullptr, &destination, angle, nullptr, SDL_FLIP_NONE);
  } else {
    SDL_RenderTexture(&renderer, texture, source.has_value() ? &source.value() : nullptr, &destination);
  }

  SDL_SetTextureColorMod(texture, 255, 255, 255);
  SDL_SetTextureAlphaMod(texture, 255);
}

bool SdlTextureRenderer::LoadImage(const Graphics::ImageAsset& image) {
  if (loadedTextures.contains(image.GetResourcePath())) {
    Log::LogDebug("Image texture already loaded: {}", image.GetResourcePath());
    return true;
  }
  auto shortPath = std::string(image.GetResourcePath());
  auto fullPath = std::string(SDL_GetBasePath()) + std::string(image.GetResourcePath());
  SDL_Texture* texture = IMG_LoadTexture(&renderer, fullPath.c_str());
  if (!texture) {
    Log::LogError("Failed to load image: {}", shortPath);
    return false;
  }
  SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);
  loadedTextures[shortPath] = texture;
  return true;
}

void SdlTextureRenderer::UnloadImage(const Graphics::ImageAsset& image) {
  if (!loadedTextures.contains(image.GetResourcePath())) {
    Log::LogDebug("Texture has already been unloaded for: {}", image.GetResourcePath());
    return;
  }
  auto shortPath = std::string(image.GetResourcePath());
  SDL_Texture* texture = loadedTextures[shortPath];
  SDL_DestroyTexture(texture);
  loadedTextures.erase(shortPath);
}

void SdlTextureRenderer::UnloadAll() {
  for (auto& [_, texture] : loadedTextures) {
    SDL_DestroyTexture(texture);
  }
  loadedTextures.clear();
}
