#pragma once

#include "Point2d.hpp"

namespace Math {

class IntRect {
 public:
  IntRect(int x, int y, int width, int height);
  IntRect(const IntRect& rect);

  IntPoint2d GetPosition() const {
    return {x, y};
  }

  int x;
  int y;
  int width;
  int height;
};

}  // namespace Math
