#pragma once

#include <Math/Vector.hpp>

#include <cstdint>

namespace Graphics {

class ColorHsva;

class ColorRgba : public Math::Vector<4, uint8_t> {
 public:
  ColorRgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
  ColorRgba(const ColorRgba& c);
  ColorRgba(const Vector<4, uint8_t>& v);
  ColorRgba(const ColorHsva& c);

  /**
   * Linearly interpolates between color a and b by t amount.
   *
   * Will interpolate linearly in RGB space.
   */
  static ColorRgba Lerp(const ColorRgba& a, const ColorRgba& b, float t);

  uint8_t GetR() const {
    return (*this)[0];
  }

  uint8_t GetG() const {
    return (*this)[1];
  }

  uint8_t GetB() const {
    return (*this)[2];
  }

  uint8_t GetAlpha() const {
    return (*this)[3];
  }

  void SetR(uint8_t r) {
    (*this)[0] = r;
  }

  void SetG(uint8_t g) {
    (*this)[1] = g;
  }

  void SetB(uint8_t b) {
    (*this)[2] = b;
  }

  void SetAlpha(uint8_t alpha) {
    (*this)[3] = alpha;
  }
};

class ColorHsva : public Math::Vector<4, float> {
 public:
  ColorHsva(float h, float s, float v, float a);
  ColorHsva(const ColorHsva& c);
  ColorHsva(const Vector<4, float>& v);
  ColorHsva(const ColorRgba& c);

  /**
   * Linearly interpolates between color a and b by t amount.
   *
   * Will interpolate linearly in HSV space.
   */
  static ColorHsva Lerp(const ColorHsva& a, const ColorHsva& b, float t);

  float GetHue() const {
    return (*this)[0];
  }

  float GetSaturation() const {
    return (*this)[1];
  }

  float GetValue() const {
    return (*this)[2];
  }

  float GetAlpha() const {
    return (*this)[3];
  }

  void SetHue(float hue) {
    (*this)[0] = hue;
  }

  void SetSaturation(float saturation) {
    (*this)[1] = saturation;
  }

  void SetValue(float value) {
    (*this)[2] = value;
  }

  void SetAlpha(float alpha) {
    (*this)[3] = alpha;
  }
};

}  // namespace Graphics