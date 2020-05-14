#pragma once
#include <cores/intersection.hpp>
#include <cores/primitive.h>
#include <math/geometry.hpp>
#include <vector>
NAMESPACE_BEGIN(DR)
class BVHTree : public Primitive {
  enum class Method { NAIVE, SAH };
  Bounds3 WorldBounds() const override;
  bool Intersect_test(const Ray &ray) const override;
  bool Intersect(const Ray &ray, Intersection *isect) const override;
  BVHTree(std::vector<std::shared_ptr<Primitive>> prims, int maxPrimsInNode = 1,
          Method splitmethod = Method::NAIVE);

private:
  struct BvhNode {
    BvhNode() {
      bounds = Bounds3();
      left_ = right_ = nullptr;
      primitives_ = decltype(primitives_){};
      area = 0.0f;
      nPrimitives = 0;
    }
    Bounds3 bounds;
    std::shared_ptr<BvhNode> left_;
    std::shared_ptr<BvhNode> right_;
    std::vector<std::shared_ptr<Primitive>> primitives_;
    float area;
    int nPrimitives;
  };
  std::vector<std::shared_ptr<Primitive>> prims_;
  std::shared_ptr<BvhNode> root_;
  int maxPrimsInNode = 1;
  Method method = Method::NAIVE;
  std::shared_ptr<Primitive> recursiveBuild();
};

NAMESPACE_END(DR)
