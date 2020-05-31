#pragma once
#include <cores/intersection.hpp>
#include <cores/primitive.h>
#include <math/geometry.hpp>
#include <vector>

NAMESPACE_BEGIN(DR)
// Linear List should be used only for debugging
class LinearList : public Primitive {
public:
  Bounds3 WorldBounds() const override;
  bool Intersect_test(const Ray &ray) const override;
  bool Intersect(const Ray &ray, Intersection *isect) const override;
  LinearList(std::vector<std::shared_ptr<Primitive>> prims) : prims_(prims) {}
  std::pair<Intersection, float> sample() const override {
    return prims_[0]->sample();
  }
  std::pair<Intersection, float> sample(const Point3f &ref) const override {
    return prims_[0]->sample(ref);
  }

private:
  std::vector<std::shared_ptr<Primitive>> prims_;
};

NAMESPACE_END(DR)
