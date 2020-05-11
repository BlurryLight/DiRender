#ifndef DI_RAY_HPP
#define DI_RAY_HPP
#include <math/geometry.hpp>
#include <utils/di_global.h>

NAMESPACE_BEGIN(DR)
class Ray {
public:
  Point3f origin_;
  Vector3f direction_;
  float tMax_;
  float time_;

public:
  Ray() : time_(0.0f), tMax_(kFloatInfinity) {}
  Ray(const Point3f &origin, const Vector3f &direction,
      float tMax = kFloatInfinity, float time = 0.f)
      : origin_(origin), direction_(direction) {
    time_ = time;
    tMax_ = tMax;
  }
  Point3f at(float time) const { return origin_ + time * direction_; }
  bool has_nan() const {
    return origin_.has_nan() || direction_.has_nan() || std::isnan(tMax_) ||
           std::isnan(time_);
  }
};

NAMESPACE_END(DR)

#endif
