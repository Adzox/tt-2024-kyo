#include "Graphics/Color.hpp"

#include <Math/Utils.hpp>

#include <cmath>

namespace Graphics {

ColorHsva::ColorHsva(float hue, float saturation, float value, float alpha)
    : Vector<4, float>({hue, saturation, value, alpha}) {}
ColorHsva::ColorHsva(const ColorHsva& color)
    : ColorHsva(color.GetHue(), color.GetSaturation(), color.GetValue(), color.GetAlpha()) {}
ColorHsva::ColorHsva(const Vector<4, float>& v) : Vector<4, float>(v) {}
ColorHsva::ColorHsva(const ColorRgba& color) {
  float r = color.GetR() / 255.f;
  float g = color.GetG() / 255.f;
  float b = color.GetB() / 255.f;

  float min = std::min(std::min(r, g), b);
  float max = std::max(std::max(r, g), b);
  float delta = max - min;

  float h;
  if (max == 0) {
    h = 0;
  } else if (r == max) {
    h = std::fmod((g - b) / delta, 6);
  } else if (g == max) {
    h = ((b - r) / delta) + 2;
  } else {
    h = ((r - g) / delta) + 4;
  }
  h *= 60;
  if (h < 0) {
    h += 360.f;
  }

  float s = max > 0 ? delta / max : 0;
  float v = max;

  SetHue(h);
  SetSaturation(s);
  SetValue(v);
  SetAlpha(color.GetAlpha() / 255.f);
}

ColorHsva ColorHsva::Lerp(const ColorHsva& a, const ColorHsva& b, float t) {
  ColorHsva color = a + (b - a) * t;
  color.SetHue(std::fmod(color.GetHue(), 360));
  color.SetSaturation(Math::Clamp(color.GetSaturation(), 0, 1));
  color.SetValue(Math::Clamp(color.GetValue(), 0, 1));
  color.SetAlpha(Math::Clamp(color.GetAlpha(), 0, 1));
  return color;
}

ColorRgba::ColorRgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : Vector<4, uint8_t>({r, g, b, a}) {}
ColorRgba::ColorRgba(const ColorRgba& color) : ColorRgba(color.GetR(), color.GetG(), color.GetB(), color.GetAlpha()) {}
ColorRgba::ColorRgba(const Vector<4, uint8_t>& v) : Vector<4, uint8_t>(v) {}
ColorRgba::ColorRgba(const ColorHsva& color) {
  float chroma = color.GetSaturation() * color.GetValue();
  float x = chroma * (1 - std::fabs(std::fmod(color.GetHue() / 60.f, 2) - 1));
  float m = color.GetValue() - color.GetSaturation();
  float hue = std::fmod(color.GetHue(), 360);

  float r = 0;
  float g = 0;
  float b = 0;

  if (0 <= hue && hue <= 60) {
    r = chroma;
    g = x;
  } else if (60 < hue && hue <= 120) {
    r = x;
    g = chroma;
  } else if (120 < hue && hue <= 180) {
    g = chroma;
    b = x;
  } else if (180 < hue && hue <= 240) {
    g = x;
    b = chroma;
  } else if (240 < hue && hue <= 300) {
    r = x;
    b = chroma;
  } else {
    r = chroma;
    b = x;
  }

  r += m;
  g += m;
  b += m;

  SetR(static_cast<uint8_t>(r * 255.f));
  SetG(static_cast<uint8_t>(g * 255.f));
  SetB(static_cast<uint8_t>(b * 255.f));
  SetAlpha(static_cast<uint8_t>(color.GetAlpha() * 255.f));
}

ColorRgba ColorRgba::Lerp(const ColorRgba& a, const ColorRgba& b, float t) {
  ColorRgba color = a + (b - a) * t;
  color.SetR(Math::Clamp(color.GetR(), 0, 255));
  color.SetG(Math::Clamp(color.GetG(), 0, 255));
  color.SetB(Math::Clamp(color.GetB(), 0, 255));
  color.SetAlpha(Math::Clamp(color.GetAlpha(), 0, 255));
  return color;
}
}  // namespace Graphics