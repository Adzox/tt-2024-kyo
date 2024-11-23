#pragma once

namespace Scenes {

class IUpdateable {
 public:
  virtual void Update(float deltaTime) = 0;
};

}  // namespace Scenes
