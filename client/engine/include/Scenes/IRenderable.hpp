#pragma once

#include <Graphics/IDrawApi.hpp>

namespace Scenes {

class IRenderable {
 public:
  virtual void Render(Graphics::IDrawApi& api) = 0;
};

}  // namespace Scenes
