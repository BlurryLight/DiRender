#pragma once
// load a obj return a BVH wrapper
#include <accelerator/bvh.h>
#include <cores/primitive.h>
#include <memory>
#include <shapes/triangle.h>
#include <texture/texture.h>
NAMESPACE_BEGIN(DR)
struct Model {
  Model(std::shared_ptr<Transform> LocalToWorld, const std::string &path,
        std::shared_ptr<DR::Material> material);
  std::shared_ptr<Primitive> model_ptr = nullptr;

private:
  std::vector<std::shared_ptr<TriangleMesh>> meshes;
};

NAMESPACE_END(DR)
