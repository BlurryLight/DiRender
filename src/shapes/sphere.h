#pragma once
#include <utils/di_global.h>
#include <math/geometry.hpp>
#include <shapes/shape.h>
#include <math/transform.h>

NAMESPACE_BEGIN(DR)
class Sphere : public Shape
{
public:
  union {
  float radius_;
  float r_;
  };
  Sphere(std::shared_ptr<Transform> LocalToWorld,
         std::shared_ptr<Transform> WorldToLocal, bool reverseOrientation,
         float radius)
      : Shape(LocalToWorld, WorldToLocal, reverseOrientation), radius_(radius) {
  }
  Bounds3 WorldBounds() override;
  Bounds3 ObjectBounds() override;
  bool Intersect(const Ray &ray, float *time = nullptr, Intersection *isect = nullptr) const override;
  float Area() const override;
  std::pair<Intersection, float> sample() const override; //uniform
  std::pair<Intersection, float> sample(const Point3f& ref) const; //uniform
  float solidAngle(const Point3f& p) const
  {
    Point3f center = (*LocalToWorld)(Point3f{0.0f});
    float distance = (center - p).squared_length();
    if(distance < radius_ * radius_) return 4 * kPi;
    float sinthetaMax2 = r_ * r_ / distance;
    float costhetaMax =  std::sqrt(std::max(0.0f,1 - sinthetaMax2));
    return 2 * kPi * (1 - costhetaMax);
  }

};

NAMESPACE_END(DR)
