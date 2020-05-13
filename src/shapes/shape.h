#pragma once
#include <cores/ray.hpp>
#include <cores/bounds.h>
#include <memory>

NAMESPACE_BEGIN(DR)
struct Transform;
struct Intersection;
class Shape
{
public:
  Shape(std::shared_ptr<Transform> LocalToWorld,std::shared_ptr<Transform> WorldToObject, bool reverseOrientation);
  virtual ~Shape();
  virtual Bounds3 ObjectBounds() = 0;
  virtual Bounds3 WorldBounds() = 0;
  virtual bool Intersect(const Ray& ray, float* time=nullptr,Intersection* isect=nullptr) const;
  virtual float Area() const = 0;
  virtual std::pair<Intersection,float> sample() const = 0; //sample position and pdf
  virtual bool HasEmission() = 0;

  bool reverseOrientation;
  std::shared_ptr<Transform> LocalToWorld,WorldToObject;

};

NAMESPACE_END(DR)
