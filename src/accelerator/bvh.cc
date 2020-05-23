#include <accelerator/bvh.h>
#include <utils/high_resolution_timer.h>
using namespace DR;
Bounds3 BVHTree::WorldBounds() const {
  return root_ ? root_->bounds : Bounds3();
}

bool BVHTree::Intersect_test(const Ray &ray) const {
  if (!root_)
    return false;
  return this->Intersect(root_, ray, nullptr);
}

bool BVHTree::Intersect(const Ray &ray, Intersection *isect) const {
  return this->Intersect(root_, ray, isect);
}

BVHTree::BVHTree(std::vector<std::shared_ptr<Primitive>> prims,
                 uint maxPrimsInNode, BVHTree::Method splitmethod) {
  this->maxPrimsInNode = maxPrimsInNode;
  this->method = splitmethod;
  this->prims_ = prims;
  HRTimer timer;
  timer.start();
  root_ = this->recursiveBuild(this->prims_);
  timer.end();
  std::cout << "BVH build costs " << timer.elapsed() << " ms\n" << std::endl;
}

bool BVHTree::Intersect(std::shared_ptr<BVHTree::BvhNode> node, const Ray &ray,
                        Intersection *isect) const {
  if (!node) // nullptr
    return false;

  Intersection res;
  auto dir_is_neg = ray.DirIsNeg();
  if (!node->bounds.Intersect_test(ray, 1 / ray.direction_, dir_is_neg))
    return false;
  if (node->left_ == nullptr && node->right_ == nullptr) // leaf
  {
    for (const auto &primitive : node->primitives_) {
      Intersection tmp;
      if (primitive->Intersect(ray, &tmp)) {
        if (tmp.t < res.t) {
          res = tmp;
        }
      }
    }
    if (res.happened) {
      if (isect != nullptr) {
        *isect = res;
      }
      return true;
    }
    return false;
  }
  Intersection hit_left;
  Intersection hit_right;
  this->Intersect(node->left_, ray, &hit_left);
  this->Intersect(node->right_, ray, &hit_right);
  if (isect != nullptr) {
    if (hit_left.happened && hit_right.happened) {
      *isect = (hit_left.t < hit_right.t ? hit_left : hit_right);
    } else if (hit_left.happened) {
      *isect = hit_left;
    } else if (hit_right.happened) {
      *isect = hit_right;
    } else {
      return false;
    }
  } else {
    return hit_left.happened || hit_right.happened;
  }
  // should never reach here
  return true;
}

std::shared_ptr<BVHTree::BvhNode>
BVHTree::recursiveBuild(std::vector<std::shared_ptr<Primitive>> prims) {
  auto node = std::make_shared<BvhNode>();
  Bounds3 bounds;
  for (const auto &i : prims) {
    bounds = Bounds3::Union(bounds, i->WorldBounds());
  }
  if (prims.size() <= this->maxPrimsInNode) {
    node->bounds = bounds;
    node->primitives_ = prims;
    node->left_ = nullptr;
    node->right_ = nullptr;
    node->nPrimitives = prims.size();
    return node;
  }
  if (prims.size() <= 2 * this->maxPrimsInNode) {
    node->left_ = recursiveBuild(std::vector<std::shared_ptr<Primitive>>(
        prims.cbegin(), prims.cbegin() + maxPrimsInNode));
    node->right_ = recursiveBuild(std::vector<std::shared_ptr<Primitive>>(
        prims.cbegin() + maxPrimsInNode, prims.cend()));
    node->bounds = Bounds3::Union(node->left_->bounds, node->right_->bounds);
    return node;
  }
  // split
  else {
    Bounds3 centroidBounds;
    for (const auto &i : prims) {
      centroidBounds =
          Bounds3::Union(centroidBounds, i->WorldBounds().centroid());
    }
    int dim = centroidBounds.max_extent();
    switch (dim) {
    case 0: // x
      std::nth_element(prims.begin(), prims.begin() + prims.size() / 2,
                       prims.end(), [](auto lhs, auto rhs) {
                         return lhs->WorldBounds().centroid().x <
                                rhs->WorldBounds().centroid().x;
                       });
      break;
    case 1: // y
      std::nth_element(prims.begin(), prims.begin() + prims.size() / 2,
                       prims.end(), [](auto lhs, auto rhs) {
                         return lhs->WorldBounds().centroid().y <
                                rhs->WorldBounds().centroid().y;
                       });
      break;
    case 2: // z
      std::nth_element(prims.begin(), prims.begin() + prims.size() / 2,
                       prims.end(), [](auto lhs, auto rhs) {
                         return lhs->WorldBounds().centroid().z <
                                rhs->WorldBounds().centroid().z;
                       });
      break;
    }
    auto vec_begin = prims.cbegin();
    auto vec_mid = prims.cbegin() + prims.size() / 2;
    auto vec_end = prims.cend();
    auto left_vector =
        std::vector<std::shared_ptr<Primitive>>{vec_begin, vec_mid};
    auto right_vector =
        std::vector<std::shared_ptr<Primitive>>{vec_mid, vec_end};
    node->left_ = recursiveBuild(left_vector);
    node->right_ = recursiveBuild(right_vector);
    node->bounds = Bounds3::Union(node->left_->bounds, node->right_->bounds);
  }
  return node;
}
