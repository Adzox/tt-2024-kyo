#pragma once

#include <Assets/IImageLoader.hpp>
#include <Graphics/Color.hpp>
#include <Graphics/IDrawApi.hpp>

#include <SDL3/SDL_render.h>

#include <functional>
#include <map>
#include <string>

class SdlTextureRenderer : public Graphics::IDrawApi, public Assets::IImageLoader {
 public:
  SdlTextureRenderer(SDL_Renderer& renderer);
  SdlTextureRenderer(const SdlTextureRenderer&) = delete;
  SdlTextureRenderer(const SdlTextureRenderer&&) = delete;
  ~SdlTextureRenderer() noexcept;

  void RenderImageWithScale(const Graphics::ImageAsset& image,
                            int x,
                            int y,
                            float scaleX,
                            float scaleY,
                            float angle,
                            const Graphics::ColorRgba& multiplyColor) override;

  void RenderImageWithSize(const Graphics::ImageAsset& image,
                           int x,
                           int y,
                           int width,
                           int height,
                           float angle,
                           const Graphics::ColorRgba& multiplyColor) override;

  void RenderImage(const Graphics::ImageAsset& image,
                   int x,
                   int y,
                   std::optional<SDL_FRect>& source,
                   SDL_FRect& destination,
                   SDL_Texture* texture,
                   float angle,
                   const Graphics::ColorRgba& color);

  bool LoadImage(const Graphics::ImageAsset& image) override;
  void UnloadImage(const Graphics::ImageAsset& image) override;
  void UnloadAll() override;

 private:
  SDL_Renderer& renderer;
  std::map<std::string, SDL_Texture*, std::less<>> loadedTextures;
};
