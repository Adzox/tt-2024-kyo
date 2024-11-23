#pragma once

#include "Vector.hpp"

namespace Math {

class FloatPoint2d;

class IntPoint2d : public Vector<2, int> {
 public:
  IntPoint2d();
  IntPoint2d(int x, int y);
  IntPoint2d(const IntPoint2d& v);
  IntPoint2d(const Vector<2, int>& v);
  explicit IntPoint2d(const FloatPoint2d& point);

  int GetX() const {
    return (*this)[0];
  }

  int GetY() const {
    return (*this)[1];
  }

  void SetX(int x) {
    (*this)[0] = x;
  }

  void SetY(int y) {
    (*this)[1] = y;
  }
};

class FloatPoint2d : public Vector<2, float> {
 public:
  FloatPoint2d();
  FloatPoint2d(float x, float y);
  FloatPoint2d(const FloatPoint2d& v);
  FloatPoint2d(const Vector<2, float>& v);
  explicit FloatPoint2d(const IntPoint2d& point);

  float SquaredLength() const;

  float Length() const;

  void Normalize();

  FloatPoint2d Rotated(float angle) const;

  float GetX() const {
    return (*this)[0];
  }

  float GetY() const {
    return (*this)[1];
  }

  void SetX(float x) {
    (*this)[0] = x;
  }

  void SetY(float y) {
    (*this)[1] = y;
  }
};

}  // namespace Math
