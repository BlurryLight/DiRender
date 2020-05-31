#pragma once
#include <cores/intersection.hpp>
#include <cores/primitive.h>
#include <math/geometry.hpp>
#include <vector>
NAMESPACE_BEGIN(DR)
class BVHTree : public Primitive {
public:
  enum class Method { NAIVE, SAH };
  Bounds3 WorldBounds() const override;
  bool Intersect_test(const Ray &ray) const override;
  bool Intersect(const Ray &ray, Intersection *isect) const override;
  BVHTree(std::vector<std::shared_ptr<Primitive>> prims,
          uint maxPrimsInNode = 1, Method splitmethod = Method::NAIVE);

  virtual std::pair<Intersection, float> sample() const override {
    return this->sample_a_node(root_);
  }
  virtual std::pair<Intersection, float>
  sample(const Point3f &ref) const override {
    return this->sample_a_node(root_, ref);
  }

private:
  struct BvhNode;
  bool Intersect(std::shared_ptr<BvhNode> node, const Ray &ray,
                 Intersection *isect) const;
  struct BvhNode {
    BvhNode() {
      bounds = Bounds3();
      left_ = right_ = nullptr;
      primitives_ = decltype(primitives_){};
      //      area = 0.0f;
      nPrimitives = 0;
    }
    Bounds3 bounds;
    std::shared_ptr<BvhNode> left_;
    std::shared_ptr<BvhNode> right_;
    std::vector<std::shared_ptr<Primitive>> primitives_;
    float area;
    int nPrimitives;
  };
  std::vector<std::shared_ptr<Primitive>> prims_;
  std::shared_ptr<BvhNode> root_;
  uint maxPrimsInNode = 1;
  Method method = Method::NAIVE;
  std::shared_ptr<BvhNode>
  recursiveBuild(std::vector<std::shared_ptr<DR::Primitive>> prims);
  std::pair<Intersection, float> sample_a_node(std::shared_ptr<BvhNode> node,
                                               const Point3f &ref) const {
    if (node == nullptr) {
      return {};
    }
    if (node->left_ == nullptr && node->right_ == nullptr) {
      int size = node->primitives_.size();
      int index = std::floor(size * get_random_float());
      return node->primitives_.at(index)->sample(ref);
    }
    if (node->left_ == nullptr)
      return sample_a_node(node->right_, ref);
    else if (node->right_ == nullptr)
      return sample_a_node(node->left_, ref);
    else {
      if (get_random_float() < 0.5)
        return sample_a_node(node->right_, ref);
      else
        return sample_a_node(node->left_, ref);
    }
  }
  std::pair<Intersection, float>
  sample_a_node(std::shared_ptr<BvhNode> node) const {
    if (node == nullptr) {
      return {};
    }
    if (node->left_ == nullptr && node->right_ == nullptr) {
      int size = node->primitives_.size();
      int index = std::floor(size * get_random_float());
      return node->primitives_.at(index)->sample();
    }
    if (node->left_ == nullptr)
      return sample_a_node(node->right_);
    else
      return sample_a_node(node->left_);
  }
};

NAMESPACE_END(DR)
