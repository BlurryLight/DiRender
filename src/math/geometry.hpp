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

NAMESPACE_END(DR)
