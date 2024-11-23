#include "Math/Point2d.hpp"

#include <cmath>
#include <numbers>

namespace Math {

IntPoint2d::IntPoint2d() : IntPoint2d(0, 0) {}

IntPoint2d::IntPoint2d(int x, int y) : Vector<2, int>({x, y}) {}

IntPoint2d::IntPoint2d(const IntPoint2d& v) : IntPoint2d(v.GetX(), v.GetY()) {}

IntPoint2d::IntPoint2d(const Vector<2, int>& v) : Vector<2, int>(v) {}

IntPoint2d::IntPoint2d(const FloatPoint2d& point)
    : Vector<2, int>({static_cast<int>(point.GetX()), static_cast<int>(point.GetY())}) {}

FloatPoint2d::FloatPoint2d() : FloatPoint2d(0, 0) {}

FloatPoint2d::FloatPoint2d(float x, float y) : Vector<2, float>({x, y}) {}

FloatPoint2d::FloatPoint2d(const FloatPoint2d& v) : FloatPoint2d(v.GetX(), v.GetY()) {}

FloatPoint2d::FloatPoint2d(const Vector<2, float>& v) : Vector<2, float>(v) {}

FloatPoint2d::FloatPoint2d(const IntPoint2d& point)
    : Vector<2, float>({static_cast<float>(point.GetX()), static_cast<float>(point.GetY())}) {}

float FloatPoint2d::SquaredLength() const {
  return GetX() * GetX() + GetY() * GetY();
}

float FloatPoint2d::Length() const {
  return std::sqrt(SquaredLength());
}

void FloatPoint2d::Normalize() {
  (*this) /= Length();
}

FloatPoint2d FloatPoint2d::Rotated(float angle) const {
  return {GetX() * std::cosf(angle / 180 * std::numbers::pi) - GetY() * std::sinf(angle / 180 * std::numbers::pi),
          GetX() * std::sinf(angle / 180 * std::numbers::pi) + GetY() * std::cosf(angle / 180 * std::numbers::pi)};
}

}  // namespace Math
