#pragma once

#include <utils/di_global.h>
#include <math/geometry.hpp>
NAMESPACE_BEGIN(DR)
struct Primitive;
struct Material;

struct Intersection
{
  Intersection()
  {
    happened = false;
    coords = Vector3f{};
    normal = Vector3f{};
    texcoords = Point2f{};
    mat_ptr = nullptr;
    primi_ptr = nullptr;
    t = kFloatInfinity;
  }
  float t;
  bool happened;
  Vector3f coords;
  Point2f texcoords;
  Normal3f normal;
  Material* mat_ptr;
  Primitive* primi_ptr;
};
NAMESPACE_END(DR)
