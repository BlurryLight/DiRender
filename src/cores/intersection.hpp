#pragma once

#include <math/geometry.hpp>
#include <utils/di_global.h>
NAMESPACE_BEGIN(DR)
struct Primitive;
struct Material;

struct Intersection {
  Intersection() {
    happened = false;
    coords = Point3f{};
    normal = Vector3f{};
    texcoords = Point2f{};
    mat_ptr = nullptr;
    primi_ptr = nullptr;
    t = kFloatInfinity;
  }
  float t;
  bool happened;
  Point3f coords;
  Point2f texcoords;
  Normal3f normal;
  Material *mat_ptr;
  Primitive *primi_ptr;
};
NAMESPACE_END(DR)
