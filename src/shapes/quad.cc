//
// Created by panda on 2021/2/28.
//

#include "quad.h"
using namespace DR;
Quad::Quad(observer_ptr<Transform> LocalToWorld,
           observer_ptr<Transform> WorldToObject, Point3f a, Point3f b,
           Point3f c, bool reverse)
    : Shape(LocalToWorld, WorldToObject, reverse)

// clang-format off
/*
   *            (2)c ------- d(3)
   *               / \    /
 *                /    \ /
 *               /------/
 *             a(0)    b(1)
 *        index: 0 1 2
 *        index: 1 3 2
 */
// clang-format on
{
  vec3 ad = (b - a) + (c - a);
  Point3f d = a + ad;
  std::vector<Point3f> vertices{a, b, c, d};
  int nVertices = 4;
  int nTriangles = 2;
  std::vector<DR::uint> vertexIndices{0, 1, 2, 1, 3, 2};
  Normal3f normal = reverse ? (c - a).cross(b - a) : (b - a).cross(c - a);
  normal = normal.normalize();
  std::vector<Normal3f> normals(4, normal);
  std::vector<Point2f> uvs{{0, 0}, {0, 1}, {1, 0}, {1, 1}};
  std::vector<int> faceIndices(nTriangles, 0);
  auto mesh_ptr = std::make_shared<TriangleMesh>(
      LocalToWorld, nTriangles, nVertices, vertexIndices.data(),
      vertices.data(), normals.data(), uvs.data(), faceIndices.data());
  this->tris_[0] = std::make_unique<Triangle>(LocalToWorld, WorldToObject,
                                              reverse, mesh_ptr, 0);
  this->tris_[1] = std::make_unique<Triangle>(LocalToWorld, WorldToObject,
                                              reverse, mesh_ptr, 1);
}
Bounds3 Quad::ObjectBounds() const {
  return Bounds3::Union(tris_[0]->ObjectBounds(), tris_[1]->ObjectBounds());
}
Bounds3 Quad::WorldBounds() const {

  return Bounds3::Union(tris_[0]->WorldBounds(), tris_[1]->WorldBounds());
}
bool Quad::Intersect(const Ray &ray, float *time, Intersection *isect) const {
  if (tris_[0]->Intersect(ray, time, isect))
    return true;
  return tris_[1]->Intersect(ray, time, isect);
}
float Quad::Area() const { return tris_[0]->Area() + tris_[1]->Area(); }
std::pair<Intersection, float> Quad::sample() const {
  // naive sampler
  int index = get_random_float() > 0.5 ? 0 : 1;
  return tris_[index]->sample();
}
