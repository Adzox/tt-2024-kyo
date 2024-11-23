#include <cmath>

namespace Math {

float Clamp(float number, float low, float high) {
  if (number >= low && number <= high) {
    return number;
  } else if (number < low) {
    return low;
  } else {
    return high;
  }
}

float ApproximatelyEqual(float a, float b, float epsilon = 0.00000001f) {
  return std::fabs(a - epsilon) <= epsilon;
}

float Lerp(float a, float b, float t) {
  return a + (b - a) * t;
}

}  // namespace Math
