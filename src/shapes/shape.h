#pragma once
#include <cores/ray.hpp>
#include <cores/bounds.h>
#include <memory>

NAMESPACE_BEGIN(DR)
class Transform;
struct Intersection;
class Shape
{
public:
  Shape(std::shared_ptr<Transform> LocalToWorld,
        std::shared_ptr<Transform> WorldToLocal, bool reverseOrientation)
      : LocalToWorld(LocalToWorld), WorldToObject(WorldToLocal),
        reverseOrientation(reverseOrientation) {}
  virtual ~Shape(){};
  virtual Bounds3 ObjectBounds() const = 0;
  virtual Bounds3 WorldBounds() const = 0;
  virtual bool Intersect(const Ray &ray, float *time = nullptr,
                         Intersection *isect = nullptr) const {
    ignore(ray);
    ignore(time);
    ignore(isect);
    return false;
  };
  virtual float Area() const = 0;
  virtual std::pair<Intersection,float> sample() const = 0; //sample position and pdf
  virtual std::pair<Intersection,float> sample(const Point3f& ref) const
  {
    ignore(ref);
    return sample();
  }
//  virtual bool HasEmission() = 0; // Should be in Primitive

  std::shared_ptr<Transform> LocalToWorld,WorldToObject;
  bool reverseOrientation;
};

NAMESPACE_END(DR)
