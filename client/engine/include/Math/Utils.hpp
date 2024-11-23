#pragma once

#include <cmath>

namespace Math {

/**
 * Clamps the float between two numbers, ensuring number is within [low, high]
 */
float Clamp(float number, float low, float high);

float ApproximatelyEqual(float a, float b, float epsilon = 0.00000001f);

float Lerp(float a, float b, float t);

}  // namespace Math
