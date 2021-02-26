#pragma once
#include <cores/bounds.h>
#include <cores/ray.hpp>
#include <math/transform.h>
#include <memory>

NAMESPACE_BEGIN(DR)
struct Intersection;
class Shape {
public:
  Shape(observer_ptr<Transform> LocalToWorld,
        observer_ptr<Transform> WorldToLocal, bool reverseOrientation)
      : LocalToWorld(LocalToWorld), WorldToObject(WorldToLocal),
        reverseOrientation(reverseOrientation) {
    assert((LocalToWorld->m_ * WorldToLocal->m_).is_identity());
    assert((LocalToWorld->mInv_ * WorldToLocal->mInv_).is_identity());
    assert((LocalToWorld->mInv_ * LocalToWorld->m_).is_identity());
    assert((WorldToLocal->mInv_ * WorldToLocal->m_).is_identity());
  }
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
  virtual std::pair<Intersection, float>
  sample() const = 0; // sample position and pdf
  virtual std::pair<Intersection, float> sample(const Point3f &ref) const {
    ignore(ref);
    return sample();
  }
  //  virtual bool HasEmission() = 0; // Should be in Primitive

  observer_ptr<Transform> LocalToWorld = nullptr, WorldToObject = nullptr;
  bool reverseOrientation;
};

NAMESPACE_END(DR)
