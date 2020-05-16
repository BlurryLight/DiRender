#include <cores/intersection.hpp>
#include <shapes/sphere.h>
using namespace DR;
static bool solveQuadratic(const float &a, const float &b, const float &c,
                           float &x0, float &x1) {
  float discr = b * b - 4 * a * c;
  if (discr < 0)
    return false;
  else if (discr == 0)
    x0 = x1 = -0.5 * b / a;
  else {
    float q = (b > 0) ? -0.5 * (b + sqrt(discr)) : -0.5 * (b - sqrt(discr));
    x0 = q / a;
    x1 = c / q;
  }
  if (x0 > x1)
    std::swap(x0, x1);
  return true;
}

Bounds3 Sphere::WorldBounds() const { return (*LocalToWorld)(ObjectBounds()); }

Bounds3 Sphere::ObjectBounds() const {
  return Bounds3{{-r_, -r_, -r_}, {r_, r_, r_}};
}

bool Sphere::Intersect(const Ray &ray, float *time, Intersection *isect) const {
  Intersection result;
  Ray objRay = (*WorldToObject)(ray);
  Vector3f L = objRay.origin_ - Point3f{0.0f};
  float a = dot(objRay.direction_, objRay.direction_);
  float b = 2 * dot(objRay.direction_, L);
  float c = dot(L, L) - radius_ * radius_;
  float t0, t1;
  if (!solveQuadratic(a, b, c, t0, t1))
    return false;
  if (t0 < 0)
    t0 = t1;
  if (t0 < 0)
    return false;
  if (t0 > 1e-2) {
    if (time != nullptr) {
      *time = t0;
    }
    if (isect != nullptr) {
      isect->happened = true;
      isect->coords = static_cast<Vector3f>(ray.at(t0));
      isect->normal =
          ((*LocalToWorld)(objRay.at(t0) - Point3f{0.0f})).normalize();
      isect->t = t0;
    }
    return true;
  }
  return false;
}

float Sphere::Area() const {
  // object space
  return kPi * 4 * radius_ * radius_;
}
