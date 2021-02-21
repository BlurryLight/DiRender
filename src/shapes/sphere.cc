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
  if (t0 > 1e-2 && t0 > ray.tMin_ && t0 < ray.tMax_) {
    if (time != nullptr) {
      *time = t0;
    }
    if (isect != nullptr) {
      isect->happened = true;
      isect->coords = ray.at(t0);
      auto local_normal = (objRay.at(t0) - Point3f{0.0f}).normalize();
      // from https://en.wikipedia.org/wiki/UV_mapping
      float u = 0.5 + std::atan2(local_normal.z, local_normal.x) * k1_2Pi;
      float v = 0.5 - std::asin(local_normal.y) * k1_Pi;
      isect->texcoords = {u, v};
      isect->normal = ((*LocalToWorld)(local_normal)).normalize();
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

std::pair<Intersection, float> Sphere::sample() const {
  Point3f sample_point;
  float pdf;
  std::tie(sample_point, pdf) = uniform_sample_sphere();
  Intersection result;
  result.coords = (*LocalToWorld)(radius_ * sample_point);
  result.normal = (*LocalToWorld)(static_cast<Normal3f>(sample_point),
                                  this->reverseOrientation);
  return {result, pdf};
}

std::pair<Intersection, float> Sphere::sample(const Point3f &ref) const {
  auto center = (*LocalToWorld)(Point3f{0.0f});
  float distance_squared = (center - ref).squared_length();
  Intersection result;
  if (distance_squared <= radius_ * radius_) // point is inside or on the sphere
  {
    return {result, 0.0};
  }
  float cos_theta_max = sqrt(1 - radius_ * radius_ / distance_squared);
  float solid_angle = 2 * kPi * (1 - cos_theta_max);

  float pdf = 1.0 / solid_angle;

  auto r1 = get_random_float();
  auto r2 = get_random_float();
  float z = 1 + r2 * (cos_theta_max - 1);

  auto phi = 2 * kPi * r1;
  float x = cos(phi) * sqrt(1 - z * z);
  float y = sin(phi) * sqrt(1 - z * z);

  auto localPoint = Point3f{x, y, z};
  auto coords = (*LocalToWorld)(radius_ * localPoint);
  Ray r(ref, coords - ref);
  if (this->Intersect(r)) {
    result.coords = coords;
    result.normal = (*LocalToWorld)(static_cast<Normal3f>(localPoint),
                                    this->reverseOrientation);
    return {result, pdf};
  }
  // if the sample point is not visible to ref
  return {result, 0.0};
}
