#pragma once
#include <cores/bounds.h>
#include <cores/intersection.hpp>
#include <math/geometry.hpp>
#include <math/transform.h>
#include <memory>
#include <shapes/shape.h>
#include <utils/di_global.h>
NAMESPACE_BEGIN(DR)

struct Material;

struct Primitive {
  Primitive() {}
  virtual ~Primitive(){};
  virtual Bounds3 WorldBounds() const = 0;
  virtual bool Intersect(const Ray &, Intersection *) const { return false; };
  virtual bool Intersect_test(const Ray &) const { return false; };
  virtual float Area() const = 0;

  virtual std::pair<Intersection, float> sample() const = 0;
  virtual std::pair<Intersection, float> sample(const Point3f &) const = 0;
};

// binding material with shape
struct GeometricPrimitive : public Primitive {
  GeometricPrimitive(std::shared_ptr<Shape> shape,
                     std::shared_ptr<Material> mat)
      : shape_(shape), mat_(mat) {}
  virtual Bounds3 WorldBounds() const override { return shape_->WorldBounds(); }
  virtual bool Intersect(const Ray &ray, Intersection *isect) const override {
    auto flag = shape_->Intersect(ray, nullptr, isect);
    if (flag && isect != nullptr) {
      isect->mat_ptr = mat_.get();
      isect->primi_ptr = (Primitive *)this;
    }
    return flag;
  }
  virtual bool Intersect_test(const Ray &ray) const override {
    return shape_->Intersect(ray, nullptr, nullptr);
  }

  virtual std::pair<Intersection, float> sample() const override {
    return this->shape_->sample();
  }
  virtual std::pair<Intersection, float>
  sample(const Point3f &ref) const override {
    return this->shape_->sample(ref);
  }
  virtual float Area() const override { return shape_->Area(); }

private:
  std::shared_ptr<Shape> shape_;
  std::shared_ptr<Material> mat_;
};

struct TransformedPrimitive : public Primitive
{
  TransformedPrimitive(std::shared_ptr<Primitive> prim,
                     std::shared_ptr<Transform> trans)
      : prim_(prim), trans_(trans) {}
  // transform the AABB
  Bounds3 WorldBounds() const override {
    return (*trans_)(prim_->WorldBounds());
  }
  //transform the ray
  bool Intersect_test(const Ray &ray) const override;
  bool Intersect(const Ray &ray, Intersection *isect) const override;
  std::pair<Intersection, float> sample() const override {
    return this->prim_->sample();
  }

  std::pair<Intersection, float> sample(const Point3f &ref) const override {
    return this->prim_->sample(ref);
  }
  float Area() const override { return prim_->Area(); }

private:
  std::shared_ptr<Primitive> prim_;
  std::shared_ptr<Transform> trans_;
};


NAMESPACE_END(DR)
