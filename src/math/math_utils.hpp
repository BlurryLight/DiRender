#pragma once

#include <algorithm>
#include <cmath>
#include <limits>
#include <numeric>
#include <random>
#include <thread>
#include <utils/di_global.h>

NAMESPACE_BEGIN(DR)
constexpr float kFloatInfinity = std::numeric_limits<float>::max();
constexpr float kFloatMax = kFloatInfinity;
constexpr float kFloatMin = std::numeric_limits<float>::min();
constexpr float kFloatLowest = -kFloatMax;
constexpr float kEpsilon = std::numeric_limits<float>::epsilon();
constexpr float kPi = 3.14159265358979323846;
constexpr float kPi_2 = 1.57079632679489661923; // pi/2
constexpr float kPi_4 = 0.78539816339744830962;
constexpr float k1_Pi = 0.31830988618379067154; // 1/pi
constexpr float k2_Pi = 0.63661977236758134308; // 2/pi
constexpr float kSqrt2 = 1.41421356237309504880;

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

inline bool almost_equal(float x, float y) {
  if (x == y)
    return true;
  return impl::boost_epsilon_difference(x, y) <
         3; // Binary predicate to support <algorithm>
}

inline bool almost_equal(float x, float y,
                         int ulp_num) // units of least precision, or tolarance
{
  if (x == y)
    return true;
  return impl::boost_epsilon_difference(x, y) < ulp_num;
}

inline float deg2rad(const float &deg) { return deg * kPi / 180.0f; }
inline float rad2deg(const float &rad) { return rad * k1_Pi * 180.0f; }

inline float get_random_float(float min = 0.0f, float max = 1.0f) {
  static thread_local std::mt19937 generator(std::random_device{}());
  std::uniform_real_distribution<> dis(min, max);
  return dis(generator);
}

NAMESPACE_END(DR)
