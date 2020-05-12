#pragma once

#include <algorithm>
#define _USE_MATH_DEFINES // to make MSVC happy
#include <cmath>
#include <limits>
#include <numeric>
#include <utils/di_global.h>

NAMESPACE_BEGIN(DR)
constexpr float kFloatInfinity = std::numeric_limits<float>::max();
constexpr float kFloatMax = kFloatInfinity;
constexpr float kFloatMin = std::numeric_limits<float>::min();
constexpr float kFloatLowest = -kFloatMax;
constexpr float kEpsilon = std::numeric_limits<float>::epsilon();
constexpr float kPi = M_PI;
constexpr float kPi_2 = M_PI_2; // pi/2
constexpr float kPi_4 = M_PI_4;
constexpr float k1_Pi = M_1_PI; // 1/pi
constexpr float k2_Pi = M_2_PI;

template <typename T>
inline T clamp(const T &low, const T &high, const T &value) {
  return std::max(low, std::min(high, value));
}

template <typename T> inline T lerp(const T &l, const T &r, const float &t) {
  return l * (1 - t) + r * t;
}

NAMESPACE_BEGIN(impl)
// from boost
inline float boost_relative_difference(float a, float b) {
  return std::fabs((a - b) / std::min(a, b));
}

inline float boost_epsilon_difference(float a, float b) {
  return boost_relative_difference(a, b) / kEpsilon;
}
NAMESPACE_END(impl)

inline bool
almost_equal(float x, float y,
             int ulp_num = 3) // units of least precision, or tolarance
{
  if (x == y)
    return true;
  return impl::boost_epsilon_difference(x, y) < ulp_num;
}

inline float deg2rad(const float &deg) { return deg * kPi / 180.0f; }
inline float rad2deg(const float &rad) { return rad * k1_Pi * 180.0f; }

NAMESPACE_END(DR)
