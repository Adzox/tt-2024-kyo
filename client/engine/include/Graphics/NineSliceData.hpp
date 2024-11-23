#pragma once

#include <Math/IntRect.hpp>
#include <Math/Point2d.hpp>

namespace Graphics {

struct NineSliceData {
  NineSliceData(int width, int height, Math::IntPoint2d topLeft, Math::IntPoint2d bottomRight);

  int minWidth;
  int minHeight;

  Math::IntRect topLeft;
  Math::IntRect topCenter;
  Math::IntRect topRight;
  Math::IntRect middleLeft;
  Math::IntRect middleCenter;
  Math::IntRect middleRight;
  Math::IntRect bottomLeft;
  Math::IntRect bottomCenter;
  Math::IntRect bottomRight;
};

};  // namespace Graphics
