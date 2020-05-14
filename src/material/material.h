#pragma once
#include <math/geometry.hpp>
#include <utils/di_global.h>
#include <cores/ray.hpp>
#include <cores/intersection.hpp>
NAMESPACE_BEGIN(DR)

struct Material
{
  virtual ~Material(){};
  virtual Vector3f evalBxDF(const Ray& r_in,const Intersection& isect,const Ray& r_out) const = 0;
  virtual std::pair<Vector3f,float> evalScatter(const Ray& r_in,const Intersection& isect) const = 0;
  virtual Vector3f evalEmitted(const Ray& r_in,const Intersection& isect) const
  {
    return {};
  }
};


//class SubsurfaceMaterial : public Material
//{

//};

NAMESPACE_END(DR)
