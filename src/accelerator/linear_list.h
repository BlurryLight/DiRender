#pragma once
#include <cores/intersection.hpp>
#include <cores/primitive.h>
#include <math/geometry.hpp>
#include <vector>

NAMESPACE_BEGIN(DR)
// Linear List should be used only for debugging
class LinearList : public Primitive {
public:
  Bounds3 WorldBounds() const override {
    Bounds3 ret;
    for (auto i : prims_) {
      ret = Bounds3::Union(ret, i->WorldBounds());
    }
    return ret;
  }
  bool Intersect_test(const Ray &ray) const override {
    for (auto i : prims_) {
      if (i->Intersect_test(ray)) {
        return true;
        break;
      }
    }
    return false;
  }
  bool Intersect(const Ray &ray, Intersection *isect) const override {
    Intersection tmpIsect;
    Intersection tmpIsect2;
    float t_max = kFloatMax;
    bool hit = false;
    for (auto i : prims_) {
      if (i->Intersect(ray, &tmpIsect)) {
        hit = true;
        if (tmpIsect.t < t_max) {
          t_max = tmpIsect.t;
          tmpIsect2 = tmpIsect;
        }
      }
    }
    *isect = tmpIsect2;
    return hit;
  }
  LinearList(std::vector<std::shared_ptr<Primitive>> prims) : prims_(prims) {}

private:
  std::vector<std::shared_ptr<Primitive>> prims_;
};

NAMESPACE_END(DR)
