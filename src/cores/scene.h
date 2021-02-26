#pragma once
#include <cameras/camera.h>
#include <cores/primitive.h>
#include <map>
#include <unordered_map>
#include <vector>

NAMESPACE_BEGIN(DR)
namespace IMPL {
class TransTable {
public:
  TransTable() = default;
  TransTable(const TransTable &) = delete;
  TransTable &operator=(const TransTable &) = delete;
  // if not exsits, insert then get
  observer_ptr<Transform> get_tf(const Matrix4 &mat) const;
  observer_ptr<Transform> get_tf(const Transform &trans) const;

  using const_tf_pair =
      std::pair<observer_ptr<Transform>, observer_ptr<Transform>>;
  const_tf_pair get_tf_and_inv(const Matrix4 &mat) const;
  const_tf_pair get_tf_and_inv(const Transform &trans) const;

private:
  using key_type = DR::Transform;
  using value_type = std::unique_ptr<const DR::Transform>;
  using tf_table_t = std::unordered_map<key_type, value_type>;
  mutable tf_table_t tf_table_;
};
} // namespace IMPL
struct Scene {
  Scene() = default;
  Scene(const Scene &other) = delete;
  Scene &operator=(const Scene &other) = delete;

  void add(const std::shared_ptr<Camera> &cam) { cams_.push_back(cam); }
  void add(const std::shared_ptr<Primitive> &prim) { prims_.push_back(prim); }
  std::vector<std::shared_ptr<Camera>> cams_;
  std::vector<std::shared_ptr<Primitive>> prims_;
  std::vector<std::shared_ptr<Primitive>> light_shapes_;
  Vector3f background_{0};
  // has ownership
  IMPL::TransTable trans_table;
};
NAMESPACE_END(DR)
