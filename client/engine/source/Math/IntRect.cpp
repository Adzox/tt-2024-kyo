#include "Math/IntRect.hpp"

namespace Math {

IntRect::IntRect(int x, int y, int width, int height) : x(x), y(y), width(width), height(height) {}

IntRect::IntRect(const IntRect& rect) : x(rect.x), y(rect.y), width(rect.width), height(rect.height) {}

}  // namespace Math
