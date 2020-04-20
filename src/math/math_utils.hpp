#ifndef DI_MATH_UTILS_HPP
#define DI_MATH_UTILS_HPP

#include <cmath>
#include <limits>
#include <numeric>
#include <utils/di_global.h>
#include <algorithm>

NAMESPACE_BEGIN(DR)
constexpr float kFloatInfinity = std::numeric_limits<float>::max();
constexpr float kFloatMax = kFloatInfinity;
constexpr float kFloatMin = -kFloatMax;
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

NAMESPACE_END(DR)

#endif
