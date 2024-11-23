#pragma once

namespace Input {

enum class MouseButton { None, Left, Right };

struct MouseMoved {
  float x;
  float y;
};

struct MousePress {
  float x;
  float y;
  MouseButton button;
};

struct MouseRelease {
  float x;
  float y;
  MouseButton button;
};

}  // namespace Input
