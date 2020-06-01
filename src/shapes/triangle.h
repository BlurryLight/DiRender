#pragma once
#include <cores/intersection.hpp>
#include <shapes/shape.h>
#include <utils/di_global.h>
NAMESPACE_BEGIN(DR)
#include <vector>
struct TriangleMesh {
  const int nTriangles, nVertices;
  std::vector<int> vertexIndices; // 记录ith三角形所对应的vertex序号
  std::vector<int> faceIndices;   //记录ith三角形对应的face序号
  std::unique_ptr<std::vector<Point3f>> vertices = nullptr;
  std::unique_ptr<std::vector<Normal3f>> normals = nullptr;
  std::unique_ptr<std::vector<Point2f>> uvs = nullptr;
  std::unique_ptr<std::vector<Vector3f>> edges = nullptr;
  TriangleMesh(std::shared_ptr<Transform> LocalToWorld, int nTriangles,
               int nVertices, const DR::uint *vertexIndices,
               const Point3f *vertices, const Normal3f *normals,
               const DR::Point2f *uvs, const int *faceIndices);
};
class Triangle final : public Shape {
public:
  Triangle(std::shared_ptr<Transform> LocalToWorld,
           std::shared_ptr<Transform> WorldToLocal, bool reverse,
           const std::shared_ptr<TriangleMesh> &mesh, int triIndex)
      : Shape(LocalToWorld, WorldToLocal, reverse), mesh_(mesh) {
    vertexIndex_ = &mesh_->vertexIndices[3 * triIndex];
    faceIndex = mesh_->faceIndices.size() ? mesh_->faceIndices.at(triIndex) : 0;
    triIndex_ = triIndex;
  }
  Bounds3 WorldBounds() const override {
    const Point3f &p0 = mesh_->vertices->at(vertexIndex_[0]);
    const Point3f &p1 = mesh_->vertices->at(vertexIndex_[1]);
    const Point3f &p2 = mesh_->vertices->at(vertexIndex_[2]);
    Bounds3 bounds;
    bounds = Bounds3::Union(bounds, p0);
    bounds = Bounds3::Union(bounds, p1);
    bounds = Bounds3::Union(bounds, p2 + 0.0001);
    return bounds;
  }
  Bounds3 ObjectBounds() const override {
    const Point3f &p0 = (*WorldToObject)(mesh_->vertices->at(vertexIndex_[0]));
    const Point3f &p1 = (*WorldToObject)(mesh_->vertices->at(vertexIndex_[1]));
    const Point3f &p2 = (*WorldToObject)(mesh_->vertices->at(vertexIndex_[2]));
    Bounds3 bounds;
    bounds = Bounds3::Union(bounds, p0);
    bounds = Bounds3::Union(bounds, p1);
    bounds = Bounds3::Union(bounds, p2 + 0.0001);

    return bounds;
  }
  float Area() const override // world area
  {
    const Vector3f &edge0 = mesh_->edges->at(3 * triIndex_);
    const Vector3f &edge1 = mesh_->edges->at(3 * triIndex_ + 1);
    return 0.5f * cross(edge0, edge1).length();
  }
  std::pair<Intersection, float> sample() const override {

    const Vector3f &p0 =
        static_cast<Vector3f>(mesh_->vertices->at(vertexIndex_[0]));
    const Vector3f &p1 =
        static_cast<Vector3f>(mesh_->vertices->at(vertexIndex_[1]));
    const Vector3f &p2 =
        static_cast<Vector3f>(mesh_->vertices->at(vertexIndex_[2]));
    float su0 = std::sqrt(get_random_float());
    float x = 1 - su0, y = get_random_float() * su0;
    Intersection pos;
    Vector3f tmp = p0 * x + p1 * y + p2 * (1 - x - y);
    pos.coords = Point3f{tmp.x, tmp.y, tmp.z};
    pos.normal = mesh_->normals->at(vertexIndex_[0]) * x +
                 mesh_->normals->at(vertexIndex_[1]) * y +
                 mesh_->normals->at(vertexIndex_[2]) * (1 - x - y);
    if (reverseOrientation)
      pos.normal *= -1;

    Point2f uvs[3];
    get_uv(uvs);
    Vector2f uv_tmp = static_cast<Vector2f>(uvs[0] * x) +
                      static_cast<Vector2f>(uvs[1] * y) +
                      static_cast<Vector2f>(uvs[2] * (1 - x - y));
    pos.texcoords = Point2f{uv_tmp.x, uv_tmp.y};
    float pdf = 1 / this->Area();
    return {pos, pdf};
  }

  bool Intersect(const Ray &ray, float *time = nullptr,
                 Intersection *isect = nullptr) const override {

    const Point3f &p0 = mesh_->vertices->at(vertexIndex_[0]);
    const Point3f &p1 = mesh_->vertices->at(vertexIndex_[1]);
    const Point3f &p2 = mesh_->vertices->at(vertexIndex_[2]);

    Vector3f edge1 = p1 - p0;
    Vector3f edge2 = p2 - p0;

    Vector3f pvec = cross(ray.direction_, edge2); // S1
    float det = dot(edge1, pvec);
    if (almost_equal(std::abs(det), 0))
      return false;

    float invDet = 1 / det;
    Vector3f tvec = ray.origin_ - p0;
    float u = dot(tvec, pvec);
    u *= invDet;
    if (u < 0 || u > 1.0f)
      return false;

    Vector3f qvec = cross(tvec, edge1);
    float v = dot(ray.direction_, qvec);
    v *= invDet;
    if (v < 0 || u + v > 1.0f)
      return false;

    float tnear = dot(edge2, qvec) * invDet;
    if (tnear > 1e-3) {
      if (time) {
        *time = tnear;
      }
      if (isect) {
        isect->t = tnear;
        isect->coords = ray.at(tnear);
        isect->normal = mesh_->normals->at(vertexIndex_[0]) * (1 - u - v) +
                        mesh_->normals->at(vertexIndex_[1]) * u +
                        mesh_->normals->at(vertexIndex_[2]) * v;

        Point2f uvs[3];
        get_uv(uvs);
        Vector2f uv_tmp = static_cast<Vector2f>(uvs[0] * (1 - u - v)) +
                          static_cast<Vector2f>(uvs[1] * u) +
                          static_cast<Vector2f>(uvs[2] * v);
        isect->texcoords = Point2f{uv_tmp.x, uv_tmp.y};
        isect->happened = true;
      }
      return true;
    }
    return false;
  }
  std::pair<Intersection, float> sample(const Point3f &ref) const override {

    Intersection pos;
    float pdf;
    std::tie(pos, pdf) = sample();
    Ray ray(ref, pos.coords - ref);
    Intersection light_isect;
    if (this->Intersect(ray) || Bounds3::Inside(ref, this->WorldBounds())) {
      return {pos, pdf};
    } else {
      return {pos, 0.0f};
    }
  }

private:
  void get_uv(Point2f uv[3]) const {
    if (mesh_->uvs) {
      uv[0] = mesh_->uvs->at(vertexIndex_[0]);
      uv[1] = mesh_->uvs->at(vertexIndex_[1]);
      uv[2] = mesh_->uvs->at(vertexIndex_[2]);
    } else {
      uv[0] = {0, 0};
      uv[1] = {1, 0};
      uv[2] = {1, 1};
    }
  }

  std::shared_ptr<TriangleMesh> mesh_;
  const int *vertexIndex_;
  int faceIndex;
  int triIndex_;

  //  Point3f v0,v1,v2;
};
NAMESPACE_END(DR)
