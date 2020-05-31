#include <math/transform.h>
#include <shapes/triangle.h>
using namespace DR;

TriangleMesh::TriangleMesh(std::shared_ptr<Transform> LocalToWorld,
                           int nTriangles, int nVertices,
                           const uint *vertexIndices, const Point3f *vertices,
                           const Normal3f *normals, const Point2f *uvs,
                           const int *faceIndices)
    : nTriangles(nTriangles), nVertices(nVertices) {
  if (vertices) {
    this->vertices = std::make_unique<std::vector<Point3f>>();
    this->vertexIndices.assign(vertexIndices, vertexIndices + 3 * nTriangles);
    this->vertices->assign(vertices, vertices + nVertices);
    for (int i = 0; i < nVertices; ++i) {
      this->vertices->at(i) = (*LocalToWorld)(*(vertices + i));
    }
    this->edges = std::make_unique<std::vector<Vector3f>>();
    this->edges->resize(3 * nTriangles);
    for (int i = 0; i < nTriangles; i++) {
      this->edges->at(3 * i) =
          this->vertices->at(this->vertexIndices.at(3 * i)) -
          this->vertices->at(this->vertexIndices.at(3 * i + 1));

      this->edges->at(3 * i + 1) =
          this->vertices->at(this->vertexIndices.at(3 * i + 1)) -
          this->vertices->at(this->vertexIndices.at(3 * i + 2));

      this->edges->at(3 * i + 2) =
          this->vertices->at(this->vertexIndices.at(3 * i + 2)) -
          this->vertices->at(this->vertexIndices.at(3 * i + 0));
    }
  }
  if (uvs) {
    this->uvs = std::make_unique<std::vector<Point2f>>();
    this->uvs->assign(uvs, uvs + nVertices);
  }
  if (normals) {
    this->normals = std::make_unique<std::vector<Normal3f>>();
    this->normals->resize(nVertices);
    for (int i = 0; i < nVertices; ++i) {
      this->normals->at(i) = (*LocalToWorld)(normals[i], false).normalize();
    }
  }
  if (faceIndices) {
    this->faceIndices = std::vector<int>(faceIndices, faceIndices + nTriangles);
  }
}
