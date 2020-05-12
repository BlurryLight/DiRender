#pragma once
#include <cores/ray.hpp>
#include <math/geometry.hpp>
#include <bitset>

NAMESPACE_BEGIN(DR)
class Bounds3 {
public:
  Bounds3();
  Bounds3(const Bounds3 &other);
  Bounds3(const Point3f &pMin, const Point3f &pMax);
  const Point3f &operator[](int index) const;
  bool operator==(const Bounds3 &other) const;
  bool operator!=(const Bounds3 &other) const;
  Vector3f diagonal() const;
  float surface_area() const;
  float volume() const;
  int max_extent() const;
  Vector3f centroid() const;
  Vector3f offset(const Point3f &p) const;
  std::pair<Point3f, float> bounding_sphere() const;
  Bounds3 intersect(const Bounds3 &other) const;
  bool is_overlapped(const Bounds3 &other) const;
  bool is_inside(const Point3f &p) const;
  bool Intersect_test(const Ray &ray, const Vector3f &invDir,
                      std::bitset<3> dirIsNeg);

  static Bounds3 Union(const Bounds3 &bs,const Point3f& p);
  static Bounds3 Union(const Bounds3 &lhs,const Bounds3& rhs);
  static Bounds3 Intersect(const Bounds3 &lhs,const Bounds3& rhs);
  static bool Overlaps(const Bounds3 &lhs,const Bounds3& rhs);
  static bool Inside(const Point3f &p,const Bounds3& bs);

private:
  Point3f pMin_, pMax_;
};

NAMESPACE_END(DR)
