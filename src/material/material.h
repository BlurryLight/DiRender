#pragma once
#include <math/geometry.hpp>
#include <utils/di_global.h>
#include <cores/ray.hpp>
#include <cores/intersection.hpp>
NAMESPACE_BEGIN(DR)

struct Material
{
  virtual ~Material(){};
  virtual Vector3f evalBxDF(const Vector3f &r_in, const Intersection &isect,
                            const Vector3f &r_out) const = 0;
  virtual std::pair<Vector3f, float>
  sampleScatter(const Vector3f &r_in, const Intersection &isect) const = 0;
  virtual Vector3f evalEmitted(const Vector3f &r_in,
                               const Intersection &isect) const {
    ignore(r_in);
    ignore(isect);
    return {};
  }
};


//class SubsurfaceMaterial : public Material
//{

//};

NAMESPACE_END(DR)
