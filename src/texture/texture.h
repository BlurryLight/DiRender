#pragma once
#include <cores/intersection.hpp>
#include <math/geometry.hpp>
#include <utils/di_global.h>
NAMESPACE_BEGIN(DR)
class Texture {
public:
  virtual Vector3f evalValue(const Intersection &isect) const = 0;
  virtual ~Texture() {}
};
NAMESPACE_END(DR)
