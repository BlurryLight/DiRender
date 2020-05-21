#pragma once
#include <cameras/camera.h>
#include <cores/primitive.h>
#include <map>
#include <unordered_map>
#include <vector>

NAMESPACE_BEGIN(DR)
struct Scene {
  Scene() {}
  Scene(const Scene &other) = delete;
  Scene &operator=(const Scene &other) = delete;

  void add(std::shared_ptr<Camera> cam) { cams_.push_back(std::move(cam)); }
  void add(std::shared_ptr<Primitive> prim) {
    Prims_.push_back(std::move(prim));
  }
  std::vector<std::shared_ptr<Camera>> cams_;
  std::vector<std::shared_ptr<Primitive>> Prims_;
};
NAMESPACE_END(DR)
