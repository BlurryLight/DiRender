//
// Created by panda on 2021/2/28.
//

#ifndef DIRENDER_QUAD_H
#define DIRENDER_QUAD_H

#include <array>
#include <shapes/shape.h>
#include <shapes/triangle.h>
#include <utils/di_global.h>

namespace DR {
class Quad final : public Shape {
public:
  Bounds3 ObjectBounds() const override;
  Bounds3 WorldBounds() const override;
  bool Intersect(const Ray &ray, float *time = nullptr,
                 Intersection *isect = nullptr) const override;
  float Area() const override;
  std::pair<Intersection, float> sample() const override;
  // This function is meaningless
  //  std::pair<Intersection, float> sample(const Point3f &ref) const override {
  //    ignore(ref);
  //    return sample();
  //  }

  Quad(observer_ptr<Transform> LocalToWorld,
       observer_ptr<Transform> WorldToObject, Point3f a, Point3f b, Point3f c,
       bool reverse);

  std::array<std::unique_ptr<Triangle>, 2> tris_;
};
} // namespace DR

#endif // DIRENDER_QUAD_H
