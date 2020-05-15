#pragma once

#include <math/math_utils.hpp>
#include <utils/di_global.h>

// Vector
#include <math/vector.hpp>
// Matrix
#include <math/matrix.hpp>

NAMESPACE_BEGIN(DR)

inline Vector3f reflect(const Vector3f &in, const Vector3f &normal) {
  return in - 2 * dot(in, normal) * normal;
}

// coords,pdf
inline std::pair<Vector3f, float> uniform_sample_sphere() {
  static float pdf = 1.0f / (4 * kPi);
  float r1 = get_random_float();
  float r2 = get_random_float();
  float phi = 2 * kPi * r1;
  float z = 1 - 2 * r2;
  auto tmp = 2 * std::sqrt(r2 * (1 - r2));
  float x = std::cos(phi) * tmp;
  float y = std::sin(phi) * tmp;
  return {{x, y, z}, pdf};
}

inline std::pair<Vector3f, float> uniform_sample_hemisphere() {
  static float pdf = 1.0f / (2 * kPi);
  float r1 = get_random_float();
  float r2 = get_random_float();
  float phi = 2 * kPi * r2;
  float z = r1;
  auto tmp = std::sqrt(1 - z * z);
  float x = std::cos(phi) * tmp;
  float y = std::sin(phi) * tmp;
  return {{x, y, z}, pdf};
}

inline std::pair<Vector3f, float> cosine_sample_hemisphere() {
  float r1 = get_random_float();
  float r2 = get_random_float();
  float phi = 2 * kPi * r1;
  auto tmp = std::sqrt(r2);
  float x = std::cos(phi) * tmp;
  float y = std::sin(phi) * tmp;
  float z = std::sqrt(1 - r2);
  return {{x, y, z}, z * k1_Pi};
}

NAMESPACE_END(DR)
