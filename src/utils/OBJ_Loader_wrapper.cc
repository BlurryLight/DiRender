#include <material/matte_material.h>
#include <shapes/triangle.h>
#include <texture/constant_texture.h>
#include <utils/OBJ_Loader_wrapper.h>
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-compare"
#pragma GCC diagnostic ignored "-Wunused-value"
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
#endif
#include "../third_party/OBJ_Loader.h"
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif
using namespace DR;
Model::Model(observer_ptr<Transform> LocalToWorld,
             observer_ptr<Transform> WorldToLocal, const std::string &path,
             std::shared_ptr<Material> material) {
  objl::Loader loader;
  if (!loader.LoadFile(path)) {
    std::cerr << path << "cannot be loaded!" << std::endl;
    return;
  }
  for (size_t j = 0; j < loader.LoadedMeshes.size(); j++) {
    auto mesh = loader.LoadedMeshes[j];
    int nTriangles = mesh.Indices.size() / 3;
    int nVertices = mesh.Vertices.size();
    const uint *meshIndices = mesh.Indices.data();
    Point3f *vertices = new Point3f[mesh.Vertices.size()];
    Normal3f *normals = new Normal3f[mesh.Vertices.size()];
    Point2f *uvs = new Point2f[mesh.Vertices.size()];
    //    int *faceIndices = new int[nTriangles]();
    //    std::fill_n(faceIndices, nTriangles, j);
    for (size_t i = 0; i < mesh.Vertices.size(); i++) {
      vertices[i].x = mesh.Vertices[i].Position.X;
      vertices[i].y = mesh.Vertices[i].Position.Y;
      vertices[i].z = mesh.Vertices[i].Position.Z;

      normals[i].x = mesh.Vertices[i].Normal.X;
      normals[i].y = mesh.Vertices[i].Normal.Y;
      normals[i].z = mesh.Vertices[i].Normal.Z;
      normals[i] = normals[i].normalize();

      uvs[i].x = mesh.Vertices[i].TextureCoordinate.X;
      uvs[i].y = mesh.Vertices[i].TextureCoordinate.Y;
    }

    this->meshes.emplace_back(std::make_shared<TriangleMesh>(
        LocalToWorld, nTriangles, nVertices, meshIndices, vertices, normals,
        uvs, nullptr));
  }
  std::vector<std::shared_ptr<Primitive>> tris;
  for (uint mesh_index = 0; mesh_index < meshes.size(); mesh_index++) {
    for (int tri_index = 0; tri_index < meshes[mesh_index]->nTriangles;
         tri_index++) {
      auto tri_shape = std::make_shared<Triangle>(
          LocalToWorld, WorldToLocal, false, meshes[mesh_index], tri_index);
      tris.emplace_back(
          std::make_shared<GeometricPrimitive>(tri_shape, material)

      );
    }
  }
  std::shared_ptr<Primitive> bvh_tree = std::make_shared<BVHTree>(tris);
  model_ptr = bvh_tree;
}
